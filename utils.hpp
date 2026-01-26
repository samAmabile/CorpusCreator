#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <regex>
#include <zstd.h>
#include <vector>
#include <iostream>
#include <fstream>



namespace Utils {
    inline std::string stripHTML(std::string html) {
        static const std::regex tags("<[^>]*>");
        return std::regex_replace(html, tags, "");
    }
    
    inline std::vector<char> compress(std::string& input) {
        size_t const cBuffSize = ZSTD_compressBound(input.size());
        std::vector<char> compressedBuffer(cBuffSize);
        
        size_t const cSize = ZSTD_compress(compressedBuffer.data(), cBuffSize, input.c_str(), input.size(), 1);

        if (ZSTD_isError(cSize)) {
            std::cerr<<"Compression Error: "<<ZSTD_getErrorName(cSize)<<std::endl;
        }
        
        compressedBuffer.resize(cSize);
        
        return compressedBuffer;
    }

    inline std::string decompress(const std::vector<char>& compressedData) {
        if (compressedData.empty()) return "";
        
        unsigned long long const rSize = ZSTD_getFrameContentSize(compressedData.data(), compressedData.size());
        
        if (rSize == ZSTD_CONTENTSIZE_ERROR || rSize == ZSTD_CONTENTSIZE_UNKNOWN) {
            std::cerr<<"Error: Could not detemine original data size."<<std::endl;
            return "";
        }

        std::string decompressed(rSize, '\0');
        size_t const dSize = ZSTD_decompress(&decompressed[0], rSize, compressedData.data(), compressedData.size());

        if (ZSTD_isError(dSize)) {
            std::cerr<<"Decompression error: "<<ZSTD_getErrorName(dSize)<<std::endl;
            return "";
        }
    
        return decompressed;
}
    
    inline void saveBinary(const std::string& filename, const std::vector<char>& data) {
        std::ofstream ofs(filename, std::ios::binary | std::ios::app);
        if (ofs.is_open() && !data.empty()) {
            ofs.write(data.data(), data.size());
        }
    }
    
}


#endif
