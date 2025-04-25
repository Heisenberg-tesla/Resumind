#include <iostream>
#include <filesystem>
#include "Resume.hpp"
#include "Huffman.hpp"
#include "JobDescription.hpp"

namespace fs = std::filesystem;

int main() {
    try {
        // List available job descriptions
        std::cout << "Available job descriptions:" << std::endl;
        int jobCount = 0;
        std::vector<std::string> jobFiles;
        
        for (const auto& entry : fs::directory_iterator("job_descriptions")) {
            if (entry.path().extension() == ".txt") {
                jobCount++;
                std::cout << jobCount << ". " << entry.path().filename() << std::endl;
                jobFiles.push_back(entry.path().string());
            }
        }
        
        if (jobCount == 0) {
            std::cout << "No job descriptions found in job_descriptions/ directory." << std::endl;
            return 1;
        }
        
        // Let user select a job description
        int choice;
        do {
            std::cout << "\nSelect a job description (1-" << jobCount << "): ";
            std::cin >> choice;
        } while (choice < 1 || choice > jobCount);
        
        // Load selected job description
        JobDescription jobDesc(jobFiles[choice - 1]);
        
        std::cout << "\nRequired Skills:" << std::endl;
        for (const auto& skill : jobDesc.getRequiredSkills()) {
            std::cout << "- " << skill << std::endl;
        }
        
        std::cout << "\nPreferred Skills:" << std::endl;
        for (const auto& skill : jobDesc.getPreferredSkills()) {
            std::cout << "- " << skill << std::endl;
        }
        
        std::cout << "\nProcessing resumes..." << std::endl;
        
        // Process all .txt files in the resumes directory
        for (const auto& entry : fs::directory_iterator("resumes")) {
            if (entry.path().extension() == ".txt") {
                std::cout << "\nProcessing: " << entry.path().filename() << std::endl;
                
                // Create and process resume
                Resume resume(entry.path().string());
                
                // Calculate match score
                double matchScore = jobDesc.calculateMatchScore(resume.getContent());
                std::cout << "Match Score: " << matchScore << std::endl;
                
                // Show keyword matches
                auto matches = jobDesc.findKeywordMatches(resume.getContent());
                if (!matches.empty()) {
                    std::cout << "Keyword Matches:" << std::endl;
                    for (const auto& match : matches) {
                        std::cout << "- " << match.first << " (" << match.second << " matches)" << std::endl;
                    }
                }
                
                std::cout << "-------------------" << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

