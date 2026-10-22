#include "manager/RoomManager.h"
#include <fstream>
#include <iostream>

RoomManager::RoomManager(const std::string& filePath)
    : Manager<Room>(filePath) {}

void RoomManager::loadFromFile() {
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
            std::string type = item.value("roomType", "STANDARD");
            if (type == "VIP") {
                list.push_back(new VipRoom(VipRoom::fromJson(item)));
            } else {
                list.push_back(new StandardRoom(StandardRoom::fromJson(item)));
            }
        }
        std::cout << "[Thanh cong] Da nap " << list.size() << " phong tu " << dataFilePath << "\n";
    } catch (const std::exception& e) {
        std::cout << "[Loi] Parse JSON phong that bai: " << e.what() << "\n";
    }
}

void RoomManager::saveToFile() const {
    std::ofstream file(dataFilePath);
    if (!file.is_open()) {
        std::cout << "[Loi] Khong the mo file de ghi: " << dataFilePath << "\n";
        return;
    }

    nlohmann::json jArr = nlohmann::json::array();
    for (const Room* r : list) {
        if (r != nullptr) {
            jArr.push_back(r->toJson());
        }
    }

    file << jArr.dump(2);
    std::cout << "[Thanh cong] Da luu " << list.size() << " phong vao " << dataFilePath << "\n";
}

std::vector<Room*> RoomManager::findAvailableRoom() const {
    std::vector<Room*> results;
    for (Room* r : list) {
        if (r != nullptr && !r->isFull() && r->getStatus() != RoomStatus::MAINTENANCE) {
            results.push_back(r);
        }
    }
    return results;
}

void RoomManager::printAvailableRoom() const {
    std::vector<Room*> available = findAvailableRoom();
    if (available.empty()) {
        std::cout << "(Khong con phong nao trong cho)\n";
        return;
    }
    std::cout << "=== DANH SACH PHONG CON CHO TRONG (" << available.size() << " phong) ===\n";
    for (const Room* r : available) {
        r->print();
    }
}

std::vector<Room*> RoomManager::findByBuilding(const std::string& building) const {
    std::vector<Room*> results;
    for (Room* r : list) {
        if (r != nullptr && r->getBuilding() == building) {
            results.push_back(r);
        }
    }
    return results;
}

std::vector<Room*> RoomManager::findByType(const std::string& roomType) const {
    std::vector<Room*> results;
    for (Room* r : list) {
        if (r != nullptr && r->getRoomType() == roomType) {
            results.push_back(r);
        }
    }
    return results;
}
