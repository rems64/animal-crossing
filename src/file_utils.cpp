#include "file_utils.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

std::string read_file(const std::string &filepath) {
    std::ifstream file(filepath, std::ios::in | std::ios::binary);

    if (!file) {
        std::cerr << "Could not open the file" << std::endl;
        std::cerr << "\tfilepath : " << filepath << std::endl;
        throw std::runtime_error("failed to open file");
    }

    file.seekg(0, std::ios::end);
    std::string content(file.tellg(), '\0');
    file.seekg(0, std::ios::beg);

    file.read(&content[0], content.size());

    return content;
}
