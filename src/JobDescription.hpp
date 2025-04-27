#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include "Resume.hpp"

class JobDescription {
private:
    std::string content;
    std::set<std::string> requiredSkills;
    std::set<std::string> preferredSkills;
    std::set<std::string> allWords;
    
    void extractKeywords();
    void normalizeText(std::string& text) const;
    
public:
    JobDescription();
    explicit JobDescription(const std::string& filePath);
    
    // File handling
    void loadFromFile(const std::string& filePath);
    void loadFromText(const std::string& text);
    
    // Keyword analysis
    const std::set<std::string>& getAllWords() const;
    const std::set<std::string>& getRequiredSkills() const;
    const std::set<std::string>& getPreferredSkills() const;
    
    // Matching
    std::set<std::string> findKeywordMatches(const std::string& text) const;
    double calculateMatchScore(const std::string& text) const;
    double calculateResumeMatchScore(const Resume& resume) const;
    
    // Getters
    std::string getContent() const;
}; 