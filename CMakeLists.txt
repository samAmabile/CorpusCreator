cmake_minimum_required(VERSION 3.10)
project(corpusbuilder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(scraper
    main.cpp
    Crawler.cpp
)

target_link_libraries(scraper PRIVATE cpr::cpr nlohmann_json::nlohmann_json)


