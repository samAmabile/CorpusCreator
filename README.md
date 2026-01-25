# CorpusCreator
A C++ tool for building linguistic corpora from Wikipedia and Reddit.

## Requirements
* C++17
* vcpkg (cpr, nlohmann-json)
* CMake 3.10+

## Build
```powershell
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE="C:/Users/swama/vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build . --config Release
