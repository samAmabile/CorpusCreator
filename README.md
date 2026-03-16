# CorpusCreator
A C++ tool for building linguistic corpora from Wikipedia, StackExchange and Project Gutenberg

## Requirements
* **Compiler:** C++17 or higher
* **Build System:** CMake 3.10+
* **Dependencies:**
    * `cpr` (for HTTP requests)
    * `nlohmann-json` (for JSON parsing)
    * `zstd` (for file compression)

## Build
* **Linux:**
```bash
git clone "https://github.com/samAmabile/CorpusCreator.git"
cd CorpusCreator
mkdir build
cd build
cmake ..
cmake --build . 
```
* **Windows:**
**note:** requires the [MSVC compiler](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022) that comes with Visual Studio Build Tools. 
* Build and run from **Developer Powershell for VS**
* Make sure you have `vcpkg` installed and `VCPKG_ROOT` environment variable set

```developer powershell for VS
git clone "https://github.com/samAmabile/CorpusCreator.git"
cd CorpusCreator
mkdir build
cd build
cmake .. 
cmake --build . --config Release
```

## Run
* executable is `scraper`
* program takes 5 arguments:
1. executable 
2. number(*x10*) of Wikipedia pages to scrape 
3. number(*x10*) of StackExchange pages to scrape
4. number of books to scrape from Project Gutenberg
5. filename to save .csv (optional, defaults to `corpus_yyyy-mm-dd.csv`)

**example call**
```bash 
# build corpus with 50 wiki pages, 120 stackexchange pages and 4 books and save to corpus_x.csv:
./scraper 5 12 4 corpus_x.csv
```





