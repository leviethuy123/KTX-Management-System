#include "manager/RoomManager.h"
#include "Rent.h"
#include "Human.h"
#include <fstream>
#include <iostream>
#include <iomanip>

RoomManager::RoomManager(const std::string& filePath)
    : Manager<Room>(filePath) {}

void RoomManager::loadFromFile() {
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
            std::string type = item.value("roomType", "STANDARD");
            if (type == "VIP") {
                list.push_back(new VipRoom(VipRoom::fromJson(item)));
            } else {
                list.push_back(new StandardRoom(StandardRoom::fromJson(item)));
            }
        }
        std::cout << "[Thành công] Đã nạp " << list.size() << " phòng từ " << dataFilePath << "\n";
    } catch (const std::exception& e) {
        std::cout << "[Lỗi] Parse JSON phòng thất bại: " << e.what() << "\n";
    }
}

void RoomManager::saveToFile() const {
    std::ofstream file(dataFilePath);
    if (!file.is_open()) {
        std::cout << "[Lỗi] Không thể mở file để ghi: " << dataFilePath << "\n";
        return;
    }

    nlohmann::json jArr = nlohmann::json::array();
    for (const Room* r : list) {
        if (r != nullptr) {
            jArr.push_back(r->toJson());
        }
    }

    file << jArr.dump(2);
    std::cout << "[Thành công] Đã lưu " << list.size() << " phòng vào " << dataFilePath << "\n";
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
        std::cout << "(Không còn phòng nào trống chỗ)\n";
        return;
    }
    std::cout << "\n=== DANH SÁCH PHÒNG CÒN CHỖ TRỐNG (" << available.size() << " phòng) ===\n";
    TablePrinter::printRooms(available);
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
    int totalWidth = 98;
    std::cout << "\n╔" << StringUtils::repeat("═", totalWidth) << "╗\n";
    std::cout << "║" << StringUtils::padCenter("BÁO CÁO TỶ LỆ LẤP ĐẦY PHÒNG KÝ TÚC XÁ", totalWidth) << "║\n";
    std::cout << "╠══════════╦════════════╦══════════════╦════════════╦══════════╦══════════════╦════════════════════╣\n";
    std::cout << "║ " << StringUtils::padRight("MÃ PHÒNG", 8)
              << " ║ " << StringUtils::padRight("TÒA NHÀ", 10)
              << " ║ " << StringUtils::padRight("LOẠI PHÒNG", 12)
              << " ║ " << StringUtils::padRight("SỨC CHỨA", 10)
              << " ║ " << StringUtils::padRight("ĐANG Ở", 8)
              << " ║ " << StringUtils::padRight("TỶ LỆ (%)", 12)
              << " ║ " << StringUtils::padRight("TRẠNG THÁI", 18) << " ║\n";
    std::cout << "╠══════════╬════════════╬══════════════╬════════════╬══════════╬══════════════╬════════════════════╣\n";

    for (const Room* r : list) {
        if (r != nullptr) {
            int occupants = static_cast<int>(r->getActiveRent().size());
            int cap = r->getMaxStudents();
            double pct = cap > 0 ? (occupants * 100.0 / cap) : 0.0;
            std::ostringstream pctOss;
            pctOss << std::fixed << std::setprecision(1) << pct << " %";

            std::cout << "║ " << StringUtils::padRight(r->getId(), 8)
                      << " ║ " << StringUtils::padRight(r->getBuilding(), 10)
                      << " ║ " << StringUtils::padRight(r->getRoomType(), 12)
                      << " ║ " << StringUtils::padRight(std::to_string(cap) + " chỗ", 10)
                      << " ║ " << StringUtils::padRight(std::to_string(occupants) + " SV", 8)
                      << " ║ " << StringUtils::padRight(pctOss.str(), 12)
                      << " ║ " << StringUtils::padRight(EnumHelper::roomStatusToString(r->getStatus()), 18) << " ║\n";
        }
    }
    std::cout << "╠══════════╩════════════╩══════════════╩════════════╩══════════╩══════════════╩════════════════════╣\n";

    int totalCap = 0, totalOcc = 0;
    double overallRate = 0.0;
    getOccupancyStats(totalCap, totalOcc, overallRate);
    std::ostringstream overallOss;
    overallOss << std::fixed << std::setprecision(2) << overallRate << " %";

    std::cout << "║ " << StringUtils::padRight("TỔNG SỨC CHỨA TOÀN KTX : " + std::to_string(totalCap) + " chỗ", 96) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("TỔNG SINH VIÊN ĐANG Ở : " + std::to_string(totalOcc) + " sinh viên", 96) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("SỐ CHỖ CÒN TRỐNG       : " + std::to_string(totalCap - totalOcc) + " chỗ", 96) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("TỶ LỆ LẤP ĐẦY TOÀN KTX : " + overallOss.str(), 96) << " ║\n";
    std::cout << "╚" << StringUtils::repeat("═", totalWidth) << "╝\n";
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
    int cardWidth = 74;

    std::cout << "\n╔" << StringUtils::repeat("═", cardWidth) << "╗\n";
    std::cout << "║" << StringUtils::padCenter("PHIẾU BÁO TIỀN PHÒNG & SINH HOẠT PHÍ", cardWidth) << "║\n";
    std::cout << "╠" << StringUtils::repeat("═", cardWidth) << "╣\n";
    std::cout << "║ " << StringUtils::padRight("Mã phòng     : " + r->getId() + " (" + r->getRoomType() + ")", cardWidth - 2) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("Tòa nhà      : " + r->getBuilding() + " - Tầng " + std::to_string(r->getFloor()), cardWidth - 2) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("Số người ở   : " + std::to_string(occupants) + "/" + std::to_string(r->getMaxStudents()) + " sinh viên", cardWidth - 2) << " ║\n";
    std::cout << "╠" << StringUtils::repeat("═", cardWidth) << "╣\n";
    std::cout << "║ " << StringUtils::padRight("1. Tiền thuê phòng cơ bản : " + StringUtils::formatMoney(baseFee), cardWidth - 2) << " ║\n";
    if (r->getRoomType() == "VIP") {
        std::cout << "║ " << StringUtils::padRight("2. Phí dịch vụ VIP        : 250000 VND", cardWidth - 2) << " ║\n";
    }
    std::ostringstream elecOss, waterOss;
    elecOss << std::fixed << std::setprecision(1) << electricUsage;
    waterOss << std::fixed << std::setprecision(1) << waterUsage;
    std::cout << "║ " << StringUtils::padRight("3. Tiền điện (" + elecOss.str() + " kWh x 3,000đ) : " + StringUtils::formatMoney(electricCost), cardWidth - 2) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("4. Tiền nước (" + waterOss.str() + " m³ x 10,000đ)  : " + StringUtils::formatMoney(waterCost), cardWidth - 2) << " ║\n";
    std::cout << "╠" << StringUtils::repeat("═", cardWidth) << "╣\n";
    std::cout << "║ " << StringUtils::padRight(">> TỔNG CHI PHÍ CẢ PHÒNG : " + StringUtils::formatMoney(totalPolymorphicFee), cardWidth - 2) << " ║\n";

    if (occupants > 0) {
        double utilityPerPerson = utilityCost / occupants;
        std::cout << "║ " << StringUtils::padRight(">> Tiền điện nước mỗi bạn : " + StringUtils::formatMoney(utilityPerPerson) + " / người", cardWidth - 2) << " ║\n";
        std::cout << "║ " << StringUtils::padRight(">> Danh sách sinh viên nhận thông báo:", cardWidth - 2) << " ║\n";
        for (const Rent* rent : r->getActiveRent()) {
            if (rent != nullptr && rent->getTenant() != nullptr) {
                std::string lineInfo = "   * SV: " + rent->getTenant()->getFullName()
                                     + " (" + rent->getTenant()->getId() + ")"
                                     + " | Tiền phòng: " + StringUtils::formatMoney(rent->getRentFee())
                                     + " + Điện nước: " + StringUtils::formatMoney(utilityPerPerson)
                                     + " => Tổng: " + StringUtils::formatMoney(rent->getRentFee() + utilityPerPerson);
                if (StringUtils::utf8Width(lineInfo) > cardWidth - 2) {
                    lineInfo = lineInfo.substr(0, cardWidth - 5) + "...";
                }
                std::cout << "║ " << StringUtils::padRight(lineInfo, cardWidth - 2) << " ║\n";
            }
        }
    } else {
        std::cout << "║ " << StringUtils::padCenter("(Phòng hiện chưa có người ở - Chi phí do KTX bảo trì)", cardWidth) << "║\n";
    }
    std::cout << "╚" << StringUtils::repeat("═", cardWidth) << "╝\n";
}

