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

## Usage

Dependencies:

- ffmpeg and bs1170gain installed and found via PATH environment variable.
- cmake 3.12+
- vcpkg, to automatically install:
    - [fmt](https://github.com/fmtlib/fmt)
    - [portable-file-diagogs](https://github.com/samhocevar/portable-file-dialogs)

A vcpkg.json file is included and used with vcpkg configured in CMakeLists.txt
so libraries dependencies are installed before building. ffmpeg and bs1170gain
must be installed manually.

To build, first change the path to `vcpkg.cmake` file to your installation of
vcpkg in the `CMakeLists.txt` file and then run on your cli:

`cmake -B build -S .`

You can add `-D CMAKE_BUILD_TYPE=Debug` for a Debug build, or
`-D CMAKE_BUILD_TYPE=Release` for a Release build.

Finally, build with:

`cmake --build build`

The application will be inside the build folder.

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

## Uso

Dependencias:

- ffmpeg and bs1170gain instalados e com entradas na variável de ambiente PATH
- cmake 3.12+
- vcpkg, para instalar automaticamente:
    - [fmt](https://github.com/fmtlib/fmt)
    - [portable-file-diagogs](https://github.com/samhocevar/portable-file-dialogs)

Um arquivo vcpkg.json está incluso e é utilizado com vcpkg configurado em
CMakeLists.txt para que as dependências de bibliotecas sejam instaladas antes
da compilação. ffmpeg e bs1170gain devem ser instalados e configurados
manualmente.

Para compilar, primeiramente ajuste o caminho para o arquivo `vcpkg.cmake` da
sua instalação do vcpkg no arquivo `CMakeLists.txt`. Depois execute numa linha
de comando:

`cmake -B build -S .`

Você pode adicionar `-D CMAKE_BUILD_TYPE=Debug` para criar uma build Debug,
ou `-D CMAKE_BUILD_TYPE=Release` para uma build Release.

Finalmente, compile com:

`cmake --build build`

A aplicação estará dentro da pasta build.

Usando na linha de comando:

`wpl2folder <arquivos de playlist separados por espaço> pasta_destino`

Usando sem argumentos:

Um diálogo para seleção de arquivo vai ser mostrado para cada arquivo de playlist. Para terminar a seleção, cancele.
Não utilizei um diálogo com seleção múltipla de arquivo pois isso é mais confuso para meu pai.

Depois um diálogo para seleção da pasta destino é mostrada para definir onde deve ser gravado o resultado.

