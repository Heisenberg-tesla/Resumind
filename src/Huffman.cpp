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

std::vector<bool> Huffman::compress(const std::string& text) {
    auto frequencies = calculateFrequencies(text);
    buildTree(frequencies);
    
    std::vector<bool> compressedBits;
    for (char ch : text) {
        const std::string& code = huffmanCodes[ch];
        for (char bit : code) {
            compressedBits.push_back(bit == '1');
        }
    }
    
    return compressedBits;
}

std::string Huffman::decompress(const std::vector<bool>& compressedBits, const std::map<char, int>& frequencies) {
    // Build the tree
    buildTree(frequencies);

    // Check if the tree is built
    if (!root) {
        throw std::runtime_error("Huffman tree not built");
    }
    
    std::string result;
    auto current = root;

    for (bool bit : compressedBits) {
        if (bit) {
            current = current->right;
        } else {
            current = current->left;
        }

        if (current->character != '\0') {
            result += current->character;
            current = root;
        }
    }

    return result;
}

void Huffman::printCodes() const {
    for (const auto& pair : huffmanCodes) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}