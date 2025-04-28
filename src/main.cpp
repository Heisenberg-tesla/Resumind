#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include "Resume.hpp"
#include "Huffman.hpp"
#include "JobDescription.hpp"
#include <iomanip>

namespace fs = std::filesystem;

// Helper functions
std::vector<std::string> getJobDescriptionFiles() {
    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator("job_descriptions")) {
        if (entry.path().extension() == ".txt") {
            files.push_back(entry.path().string());
        }
    }
    return files;
}

std::vector<std::string> getResumeFiles() {
    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator("resumes")) {
        if (entry.path().extension() == ".txt") {
            files.push_back(entry.path().string());
        }
    }
    return files;
}

std::vector<std::string> getCompressedResumeFiles() {
    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator("compressed_resumes")) {
        if (entry.path().extension() == ".compressed") {
            files.push_back(entry.path().string());
        }
    }
    return files;
}

// Feature functions
void listJobDescriptions() {
    auto jobFiles = getJobDescriptionFiles();
    if (jobFiles.empty()) {
        std::cout << "No job descriptions found in job_descriptions/ directory." << std::endl;
        return;
    }

    std::cout << "\nAvailable Job Descriptions:" << std::endl;
    for (size_t i = 0; i < jobFiles.size(); ++i) {
        std::cout << i + 1 << ". " << fs::path(jobFiles[i]).filename() << std::endl;
    }
}

