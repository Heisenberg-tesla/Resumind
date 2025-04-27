#pragma once
#include <set>
#include <string>

namespace Skills {
    // Common technical skills
    const std::set<std::string> commonSkills = {
        // Programming Languages
        "c++", "python", "java", "javascript", "typescript", "c", "c#", "kotlin",
        // Web Technologies
        "html", "html5", "css", "css3", "react", "redux", "context api",
        "bootstrap", "tailwind css", "node.js", "express.js", "ejs", "passport.js",
        // Backend & Databases
        "sql", "sqlite", "database", "restful", "api", "microservices",
        "mongodb", "mongoose", "firebase", "cloudinary",
        // Version Control & DevOps
        "git", "github", "docker", "kubernetes", "ci/cd", "pipeline",
        // Cloud Platforms
        "aws", "azure", "gcp", "cloud", "blynk cloud",
        // Data Science & ML
        "machine learning", "deep learning", "tensorflow", "pytorch",
        "data analysis", "statistical analysis", "data visualization",
        "hadoop", "spark", "big data", "opencv", "mediapipe",
        // Game Development
        "unity", "godot", "sdl2", "qt", "c++17",
        // IoT & Embedded
        "arduino", "nodemcu", "fpga", "iot", "sensor",
        // General Development
        "algorithm", "data structure", "oop", "design pattern",
        "testing", "jest", "react testing library",
        "performance optimization", "responsive design",
        // Design Tools
        "figma", "photoshop", "premiere pro", "after effects", "lightroom",
        "canva", "capcut", "davinci resolve"
    };
    
    // Common soft skills
    const std::set<std::string> commonSoftSkills = {
        "communication", "teamwork", "leadership", "problem solving",
        "time management", "adaptability", "creativity", "critical thinking",
        "organization", "attention to detail", "analytical", "strategic",
        "collaboration", "documentation", "mentoring", "innovation",
        "user centered design", "clean code", "technical rigor"
    };
} 