#include "manager/StudentManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>

StudentManager::StudentManager(const std::string& filePath)
    : Manager<Student>(filePath) {}

void StudentManager::loadFromFile() {
    clear();
    std::ifstream file(dataFilePath);
    if (!file.is_open()) {
        std::cout << "[Thong bao] Khong the mo file: " << dataFilePath << ". Bat dau voi danh sach trong.\n";
        return;
    }

    try {
        nlohmann::json jArr;
        file >> jArr;
        for (const auto& item : jArr) {
            Student s = Student::fromJson(item);
            list.push_back(new Student(s));
        }
        std::cout << "[Thanh cong] Da nap " << list.size() << " sinh vien tu " << dataFilePath << "\n";
    } catch (const std::exception& e) {
        std::cout << "[Loi] Parse JSON sinh vien that bai: " << e.what() << "\n";
    }
}

void StudentManager::saveToFile() const {
    std::ofstream file(dataFilePath);
    if (!file.is_open()) {
        std::cout << "[Loi] Khong the mo file de ghi: " << dataFilePath << "\n";
        return;
    }

    nlohmann::json jArr = nlohmann::json::array();
    for (const Student* s : list) {
        if (s != nullptr) {
            jArr.push_back(s->toJson());
        }
    }

    file << jArr.dump(2);
    std::cout << "[Thanh cong] Da luu " << list.size() << " sinh vien vao " << dataFilePath << "\n";
}

std::vector<Student*> StudentManager::findByClass(const std::string& className) const {
    std::vector<Student*> results;
    for (Student* s : list) {
        if (s != nullptr && s->getClass() == className) {
            results.push_back(s);
        }
    }
    return results;
}

std::vector<Student*> StudentManager::findByFaculty(const std::string& faculty) const {
    std::vector<Student*> results;
    for (Student* s : list) {
        if (s != nullptr && s->getFaculty() == faculty) {
            results.push_back(s);
        }
    }
    return results;
}

std::vector<Student*> StudentManager::searchByName(const std::string& nameKeyword) const {
    std::vector<Student*> results;
    std::string lowerKeyword = nameKeyword;
    std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);

    for (Student* s : list) {
        if (s != nullptr) {
            std::string name = s->getFullName();
            std::string lowerName = name;
            std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
            if (lowerName.find(lowerKeyword) != std::string::npos) {
                results.push_back(s);
            }
        }
    }
    return results;
}
