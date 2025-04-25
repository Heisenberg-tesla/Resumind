#include "Huffman.hpp"
#include <queue>
#include <bitset>
#include <iostream>

Huffman::Huffman() : root(nullptr) {}

std::map<char, int> Huffman::calculateFrequencies(const std::string& text) {
    std::map<char, int> frequencies;
    for (char ch : text) {
        frequencies[ch]++;
    }
    return frequencies;
}

void Huffman::buildTree(const std::map<char, int>& frequencies) {
    auto compare = [](const std::shared_ptr<HuffmanNode>& a, const std::shared_ptr<HuffmanNode>& b) {
        return a->frequency > b->frequency;
    };
    
    std::priority_queue<std::shared_ptr<HuffmanNode>, 
                       std::vector<std::shared_ptr<HuffmanNode>>, 
                       decltype(compare)> minHeap(compare);
    
    // Create leaf nodes
    for (const auto& pair : frequencies) {
        minHeap.push(std::make_shared<HuffmanNode>(pair.first, pair.second));
    }
    
    // Build the tree
    while (minHeap.size() > 1) {
        auto left = minHeap.top();
        minHeap.pop();
        auto right = minHeap.top();
        minHeap.pop();
        
        auto parent = std::make_shared<HuffmanNode>('\0', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        
        minHeap.push(parent);
    }
    
    root = minHeap.top();
    generateCodes(root, "");
}

void Huffman::generateCodes(const std::shared_ptr<HuffmanNode>& node, const std::string& code) {
    if (!node) return;
    
    if (node->character != '\0') {
        huffmanCodes[node->character] = code;
        return;
    }
    
    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

std::string Huffman::compress(const std::string& text) {
    auto frequencies = calculateFrequencies(text);
    buildTree(frequencies);
    
    std::string compressed;
    for (char ch : text) {
        compressed += huffmanCodes[ch];
    }
    
    return compressed;
}

std::string Huffman::decompress(const std::string& compressedText) {
    std::string decompressed;
    auto current = root;
    
    for (char bit : compressedText) {
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }
        
        if (current->character != '\0') {
            decompressed += current->character;
            current = root;
        }
    }
    
    return decompressed;
}

void Huffman::printCodes() const {
    for (const auto& pair : huffmanCodes) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
} 