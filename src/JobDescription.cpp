#include "JobDescription.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>

JobDescription::JobDescription() : content("") {}

JobDescription::JobDescription(const std::string& filePath) {
    loadFromFile(filePath);
}

void JobDescription::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    content = buffer.str();
    file.close();
    
    extractKeywords();
}

void JobDescription::loadFromText(const std::string& text) {
    content = text;
    extractKeywords();
}

void JobDescription::normalizeText(std::string& text) const{
    // Convert to lowercase
    std::transform(text.begin(), text.end(), text.begin(), ::tolower);
    
    // Remove punctuation
    text = std::regex_replace(text, std::regex("[^a-z0-9 ]"), " ");
    
    // Remove extra spaces  
    text = std::regex_replace(text, std::regex("\\s+"), " ");
    
    // Trim leading/trailing spaces
    text = std::regex_replace(text, std::regex("^\\s+|\\s+$"), "");
}

void JobDescription::extractKeywords() {
    // Common technical skills
    const std::set<std::string> commonSkills = {
        "c++", "python", "java", "javascript", "html", "css",
        "sql", "database", "git", "linux", "windows", "macos",
        "algorithm", "data structure", "oop", "design pattern",
        "machine learning", "ai", "deep learning", "neural network",
        "web development", "frontend", "backend", "full stack",
        "cloud", "aws", "azure", "gcp", "docker", "kubernetes"
    };
    
    // Common soft skills
    const std::set<std::string> commonSoftSkills = {
        "communication", "teamwork", "leadership", "problem solving",
        "time management", "adaptability", "creativity", "critical thinking",
        "organization", "attention to detail", "analytical", "strategic"
    };
    
    std::string normalizedContent = content;
    normalizeText(normalizedContent);
    
    std::stringstream ss(normalizedContent);
    std::string word;
    
    // Extract all words as potential keywords
    while (ss >> word) {
        keywords.insert(word);
    }
    
    // Identify required and preferred skills
    for (const auto& skill : commonSkills) {
        if (normalizedContent.find(skill) != std::string::npos) {
            // Check if it's marked as required (e.g., "must have", "required")
            if (normalizedContent.find("must have " + skill) != std::string::npos ||
                normalizedContent.find("required " + skill) != std::string::npos) {
                requiredSkills.insert(skill);
            } else {
                preferredSkills.insert(skill);
            }
        }
    }
    
    // Add soft skills
    for (const auto& skill : commonSoftSkills) {
        if (normalizedContent.find(skill) != std::string::npos) {
            preferredSkills.insert(skill);
        }
    }
}

const std::set<std::string>& JobDescription::getKeywords() const {
    return keywords;
}

const std::set<std::string>& JobDescription::getRequiredSkills() const {
    return requiredSkills;
}

const std::set<std::string>& JobDescription::getPreferredSkills() const {
    return preferredSkills;
}

std::map<std::string, int> JobDescription::findKeywordMatches(const std::string& text) const {
    std::map<std::string, int> matches;
    std::string normalizedText = text;
    normalizeText(normalizedText);
    
    for (const auto& keyword : keywords) {
        size_t pos = 0;
        while ((pos = normalizedText.find(keyword, pos)) != std::string::npos) {
            matches[keyword]++;
            pos += keyword.length();
        }
    }
    
    return matches;
}

double JobDescription::calculateMatchScore(const std::string& text) const {
    auto matches = findKeywordMatches(text);
    double score = 0.0;
    
    // Calculate base score from keyword matches
    for (const auto& match : matches) {
        score += match.second; // Each match contributes to the score
    }
    
    // Bonus for required skills
    for (const auto& skill : requiredSkills) {
        if (text.find(skill) != std::string::npos) {
            score += 5.0; // Higher weight for required skills
        }
    }
    
    // Bonus for preferred skills
    for (const auto& skill : preferredSkills) {
        if (text.find(skill) != std::string::npos) {
            score += 2.0; // Lower weight for preferred skills
        }
    }
    
    return score;
}

std::string JobDescription::getContent() const {
    return content;
} 