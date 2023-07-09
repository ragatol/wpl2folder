# wpl2folder

A small application that takes one or more Windows Media Player playlist files
and exports them to a folder, while converting and normalizing the volume of
all media files included.

Made for my dad, that wanted to create an USB disk with many backing music
to play with his tenor saxophone, with his mixer.

The mixer can only play mp3 files in alphabetical order, so there was a
need to convert all media, videos included, to mp3. Not only that,
but normalizing the volume is important so there's no need to make
volume adjustments every time another music plays.

## Dependencies

Applications installed and found via PATH environment variable:

- ffmpeg
- bs1770gain
- cmake 3.12+
- vcpkg

A vcpkg.json file is included and used with vcpkg configured in CMakeLists.txt
so libraries dependencies are installed before building.

Libraries used:

- [fmt](https://github.com/fmtlib/fmt)
- [nowide](https://github.com/boostorg/nowide)
- [portable-file-diagogs](https://github.com/samhocevar/portable-file-dialogs)

## Building

To build, first change the path to `vcpkg.cmake` file to your installation of
vcpkg in `CMakeLists.txt`.

Create the build folder with:

`cmake -B build -S .`

You can add `-DCMAKE_BUILD_TYPE=Debug` for a Debug build, or
`-DCMAKE_BUILD_TYPE=Release` for a Release build on Linux.

On Linux, build the application with:

`cmake --build build`

The application will be in the build folder.

On Windows, open the *.sln file inside the build folder with Visual Studio,
select the build type (Debug or Release) and compile the solution. The
application will be inside the respective folder (Debug or Release).

## Usage:

Command line usage:

`wpl2folder <one or more playlist files separated by space> output_folder`

Running without arguments:

A file selection dialog will be shown to select each playlist file. To finish
selecting playlists, cancel the selection. Multi-selection is not used
because it's less intuitive for my father.

Then a folder selection dialog will be shown to select the output directory.

---

Uma pequena aplicação que permite escolher uma ou várias playlists geradas pelo
Windows Media Player e exporta todo o conteúdo delas para uma pasta, enquanto
converte para mp3 e normaliza o volume.

Feito para meu pai, que precisava de criar um pendrive USB com a lista de
músicas que ele montava no WMP, para tocar ao vivo com o backing da lista,
usando sua mesa de mixagem.

A mesa em questão apenas suporta mp3, e apenas reproduz os arquivos em ordem
alfabética, portanto todos os arquivos de mídia são convertidos para mp3,
nomeados de acordo a manter sua ordem na playlist. O volume dos arquivos também
são normalizados para que ele não precise ajustar o volume da mesa toda vez que
uma música diferente é tocada.

## Dependências

Aplicativos instalados e encontrados via variável de ambiente PATH:

- ffmpeg
- bs1770gain
- cmake 3.12+
- vcpkg

Um arquivo vcpkg.json está incluso para instalar as bibliotecas necessárias
antes da compilação.

Libraries used:

- [fmt](https://github.com/fmtlib/fmt)
- [nowide](https://github.com/boostorg/nowide)
- [portable-file-diagogs](https://github.com/samhocevar/portable-file-dialogs)

## Compilando

Para compilar, primeiramente mude o caminho para o arquivo `vcpkg.cmake` para
onde está na sua instalação do cmake em `CMakeLists.txt`.

Então crie a pasta de build com:

`cmake -B build -S .`

Você pode adicionar `-DCMAKE_BUILD_TYPE=Debug` para uma build Debug ou
`-DCMAKE_BUILD_TYPE=Release` pra uma build Release no Linux.

No Linux, compile a aplicação com:

`cmake --build build`

O executável estará dentro da pasta build.

No Windows, abra o arquivo .sln dentro da pasta build com o Visual Studio,
selecione o tipo de build (Debug ou Release) e compile a solução. A
aplicação estará dentro da pasta respectiva (Debug ou Release).

## Uso

Usando na linha de comando:

`wpl2folder <arquivos de playlist separados por espaço> pasta_destino`

Usando sem argumentos:

Um diálogo para seleção de arquivo vai ser mostrado para cada arquivo de playlist. Para terminar a seleção, cancele.
Não utilizei um diálogo com seleção múltipla de arquivo pois isso é mais confuso para meu pai.

Depois um diálogo para seleção da pasta destino é mostrada para definir onde deve ser gravado o resultado.

