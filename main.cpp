#include <cstdlib>
#include <filesystem>
#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <portable-file-dialogs.h>
#include <ranges>
#include <regex>

namespace fs = std::filesystem;

auto make_file_list(std::vector<std::string> const &playlists) -> std::vector<fs::path> {
    std::vector<fs::path> files;
    std::string line;
    std::regex media_path(R"/(media src="([^"]+)")/");
    std::regex apos("&apos;");
    for (const auto &playlist : playlists) {
        fs::path base_dir = fs::path(playlist).remove_filename();
        std::ifstream playlist_fs(playlist);
        while (std::getline(playlist_fs, line)) {
            std::smatch match;
            if (std::regex_search(line, match, media_path)) {
                std::string filepath = std::regex_replace(match[1].str(), apos, "'");
                if constexpr (fs::path::preferred_separator == '/') {
                    // paths are in Windows format, convert to run in *nix.
                    std::replace(filepath.begin(), filepath.end(), '\\', '/');
                }
                files.emplace_back(base_dir / fs::path(std::move(filepath)));
            }
        }
    }
    return files;
}

auto media_convert(fs::path const &source, fs::path const &destination) -> void {
    const std::string src(source);
    const std::string dst(destination);
    fmt::print(R"(Converting "{}" to "{}".)"
               "\n",
               src, dst);
    std::flush(std::cout);
    auto cmd = fmt::format(R"(ffmpeg -v 0 -i "{}" "{}")", src, dst);
    std::system(cmd.c_str());
}

auto normalize_volume(fs::path const &source, fs::path const &destination) -> void {
    const std::string src(source);
    const std::string dst(destination);
    fmt::print("Normalizing volume from files...\n");
    std::flush(std::cout);
    auto cmd = fmt::format(R"(bs1770gain --quiet "{}" -ao "{}")", src, dst);
    std::system(cmd.c_str());
}

auto get_playlists_from_cli(std::span<const char *> const &args) -> std::vector<std::string> {
    std::vector<std::string> list_selection(std::begin(args) + 1, std::end(args) - 1);
    return list_selection;
}

auto get_playlists_from_file_dialog() -> std::vector<std::string> {
    std::vector<std::string> list_selection;
    while (true) {
        auto playlist = pfd::open_file("Select playlist file(s), cancel to finish selection.", ".", {"Playlist", "*.wpl"}).result();
        if (playlist.empty()) {
            break;
        }
        list_selection.emplace_back(std::move(playlist[0]));
    }
    return list_selection;
}

auto main(int argc, const char *argv[]) -> int {
    const auto args = std::span(argv, argc);

    // get playlist files in order
    std::vector<std::string> playlists;
    if (args.size() >= 3) {
        playlists = get_playlists_from_cli(args);
    }
    else {
        playlists = get_playlists_from_file_dialog();
    }
    if (playlists.empty()) {
        std::cout << "Add at least one playlist file to continue.\n";
        return 1;
    }
    auto mediafiles = make_file_list(playlists);

    // output directory
    std::string outdir;
    if (args.size() >= 3) {
        outdir = *(std::end(args) - 1);
    }
    else {
        auto dir_selection = pfd::select_folder("Select output directory.", "./out").result();
        if (dir_selection.empty()) {
            std::cout << "Output directory not defined, exiting.\n";
            return 1;
        }
        outdir = dir_selection;
    }
    fs::path output_path(outdir);
    if (!fs::exists(output_path)) {
        if (!fs::create_directory(output_path)) {
            std::cout << "Couldn't create output directory.\n";
            return 1;
        }
    }
    else {
        if (!fs::is_directory(output_path)) {
            std::cout << "Output path should not be a file.\n";
            return 1;
        }
    }

    auto temp_dir = fs::temp_directory_path() / fs::path("wpl2folder");
    if (fs::exists(temp_dir)) {
        fs::remove_all(temp_dir);
    }
    fs::create_directory(temp_dir);
    std::cout << "Copying and renaming playlist media files...\n";
    for (std::size_t i = 1; auto const &src : mediafiles) {
        fs::path dest(fmt::format("{:03} - {}", i, src.filename().string()));
        fs::copy_file(src, temp_dir / dest);
        i++;
    }

    std::cout << "Normalizing volume...\n";
    auto processed_dir = temp_dir / fs::path("processed");
    normalize_volume(temp_dir, processed_dir);

    std::cout << "Writing mp3 files into the output directory...\n";
    for (auto const &file : fs::directory_iterator(processed_dir)) {
        if (!file.is_regular_file()) {
            continue;
        }
        auto dest = output_path / file.path().filename();
        dest.replace_extension(".mp3");
        media_convert(file, dest);
    }

    std::cout << "Cleaning-up temporary files...\n";
    fs::remove_all(temp_dir);

    std::cout << "All done!\n";
}