RoomRevenueRow RoomManager::calculateRoomMonthlyRevenue(const std::string& roomId, double electricUsage, double waterUsage) const {
    RoomRevenueRow row = {};
    Room* r = findById(roomId);
    if (r == nullptr) {
        row.roomId = roomId;
        return row;
    }

    row.roomId = r->getId();
    row.roomType = r->getRoomType();
    row.occupants = static_cast<int>(r->getActiveRent().size());
    row.maxCapacity = r->getMaxStudents();
    row.basePrice = r->getBasePrice();
    row.serviceFee = 0.0;
    if (r->getRoomType() == "VIP") {
        const VipRoom* vip = dynamic_cast<const VipRoom*>(r);
        row.serviceFee = vip ? vip->getServiceFee() : 250000.0;
    }

    double rentTotal = 0.0;
    for (const Rent* rent : r->getActiveRent()) {
        if (rent != nullptr && rent->isActive()) {
            rentTotal += rent->getRentFee();
        }
    }
    if (rentTotal == 0.0 && row.occupants > 0) {
        rentTotal = row.basePrice * row.occupants;
    }
    row.rentFeeTotal = rentTotal;

    row.electricCost = electricUsage * 3000.0;
    row.waterCost = waterUsage * 10000.0;

    if (row.occupants > 0) {
        row.totalRevenue = row.rentFeeTotal + row.serviceFee + row.electricCost + row.waterCost;
    } else {
        row.totalRevenue = 0.0;
        row.serviceFee = 0.0;
        row.electricCost = 0.0;
        row.waterCost = 0.0;
    }
    return row;
}

std::vector<RoomRevenueRow> RoomManager::calculateAllRoomsMonthlyRevenue(double defaultElectric, double defaultWater) const {
    std::vector<RoomRevenueRow> rows;
    for (const Room* r : list) {
        if (r != nullptr) {
            rows.push_back(calculateRoomMonthlyRevenue(r->getId(), defaultElectric, defaultWater));
        }
    }
    return rows;
}

void RoomManager::printMonthlyRevenueReport(const std::string& monthYear, double defaultElec, double defaultWater) const {
    auto rows = calculateAllRoomsMonthlyRevenue(defaultElec, defaultWater);
    TablePrinter::printRevenueTable(rows, monthYear);
}

void RoomManager::printSpecificRoomRevenue(const std::string& roomId, const std::string& monthYear, double electricUsage, double waterUsage) const {
    RoomRevenueRow row = calculateRoomMonthlyRevenue(roomId, electricUsage, waterUsage);
    TablePrinter::printSingleRoomRevenue(row, monthYear);
}
