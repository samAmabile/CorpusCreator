#CMakeLists for corpusbuilder v0:
cmake_minimum_required(VERSION 3.10)
project(corpusbuilder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#conditional for windows users:
if(WIN32 AND NOT CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VCPKG_ROOT})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
            CACHE FILEPATH "vcpkg toolchain file")
    else()
        message(STATUS "Note: VCPKG_ROOT not found. if build fails set -DCMAKE_TOOLCHAIN_FILE")
    endif()
endif()

find_package(cpr REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(zstd REQUIRED)

add_executable(scraper
    main.cpp
    Crawler.cpp
)
#just to make totally sure we find zstd on any system:
if(TARGET zstd::libzstd_static)
    set(ZSTD_LIBS zstd::libzstd_static)
elseif(TARGET zstd::libzstd_shared)
    set(ZSTD_LIBS zstd::libzstd_shared)
else()
    set(ZSTD_LIBS zstd)
endif()

target_link_libraries(scraper PRIVATE 
    cpr::cpr 
    nlohmann_json::nlohmann_json 
    ${ZSTD_LIBS}
)


