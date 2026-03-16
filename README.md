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
cd corpusbuilder
mkdir build
cd build
cmake ..
cmake 
```
* **Windows:**
```powershell
git clone "https://github.com/samAmabile/CorpusCreator.git"
cd corpusbuilder
mkdir build
cd build
cmake .. 
cmake --build . --config Release
```

## Run
* executable is `scraper`
* program takes 5 arguments:
1. executable 
2. number of Wikipedia pages to scrape (*x10*)
3. number of StackExchange pages to scrape
4. number of books to scrape from Project Gutenberg
5. filename to save .csv (optional, defaults to `corpus_yyyy-mm-dd.csv`)
*example call
```bash 
# build corpus with 50 wiki pages, 12 stackexchange pages and 4 books and save to corpus_x.csv:
./scraper 5 12 4 corpus_x.csv
```





