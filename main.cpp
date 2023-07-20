#include <filesystem>
#include <fmt/core.h>
#include <nowide/args.hpp>
#include <nowide/cstdlib.hpp>
#include <nowide/fstream.hpp>
#include <nowide/iostream.hpp>
#include <ranges>
#include <regex>

// include portable-file-dialogs last as it includes windows.h
// and pollutes the compilation with some defines that clashes
// with nowide library
#include <portable-file-dialogs.h>

// messages
#ifdef LANG_PTBR
#include "messages_ptbr.h"
#else
#include "messages.h"
#endif

namespace fs = std::filesystem;
namespace nw = nowide;

auto make_file_list(std::vector<std::string> const &playlists) -> std::vector<fs::path> {
    std::vector<fs::path> files;
    std::string line;
    std::regex media_path(R"/(media src="([^"]+)")/");
    std::regex apos("&apos;");
    for (const auto &playlist : playlists) {
        fs::path base_dir = fs::path(playlist).remove_filename();
        nw::ifstream playlist_fs(playlist);
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
    auto src = source.string();
    auto dst = destination.string();
    fmt::print(MSG_CONVERTING, src, dst);
    std::flush(std::cout);
    auto cmd = fmt::format(R"(ffmpeg -v 0 -i "{}" "{}")", src, dst);
    nw::system(cmd.c_str());
}

auto normalize_volume(fs::path const &source, fs::path const &destination) -> void {
    auto src = source.string();
    auto dst = destination.string();
    std::flush(std::cout);
    auto cmd = fmt::format(R"(bs1770gain --quiet "{}" -ao "{}")", src, dst);
    nw::system(cmd.c_str());
}

auto get_playlists_from_cli(std::span<char *> const &args) -> std::vector<std::string> {
    std::vector<std::string> list_selection(std::begin(args) + 1, std::end(args) - 1);
    return list_selection;
}

auto get_playlists_from_file_dialog() -> std::vector<std::string> {
    std::vector<std::string> list_selection;
    while (true) {
        auto playlist = pfd::open_file(MSG_SELECT_PLAYLIST, ".", {"Playlist", "*.wpl"}).result();
        if (playlist.empty()) {
            break;
        }
        list_selection.emplace_back(std::move(playlist[0]));
    }
    return list_selection;
}

auto main(int argc, char *argv[]) -> int {
    nw::args args_to_utf8(argc, argv);
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
        nw::cout << MSG_NO_PLAYLISTS;
        return 1;
    }
    auto mediafiles = make_file_list(playlists);

    // output directory
    std::string outdir;
    if (args.size() >= 3) {
        outdir = *(std::end(args) - 1);
    }
    else {
        auto dir_selection = pfd::select_folder(MSG_SELECT_OUTPUT, "./out").result();
        if (dir_selection.empty()) {
            nw::cout << MSG_NO_OUTPUT;
            return 1;
        }
        outdir = dir_selection;
    }
    fs::path output_path(outdir);
    if (!fs::exists(output_path)) {
        if (!fs::create_directory(output_path)) {
            nw::cout << MSG_CANT_CREATE_OUTPUT;
            return 1;
        }
    }
    else {
        if (!fs::is_directory(output_path)) {
            nw::cout << MSG_INVALID_OUTPUT;
            return 1;
        }
    }

    auto temp_dir = fs::temp_directory_path() / fs::path("wpl2folder");
    if (fs::exists(temp_dir)) {
        fs::remove_all(temp_dir);
    }
    fs::create_directory(temp_dir);
    nw::cout << MSG_COPYING_MEDIA;
    for (std::size_t num = 1; auto const &src : mediafiles) {
        fs::path dest(fmt::format("{:03} - {}", num, src.filename().string()));
        fs::copy_file(src, temp_dir / dest);
        num++;
    }

    nw::cout << MSG_NORMALIZING;
    auto processed_dir = temp_dir / fs::path("processed");
    normalize_volume(temp_dir, processed_dir);

    nw::cout << MSG_COPYING_MP3;
    for (auto const &file : fs::directory_iterator(processed_dir)) {
        if (!file.is_regular_file()) {
            continue;
        }
        auto dest = output_path / file.path().filename();
        dest.replace_extension(".mp3");
        media_convert(file, dest);
    }

    nw::cout << MSG_CLEANING_TMP;
    fs::remove_all(temp_dir);

    nw::cout << MSG_DONE;
}
