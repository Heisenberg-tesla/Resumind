#include "JobDescription.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>
#include <iostream>
#include "Resume.hpp"
#include "../constants/skills.hpp"

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
    // Directly assign the text to the content member
    content = text;
    extractKeywords();
}

void JobDescription::normalizeText(std::string& text) const{
    // Used to remove all the extra spaces and punctuation from the text

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
    std::string normalizedContent = content;
    normalizeText(normalizedContent);
    
    std::stringstream ss(normalizedContent);
    std::string word;
    
    // Extract all words as potential keywords
    while (ss >> word) {
        allWords.insert(word);
    }
    
    // Find section positions once
    size_t requiredSection = normalizedContent.find("required skills");
    size_t preferredSection = normalizedContent.find("preferred skills");
    std::string requiredSectionText;
    
    if (requiredSection != std::string::npos && preferredSection != std::string::npos) {
        // Get the text between "required skills:" and "preferred skills:"
        requiredSectionText = normalizedContent.substr(requiredSection, preferredSection - requiredSection);
    }
    
    // Identify required and preferred skills
    for (const auto& skill : Skills::commonSkills) {
        if (normalizedContent.find(skill) != std::string::npos) {
            if (!requiredSectionText.empty() && requiredSectionText.find(skill) != std::string::npos) {
                requiredSkills.insert(skill);
            } else {
                preferredSkills.insert(skill);
            }
        }
    }
    
    // Add soft skills
    for (const auto& skill : Skills::commonSoftSkills) {
        if (normalizedContent.find(skill) != std::string::npos) {
            preferredSkills.insert(skill);
        }
    }
}

const std::set<std::string>& JobDescription::getAllWords() const {
    return allWords;
}

const std::set<std::string>& JobDescription::getRequiredSkills() const {
    return requiredSkills;
}

const std::set<std::string>& JobDescription::getPreferredSkills() const {
    return preferredSkills;
}

std::set<std::string> JobDescription::findKeywordMatches(const std::string& text) const {
    // Used to find which skills appear in the text
    std::set<std::string> matches;
    std::string normalizedText = text;
    normalizeText(normalizedText);
    
    // Check matches in required skills
    for (const auto& skill : requiredSkills) {
        if (normalizedText.find(skill) != std::string::npos) {
            matches.insert(skill);
        }
    }
    
    // Check matches in preferred skills
    for (const auto& skill : preferredSkills) {
        if (normalizedText.find(skill) != std::string::npos) {
            matches.insert(skill);
        }
    }
    
    return matches;
}

double JobDescription::calculateMatchScore(const std::string& text) const {
    auto matches = findKeywordMatches(text);
    double score = 0.0;
    
    // Calculate base score from matches
    for (const auto& skill : matches) {
        // Check if it's a required skill
        if (requiredSkills.find(skill) != requiredSkills.end()) {
            score += 5.0; // Higher weight for required skills
        } else {
            score += 2.0; // Lower weight for preferred skills
        }
    }
    
    // Normalize the score based on the total possible points
    double maxPossibleScore = (requiredSkills.size() * 5.0) + (preferredSkills.size() * 2.0);
    if (maxPossibleScore > 0) {
        score = (score / maxPossibleScore) * 100.0; // Convert to percentage
    }
    
    return score;
}

std::string JobDescription::getContent() const {
    return content;
}

double JobDescription::calculateResumeMatchScore(const Resume& resume) const {
    const std::set<std::string>& resumeSkills = resume.getSkills();
    double score = 0.0;
    
    // Calculate base score from matches
    for (const auto& skill : resumeSkills) {
        // Check if it's a required skill
        if (requiredSkills.find(skill) != requiredSkills.end()) {
            score += 5.0; // Higher weight for required skills
        }
        // Check if it's a preferred skill
        else if (preferredSkills.find(skill) != preferredSkills.end()) {
            score += 2.0; // Lower weight for preferred skills
        }
    }
    
    // Normalize the score based on the total possible points
    double maxPossibleScore = (requiredSkills.size() * 5.0) + (preferredSkills.size() * 2.0);
    if (maxPossibleScore > 0) {
        score = (score / maxPossibleScore) * 100.0; // Convert to percentage
    }
    
    return score;
}