#pragma once
#include <string>
#include <set>
#include <filesystem>
#include <map>

class Resume {
private:
    std::string content;
    std::string compressedContent;
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
    std::string getCompressedContent() const;
    std::set<std::string> getSkills() const;
    
    // Compression/Decompression methods
    void compress(bool print = true);
    // void decompress(const std::string& compressedData);
    void saveCompressed(const std::string& outputDir = "compressed_resumes");
    void loadCompressed(const std::string& filePath);
}; 