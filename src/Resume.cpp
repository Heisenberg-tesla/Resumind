#include "Resume.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Huffman.hpp"
#include <iostream>

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
    wordFrequency.clear();
    std::stringstream ss(content);
    std::string word;
    
    while (ss >> word) {
        // Convert to lowercase for consistent counting
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        wordFrequency[word]++;
    }
}

std::map<std::string, int> Resume::getWordFrequency() const {
    return wordFrequency;
}

std::string Resume::getContent() const {
    return content;
}

std::string Resume::getCompressedContent() const {
    return compressedContent;
}

// These will be implemented after we create the Huffman class
void Resume::compress(bool print) {
    // TODO: Implement Huffman compression
    Huffman huffman;
    compressedContent = huffman.compress(content);
    if (print) {
        std::cout << "Original size: " << content.size() << " bytes" << std::endl;
        std::cout << "Compressed size: " << (compressedContent.size() + 7) / 8 << " bytes" << std::endl;
        std::cout << "-------------------" << std::endl;
    }
}



void Resume::decompress(const std::string& compressedData) {
    // TODO: Implement Huffman decompression
} 