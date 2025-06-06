#pragma once
#include <string>
#include <map>
#include <queue>
#include <memory>
#include <vector>

struct HuffmanNode {
    char character;
    int frequency;
    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;
    
    HuffmanNode(char ch, int freq) 
        : character(ch), frequency(freq), left(nullptr), right(nullptr) {}
};

class Huffman {
private:
    std::shared_ptr<HuffmanNode> root;
    std::map<char, std::string> huffmanCodes;
    
    void buildTree(const std::map<char, int>& frequencies);
    void generateCodes(const std::shared_ptr<HuffmanNode>& node, const std::string& code);
    
public:
    Huffman();
    
    // Compression methods
    std::vector<bool> compress(const std::string& text);
    std::string decompress(const std::vector<bool>& compressedBits, const std::map<char, int>& frequencies);

    // Helper methods
    std::map<char, int> calculateFrequencies(const std::string& text);
    void printCodes() const;
}; 