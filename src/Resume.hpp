#pragma once
#include <string>
#include <vector>
#include <map>

class Resume {
private:
    std::string content;
    std::map<std::string, int> wordFrequency;
    std::string compressedContent;
    
public:
    Resume();
    explicit Resume(const std::string& filePath);
    
    // Text processing methods
    void loadFromFile(const std::string& filePath);
    void analyzeContent();
    std::map<std::string, int> getWordFrequency() const;
    
    // Getters
    std::string getContent() const;
    std::string getCompressedContent() const;
    
    // Compression/Decompression methods
    void compress(bool print = true);
    void decompress(const std::string& compressedData);
}; 