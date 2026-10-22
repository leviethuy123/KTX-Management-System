#include "manager/RoomManager.h"
#include "model/Rent.h"
#include "model/Student.h"
#include <fstream>
#include <iostream>
#include <iomanip>

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

void RoomManager::getOccupancyStats(int& totalCapacity, int& totalOccupants, double& rate) const {
    totalCapacity = 0;
    totalOccupants = 0;
    rate = 0.0;

    for (const Room* r : list) {
        if (r != nullptr) {
            totalCapacity += r->getMaxStudents();
            totalOccupants += static_cast<int>(r->getActiveRent().size());
        }
    }

    if (totalCapacity > 0) {
        rate = (totalOccupants * 100.0) / totalCapacity;
    }
}

void RoomManager::printOccupancyReport() const {
    std::cout << "========================================================================================\n";
    std::cout << "                         BAO CAO TY LE LAP DAY PHONG KY TUC XA                          \n";
    std::cout << "========================================================================================\n";
    std::cout << std::left << std::setw(12) << "MA PHONG"
              << std::setw(14) << "TOA NHA"
              << std::setw(14) << "LOAI PHONG"
              << std::setw(12) << "SUC CHUA"
              << std::setw(12) << "DANG O"
              << std::setw(14) << "TY LE (%)"
              << "TRANG THAI\n";
    std::cout << "----------------------------------------------------------------------------------------\n";

    for (const Room* r : list) {
        if (r != nullptr) {
            int occupants = static_cast<int>(r->getActiveRent().size());
            int cap = r->getMaxStudents();
            double pct = cap > 0 ? (occupants * 100.0 / cap) : 0.0;

            std::cout << std::left << std::setw(12) << r->getId()
                      << std::setw(14) << r->getBuilding()
                      << std::setw(14) << r->getRoomType()
                      << std::setw(12) << cap
                      << std::setw(12) << occupants
                      << std::setw(12) << std::fixed << std::setprecision(1) << pct << "% "
                      << EnumHelper::roomStatusToString(r->getStatus()) << "\n";
        }
    }
    std::cout << "----------------------------------------------------------------------------------------\n";

    int totalCap = 0, totalOcc = 0;
    double overallRate = 0.0;
    getOccupancyStats(totalCap, totalOcc, overallRate);

    std::cout << "-> TONG SUC CHUA TOAN KTX : " << totalCap << " cho\n";
    std::cout << "-> TONG SINH VIEN DANG O : " << totalOcc << " sinh vien\n";
    std::cout << "-> SO CHO CON TRONG       : " << (totalCap - totalOcc) << " cho\n";
    std::cout << "-> TY LE LAP DAY TOAN KTX : " << std::fixed << std::setprecision(2) << overallRate << " %\n";
    std::cout << "========================================================================================\n";
}

void RoomManager::printRoomBill(const std::string& roomId, double electricUsage, double waterUsage) const {
    Room* r = findById(roomId);
    if (r == nullptr) {
        std::cout << "[Lỗi] Không tìm thấy phòng: " << roomId << " để xuất hóa đơn!\n";
        return;
    }

    double baseFee = r->getBasePrice();
    double totalPolymorphicFee = r->calculateTotalFee(electricUsage, waterUsage);
    double electricCost = electricUsage * 3000.0;
    double waterCost = waterUsage * 10000.0;
    double utilityCost = electricCost + waterCost;

    int occupants = static_cast<int>(r->getActiveRent().size());

    std::cout << "\n=========================================================\n";
    std::cout << "              PHIẾU BÁO TIỀN PHÒNG & SINH HOẠT PHÍ       \n";
    std::cout << "=========================================================\n";
    std::cout << " Mã phòng     : " << r->getId() << " (" << r->getRoomType() << ")\n";
    std::cout << " Tòa nhà      : " << r->getBuilding() << " - Tầng " << r->getFloor() << "\n";
    std::cout << " Số người ở   : " << occupants << "/" << r->getMaxStudents() << " sinh viên\n";
    std::cout << "---------------------------------------------------------\n";
    std::cout << " 1. Tiền thuê phòng cơ bản : " << std::fixed << std::setprecision(0) << baseFee << " VNĐ\n";
    if (r->getRoomType() == "VIP") {
        VipRoom* vip = dynamic_cast<VipRoom*>(r);
        std::cout << " 2. Phí dịch vụ VIP        : 250,000 VNĐ\n";
    }
    std::cout << " 3. Tiền điện (" << electricUsage << " kWh x 3,000đ) : " << electricCost << " VNĐ\n";
    std::cout << " 4. Tiền nước (" << waterUsage << " m³ x 10,000đ)  : " << waterCost << " VNĐ\n";
    std::cout << "---------------------------------------------------------\n";
    std::cout << " >> TỔNG CHI PHÍ CẢ PHÒNG : " << totalPolymorphicFee << " VNĐ\n";

    if (occupants > 0) {
        double utilityPerPerson = utilityCost / occupants;
        std::cout << " >> Tiền điện nước mỗi bạn : " << utilityPerPerson << " VNĐ / người\n";
        std::cout << " >> Danh sách sinh viên nhận thông báo:\n";
        for (const Rent* rent : r->getActiveRent()) {
            if (rent != nullptr && rent->getTenant() != nullptr) {
                std::cout << "    * SV: " << rent->getTenant()->getFullName()
                          << " (" << rent->getTenant()->getId() << ")"
                          << " | Tiền phòng: " << rent->getRentFee() << " VNĐ"
                          << " + Điện nước: " << utilityPerPerson << " VNĐ"
                          << " => Tổng nộp: " << (rent->getRentFee() + utilityPerPerson) << " VNĐ\n";
            }
        }
    } else {
        std::cout << " (Phòng hiện chưa có người ở - Chi phí do KTX bảo trì)\n";
    }
    std::cout << "=========================================================\n";
}

