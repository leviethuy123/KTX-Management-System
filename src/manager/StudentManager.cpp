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
        std::cout << "[Thông báo] Không thể mở file: " << dataFilePath << ". Bắt đầu với danh sách trống.\n";
        return;
    }

    try {
        nlohmann::json jArr;
        file >> jArr;
        for (const auto& item : jArr) {
            Student s = Student::fromJson(item);
            list.push_back(new Student(s));
        }
        std::cout << "[Thành công] Đã nạp " << list.size() << " sinh viên từ " << dataFilePath << "\n";
    } catch (const std::exception& e) {
        std::cout << "[Lỗi] Parse JSON sinh viên thất bại: " << e.what() << "\n";
    }
}

void StudentManager::saveToFile() const {
    std::ofstream file(dataFilePath);
    if (!file.is_open()) {
        std::cout << "[Lỗi] Không thể mở file để ghi: " << dataFilePath << "\n";
        return;
    }

    nlohmann::json jArr = nlohmann::json::array();
    for (const Student* s : list) {
        if (s != nullptr) {
            jArr.push_back(s->toJson());
        }
    }

    file << jArr.dump(2);
    std::cout << "[Thành công] Đã lưu " << list.size() << " sinh viên vào " << dataFilePath << "\n";
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