void showJobSkills(const std::string& jobFilePath) {
    try {
        JobDescription jobDesc(jobFilePath);
        std::cout << "\nRequired Skills:" << std::endl;
        for (const auto& skill : jobDesc.getRequiredSkills()) {
            std::cout << "- " << skill << std::endl;
        }
        
        std::cout << "\nPreferred Skills:" << std::endl;
        for (const auto& skill : jobDesc.getPreferredSkills()) {
            std::cout << "- " << skill << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void listResumes() {
    auto resumeFiles = getResumeFiles();
    if (resumeFiles.empty()) {
        std::cout << "No resumes found in resumes/ directory." << std::endl;
        return;
    }

    std::cout << "\nAvailable Resumes:" << std::endl;
    for (size_t i = 0; i < resumeFiles.size(); ++i) {
        std::cout << i + 1 << ". " << fs::path(resumeFiles[i]).filename() << std::endl;
    }
}

void processResumesForJob(const std::string& jobFilePath) {
    try {
        JobDescription jobDesc(jobFilePath);
        auto resumeFiles = getResumeFiles();
        
        std::cout << "\nProcessing resumes..." << std::endl;
        for (const auto& resumeFile : resumeFiles) {
            std::cout << "\nProcessing: " << fs::path(resumeFile).filename() << std::endl;
            Resume resume(resumeFile);
            double resumeMatchScore = jobDesc.calculateResumeMatchScore(resume);
            std::cout << "Resume Match Score: " << std::fixed << std::setprecision(2) << resumeMatchScore << "%" << std::endl;
            std::cout << "-------------------" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void compressAndStoreResume(const std::string& resumeFilePath) {
    try {
        Resume resume(resumeFilePath);
        resume.compress(true);  // Compress with printing enabled
        resume.saveCompressed();  // Save to default compressed_resumes directory
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void listCompressedResumes() {
    auto compressedFiles = getCompressedResumeFiles();
    if (compressedFiles.empty()) {
        std::cout << "No compressed resumes found in compressed_resumes/ directory." << std::endl;
        return;
    }

    std::cout << "\nAvailable Compressed Resumes:" << std::endl;
    for (size_t i = 0; i < compressedFiles.size(); ++i) {
        std::cout << i + 1 << ". " << fs::path(compressedFiles[i]).filename() << std::endl;
    }
}

void viewCompressedResume(const std::string& filePath) {
    try {
        Resume resume;
        resume.loadCompressed(filePath);
        
        std::cout << "\nResume Details:" << std::endl;
        std::cout << "Filename: " << fs::path(filePath).filename() << std::endl;
        std::cout << "Original Size: " << resume.getContent().size() << " bytes" << std::endl;
        std::cout << "Compressed Size: " << (resume.getCompressedBits().size() + 7) / 8 << " bytes" << std::endl;
        std::cout << "Compression Ratio: " << std::fixed << std::setprecision(2) 
                  << (static_cast<double>((resume.getCompressedBits().size() + 7) / 8) / resume.getContent().size() * 100) << "%" << std::endl;
    

        // Print the skills
        std::cout << "\nSkills:" << std::endl;
        for (const auto& skill : resume.getSkills()) {
            std::cout << "- " << skill << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void displayMenu() {
    std::cout << "\nResume Scanner Menu:" << std::endl;
    std::cout << "1. List Job Descriptions" << std::endl;
    std::cout << "2. Show Skills for a Job Description" << std::endl;
    std::cout << "3. List Available Resumes" << std::endl;
    std::cout << "4. Process Resumes for a Job Description" << std::endl;
    std::cout << "5. Compress and Store a Resume" << std::endl;
    std::cout << "6. View Compressed Resumes" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

int main() {
    try {
        int choice;
        do {
            displayMenu();
            std::cin >> choice;
            
            switch (choice) {
                case 1: {
                    listJobDescriptions();
                    break;
                }
                case 2: {
                    auto jobFiles = getJobDescriptionFiles();
                    if (jobFiles.empty()) {
                        std::cout << "No job descriptions available." << std::endl;
                        break;
                    }
                    listJobDescriptions();
                    size_t jobChoice;
                    std::cout << "Select a job description (1-" << jobFiles.size() << "): ";
                    std::cin >> jobChoice;
                    if (jobChoice >= 1 && jobChoice <= jobFiles.size()) {
                        showJobSkills(jobFiles[jobChoice - 1]);
                    }
                    break;
                }
                case 3: {
                    listResumes();
                    break;
                }
                case 4: {
                    auto jobFiles = getJobDescriptionFiles();
                    if (jobFiles.empty()) {
                        std::cout << "No job descriptions available." << std::endl;
                        break;
                    }
                    listJobDescriptions();
                    size_t jobChoice;
                    std::cout << "Select a job description (1-" << jobFiles.size() << "): ";
                    std::cin >> jobChoice;
                    if (jobChoice >= 1 && jobChoice <= jobFiles.size()) {
                        processResumesForJob(jobFiles[jobChoice - 1]);
                    }
                    break;
                }
                case 5: {
                    auto resumeFiles = getResumeFiles();
                    if (resumeFiles.empty()) {
                        std::cout << "No resumes available." << std::endl;
                        break;
                    }
                    listResumes();
                    size_t resumeChoice;
                    std::cout << "Select a resume to compress (1-" << resumeFiles.size() << "): ";
                    std::cin >> resumeChoice;
                    if (resumeChoice >= 1 && resumeChoice <= resumeFiles.size()) {
                        compressAndStoreResume(resumeFiles[resumeChoice - 1]);
                    }
                    break;
                }
                case 6: {
                    auto compressedFiles = getCompressedResumeFiles();
                    if (compressedFiles.empty()) {
                        std::cout << "No compressed resumes available." << std::endl;
                        break;
                    }
                    listCompressedResumes();
                    int resumeChoice;
                    std::cout << "Select a compressed resume to view (1-" << compressedFiles.size() << "): ";
                    std::cin >> resumeChoice;
                    if (resumeChoice >= 1 && resumeChoice <= static_cast<int>(compressedFiles.size())) {
                        viewCompressedResume(compressedFiles[resumeChoice - 1]);
                    }
                    break;
                }
                case 0: {
                    std::cout << "Exiting program..." << std::endl;
                    break;
                }
                default: {
                    std::cout << "Invalid choice. Please try again." << std::endl;
                }
            }
        } while (choice != 0);
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

