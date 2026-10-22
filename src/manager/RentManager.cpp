#include "manager/RentManager.h"
#include <fstream>
#include <iostream>

RentManager::RentManager(const std::string& filePath)
    : Manager<Rent>(filePath) {}

void RentManager::loadFromFile() {
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
            Rent r = Rent::fromJson(item, nullptr, nullptr);
            list.push_back(new Rent(r));
        }
        std::cout << "[Thanh cong] Da nap " << list.size() << " hop dong tu " << dataFilePath << "\n";
    } catch (const std::exception& e) {
        std::cout << "[Loi] Parse JSON hop dong that bai: " << e.what() << "\n";
    }
}

void RentManager::loadFromFile(StudentManager& sm, RoomManager& rm) {
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
            std::string sId = item.value("studentId", "");
            std::string rId = item.value("roomId", "");

            Student* student = sm.findById(sId);
            Room* room = rm.findById(rId);

            Rent r = Rent::fromJson(item, student, room);
            Rent* newRent = new Rent(r);
            list.push_back(newRent);

            // Đồng bộ trạng thái vào Student và Room nếu hợp đồng còn hiệu lực
            if (newRent->isActive()) {
                if (student != nullptr) {
                    student->setRent(newRent);
                }
                if (room != nullptr) {
                    room->addRent(newRent);
                }
            }
        }
        std::cout << "[Thanh cong] Da nap va dong bo " << list.size() << " hop dong tu " << dataFilePath << "\n";
    } catch (const std::exception& e) {
        std::cout << "[Loi] Parse JSON hop dong that bai: " << e.what() << "\n";
    }
}

void RentManager::saveToFile() const {
    std::ofstream file(dataFilePath);
    if (!file.is_open()) {
        std::cout << "[Loi] Khong the mo file de ghi: " << dataFilePath << "\n";
        return;
    }

    nlohmann::json jArr = nlohmann::json::array();
    for (const Rent* r : list) {
        if (r != nullptr) {
            jArr.push_back(r->toJson());
        }
    }

    file << jArr.dump(2);
    std::cout << "[Thanh cong] Da luu " << list.size() << " hop dong vao " << dataFilePath << "\n";
}

std::vector<Rent*> RentManager::findByStatus(RentStatus status) const {
    std::vector<Rent*> results;
    for (Rent* r : list) {
        if (r != nullptr && r->getStatus() == status) {
            results.push_back(r);
        }
    }
    return results;
}

std::vector<Rent*> RentManager::findByStudent(const std::string& studentId) const {
    std::vector<Rent*> results;
    for (Rent* r : list) {
        if (r != nullptr && r->getTenant() != nullptr && r->getTenant()->getId() == studentId) {
            results.push_back(r);
        }
    }
    return results;
}

std::vector<Rent*> RentManager::findByRoom(const std::string& roomId) const {
    std::vector<Rent*> results;
    for (Rent* r : list) {
        if (r != nullptr && r->getRoom() != nullptr && r->getRoom()->getId() == roomId) {
            results.push_back(r);
        }
    }
    return results;
}

Rent* RentManager::createRent(const std::string& rentId, Student* student, Room* room,
                             Period period, const Date& startDate, double fee) {
    if (student == nullptr || room == nullptr) {
        std::cout << "[Loi] Sinh vien hoac Phong khong hop le!\n";
        return nullptr;
    }

    if (student->getRent() != nullptr && student->getRent()->isActive()) {
        std::cout << "[Loi] Sinh vien " << student->getId() << " dang co hop dong thue phong dang hieu luc!\n";
        return nullptr;
    }

    if (room->isFull()) {
        std::cout << "[Loi] Phong " << room->getId() << " da day cho!\n";
        return nullptr;
    }

    if (room->getStatus() == RoomStatus::MAINTENANCE) {
        std::cout << "[Loi] Phong " << room->getId() << " dang bao tri, khong the xep phong!\n";
        return nullptr;
    }

    Rent* newRent = new Rent(rentId, student, room, period, startDate, fee);
    if (!add(newRent)) {
        delete newRent;
        return nullptr;
    }

    student->setRent(newRent);
    room->addRent(newRent);

    std::cout << "[Thanh cong] Da tao hop dong " << rentId << " cho sinh vien "
              << student->getId() << " tai phong " << room->getId() << "\n";
    return newRent;
}

bool RentManager::terminateRent(const std::string& rentId, const Date& actualEndDate) {
    Rent* r = findById(rentId);
    if (r == nullptr) {
        std::cout << "[Loi] Khong tim thay hop dong: " << rentId << "\n";
        return false;
    }

    if (!r->isActive()) {
        std::cout << "[Canh bao] Hop dong " << rentId << " von da khong con hieu luc!\n";
        return false;
    }

    r->terminateEarly(actualEndDate);

    if (r->getRoom() != nullptr) {
        r->getRoom()->removeRent(r);
    }
    if (r->getTenant() != nullptr) {
        r->getTenant()->setRent(nullptr);
    }

    std::cout << "[Thanh cong] Da thanh ly som hop dong " << rentId << " vao ngay "
              << actualEndDate.toString() << "\n";
    return true;
}
