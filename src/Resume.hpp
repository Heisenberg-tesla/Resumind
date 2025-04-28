#pragma once
#include <string>
#include <set>
#include <filesystem>
#include <map>
#include <vector>

class Resume {
private:
    std::string content;
    std::vector<bool> compressedBits;
    std::set<std::string> skills;
    std::map<char, int> frequencyTable;  // Store frequency table for decompression
    size_t originalLength;               // Store original content length
    std::string filename;
    
    void ensureDirectoryExists(const std::string& dirPath) const;
    
public:
    Resume();
    explicit Resume(const std::string& filePath);
    
    // Text processing methods
    void loadFromFile(const std::string& filePath);
    void analyzeContent();
    
    // Getters
    std::string getContent() const;
    std::vector<bool> getCompressedBits() const;
    std::set<std::string> getSkills() const;
    
    // Compression/Decompression methods
    void compress(bool print = true);
    void saveCompressed(const std::string& outputDir = "compressed_resumes");
    void loadCompressed(const std::string& filePath);
}; 