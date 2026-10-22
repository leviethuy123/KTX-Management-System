#include "manager/StaffManager.h"
#include <fstream>
#include <iostream>

StaffManager::StaffManager(const std::string& filePath)
    : Manager<Staff>(filePath) {}

void StaffManager::loadFromFile() {
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
            Staff s = Staff::fromJson(item);
            list.push_back(new Staff(s));
        }
        std::cout << "[Thanh cong] Da nap " << list.size() << " can bo tu " << dataFilePath << "\n";
    } catch (const std::exception& e) {
        std::cout << "[Loi] Parse JSON can bo that bai: " << e.what() << "\n";
    }
}

void StaffManager::saveToFile() const {
    std::ofstream file(dataFilePath);
    if (!file.is_open()) {
        std::cout << "[Loi] Khong the mo file de ghi: " << dataFilePath << "\n";
        return;
    }

    nlohmann::json jArr = nlohmann::json::array();
    for (const Staff* s : list) {
        if (s != nullptr) {
            jArr.push_back(s->toJson());
        }
    }

    file << jArr.dump(2);
    std::cout << "[Thanh cong] Da luu " << list.size() << " can bo vao " << dataFilePath << "\n";
}

std::vector<Staff*> StaffManager::findByDepartment(const std::string& dept) const {
    std::vector<Staff*> results;
    for (Staff* s : list) {
        if (s != nullptr && s->getDepartment() == dept) {
            results.push_back(s);
        }
    }
    return results;
}
