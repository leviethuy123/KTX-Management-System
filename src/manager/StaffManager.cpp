#include "manager/StaffManager.h"
#include <fstream>
#include <iostream>

StaffManager::StaffManager(const std::string& filePath)
    : Manager<Staff>(filePath) {}

void StaffManager::loadFromFile() {
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
            Staff st = Staff::fromJson(item);
            list.push_back(new Staff(st));
        }
        std::cout << "[Thành công] Đã nạp " << list.size() << " cán bộ từ " << dataFilePath << "\n";
    } catch (const std::exception& e) {
        std::cout << "[Lỗi] Parse JSON cán bộ thất bại: " << e.what() << "\n";
    }
}

void StaffManager::saveToFile() const {
    std::ofstream file(dataFilePath);
    if (!file.is_open()) {
        std::cout << "[Lỗi] Không thể mở file để ghi: " << dataFilePath << "\n";
        return;
    }

    nlohmann::json jArr = nlohmann::json::array();
    for (const Staff* st : list) {
        if (st != nullptr) {
            jArr.push_back(st->toJson());
        }
    }

    file << jArr.dump(2);
    std::cout << "[Thành công] Đã lưu " << list.size() << " cán bộ vào " << dataFilePath << "\n";
}

std::vector<Staff*> StaffManager::findByDepartment(const std::string& dept) const {
    std::vector<Staff*> results;
    for (Staff* st : list) {
        if (st != nullptr && st->getDepartment() == dept) {
            results.push_back(st);
        }
    }
    return results;
}
