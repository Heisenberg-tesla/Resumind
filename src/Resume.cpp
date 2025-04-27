#include "Resume.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Huffman.hpp"
#include <iostream>
#include "../constants/skills.hpp"

Resume::Resume() : content("") {}

Resume::Resume(const std::string& filePath) {
    loadFromFile(filePath);
}

void Resume::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    content = buffer.str();
    file.close();
    
    analyzeContent();
}

void Resume::analyzeContent() {
    skills.clear();
    std::string normalizedContent = content;
    // Convert to lowercase for consistent matching
    std::transform(normalizedContent.begin(), normalizedContent.end(), normalizedContent.begin(), ::tolower);
    
    // Check for technical skills
    for (const auto& skill : Skills::commonSkills) {
        if (normalizedContent.find(skill) != std::string::npos) {
            skills.insert(skill);
        }
    }
    
    // Check for soft skills
    for (const auto& skill : Skills::commonSoftSkills) {
        if (normalizedContent.find(skill) != std::string::npos) {
            skills.insert(skill);
        }
    }
}

std::string Resume::getContent() const {
    return content;
}

std::string Resume::getCompressedContent() const {
    return compressedContent;
}

std::set<std::string> Resume::getSkills() const {
    return skills;
}

void Resume::compress(bool print) {
    Huffman huffman;
    compressedContent = huffman.compress(content);
    if (print) {
        std::cout << "Original size: " << content.size() << " bytes" << std::endl;
        std::cout << "Compressed size: " << (compressedContent.size() + 7) / 8 << " bytes" << std::endl;
        std::cout << "-------------------" << std::endl;
    }
}

void Resume::decompress(const std::string& compressedData) {
    Huffman huffman;
    content = huffman.decompress(compressedData);
} 