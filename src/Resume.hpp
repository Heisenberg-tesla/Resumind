#pragma once
#include <string>
#include <set>

class Resume {
private:
    std::string content;
    std::string compressedContent;
    std::set<std::string> skills;
    
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
    void decompress(const std::string& compressedData);
}; 