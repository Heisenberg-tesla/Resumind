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

std::string Resume::getCompressedContent() const {
    return compressedContent;
}

std::set<std::string> Resume::getSkills() const {
    return skills;
}

void Resume::compress(bool print) {
    Huffman huffman;
    frequencyTable = huffman.calculateFrequencies(content);  // Store frequency table
    originalLength = content.size();                         // Store original length
    compressedContent = huffman.compress(content);
    if (print) {
        std::cout << "Original size: " << content.size() << " bytes" << std::endl;
        std::cout << "Compressed size: " << (compressedContent.size() + 7) / 8 << " bytes" << std::endl;
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
    if (compressedContent.empty()) {
        throw std::runtime_error("No compressed content available. Call compress() first.");
    }

    // Ensure output directory exists
    ensureDirectoryExists(outputDir);
    
    // Create path for compressed file
    std::string compressedPath = outputDir + "/" + filename + ".compressed";
    
    // Save the compressed content and metadata
    std::ofstream outFile(compressedPath);
    if (!outFile.is_open()) {
        throw std::runtime_error("Could not create compressed file: " + compressedPath);
    }
    
    // Save metadata first
    outFile << originalLength << "\n";  // Save original length
    // Save frequency table
    for (const auto& pair : frequencyTable) {
        outFile << static_cast<int>(pair.first) << " " << pair.second << "\n";
    }
    outFile << "---\n";  // Separator between metadata and compressed content
    outFile << compressedContent;
    outFile.close();
    
    std::cout << "Compressed resume saved to: " << compressedPath << std::endl;
}

void Resume::loadCompressed(const std::string& filePath) {
    filename = std::filesystem::path(filePath).filename().string();
    std::cout << "Loading compressed resume from: " << filePath << std::endl;

    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        throw std::runtime_error("Could not open compressed file: " + filePath);
    }
    
    // Read metadata
    inFile >> originalLength;
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

    // Convert frequency table to huffman tree
    Huffman huffman;

    // Read compressed content
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    compressedContent = buffer.str();

    // Decompress the content
    content = huffman.decompress(compressedContent, frequencyTable);

    std::cout << "Decompressed content: " << content.size() << std::endl;
    
    // Verify decompression
    if (content.size() != originalLength) {
        throw std::runtime_error("Decompression failed: size mismatch");
    }
    
    std::cout << "Decompression successful" << std::endl;

    // Re-analyze content for skills
    analyzeContent();
} 