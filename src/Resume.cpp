#include "Resume.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Huffman.hpp"
#include <iostream>
#include "../constants/skills.hpp"
#include <filesystem>

Resume::Resume() : content(""), filename("") {}

Resume::Resume(const std::string& filePath) {
    loadFromFile(filePath);
    filename = std::filesystem::path(filePath).filename().string();
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

std::vector<bool> Resume::getCompressedBits() const {
    return compressedBits;
}

std::set<std::string> Resume::getSkills() const {
    return skills;
}

void Resume::compress(bool print) {
    Huffman huffman;
    frequencyTable = huffman.calculateFrequencies(content);  // Store frequency table
    originalLength = content.size();                         // Store original length
    compressedBits = huffman.compress(content);

    if (print) {
        std::cout << "Original size: " << content.size() << " bytes" << std::endl;
        std::cout << "Compressed size: " << (compressedBits.size() + 7) / 8 << " bytes" << std::endl;
        std::cout << "-------------------" << std::endl;
    }
}

// void Resume::decompress(const std::string& compressedData) {
//     Huffman huffman;
//     content = huffman.decompress(compressedData, frequencyTable);
// }

void Resume::ensureDirectoryExists(const std::string& dirPath) const {
    if (!std::filesystem::exists(dirPath)) {
        std::filesystem::create_directory(dirPath);
    }
}

void Resume::saveCompressed(const std::string& outputDir) {
    if (compressedBits.empty()) {
        throw std::runtime_error("No compressed content available. Call compress() first.");
    }

    // Ensure output directory exists
    ensureDirectoryExists(outputDir);
    
    // Create path for compressed file
    std::string compressedPath = outputDir + "/" + filename + ".compressed";
    
    // Save the compressed content and metadata
    std::ofstream outFile(compressedPath, std::ios::binary);
    if (!outFile.is_open()) {
        throw std::runtime_error("Could not create compressed file: " + compressedPath);
    }
    
    // Save metadata first
    outFile << originalLength << "\n";  // Save original length
    outFile << compressedBits.size() << "\n"; // Save number of bits
    
    // Save frequency table
    for (const auto& pair : frequencyTable) {
        outFile << static_cast<int>(pair.first) << " " << pair.second << "\n";
    }
    outFile << "---\n";  // Separator between metadata and compressed content
    
    // Pack bits into bytes and save
    uint8_t byte = 0;
    int bitCount = 0;

    for (bool bit : compressedBits) {
        byte = (byte << 1) | (bit ? 1 : 0);
        bitCount++;
        if (bitCount == 8) {
            outFile.write(reinterpret_cast<const char*>(&byte), 1);
            byte = 0;
            bitCount = 0;
        }
    }
    
    // Write any remaining bits
    if (bitCount > 0) {
        byte <<= (8 - bitCount);  // Left shift remaining bits to fill the byte
        outFile.write(reinterpret_cast<const char*>(&byte), 1);
    }
    
    outFile.close();
    
    std::cout << "Compressed resume saved to: " << compressedPath << std::endl;
}

void Resume::loadCompressed(const std::string& filePath) {
    filename = std::filesystem::path(filePath).filename().string();
    std::cout << "Loading compressed resume from: " << filePath << std::endl;

    std::ifstream inFile(filePath, std::ios::binary);
    if (!inFile.is_open()) {
        throw std::runtime_error("Could not open compressed file: " + filePath);
    }
    
    // Read metadata
    inFile >> originalLength;
    inFile.ignore();  // Skip newline
    
    size_t numBits;
    inFile >> numBits;
    inFile.ignore();  // Skip newline
    
    // Read frequency table
    frequencyTable.clear();
    std::string line;
    while (std::getline(inFile, line) && line != "---") {
        std::istringstream iss(line);
        int charCode, frequency;
        iss >> charCode >> frequency;
        frequencyTable[static_cast<char>(charCode)] = frequency;
    }
    
    // Read compressed bits
    compressedBits.clear();
    compressedBits.reserve(numBits);
    
    uint8_t byte;
    size_t bitsRead = 0;
    while (inFile.read(reinterpret_cast<char*>(&byte), 1) && bitsRead < numBits) {
        for (int i = 7; i >= 0 && bitsRead < numBits; --i) {
            compressedBits.push_back((byte >> i) & 1);
            bitsRead++;
        }
    }
    
    // Decompress the content
    Huffman huffman;
    content = huffman.decompress(compressedBits, frequencyTable);
    
    // Verify decompression
    if (content.size() != originalLength) {
        throw std::runtime_error("Decompression failed: size mismatch");
    }
    
    // Re-analyze content for skills
    analyzeContent();
} 