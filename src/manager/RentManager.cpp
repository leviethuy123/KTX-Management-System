#include "manager/RentManager.h"
#include <fstream>
#include <iostream>

RentManager::RentManager(const std::string& filePath)
    : Manager<Rent>(filePath) {}

void RentManager::loadFromFile() {
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
            Rent r = Rent::fromJson(item, nullptr, nullptr);
            list.push_back(new Rent(r));
        }
        std::cout << "[Thành công] Đã nạp " << list.size() << " hợp đồng từ " << dataFilePath << "\n";
    } catch (const std::exception& e) {
        std::cout << "[Lỗi] Parse JSON hợp đồng thất bại: " << e.what() << "\n";
    }
}

void RentManager::loadFromFile(StudentManager& sm, RoomManager& rm) {
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
            std::string sId = item.value("studentId", "");
            std::string rId = item.value("roomId", "");

            Student* student = sm.findById(sId);
            Room* room = rm.findById(rId);

            Rent r = Rent::fromJson(item, student, room);
            Rent* newRent = new Rent(r);
            list.push_back(newRent);

            if (newRent->isActive()) {
                if (student != nullptr) {
                    student->setRent(newRent);
                }
                if (room != nullptr) {
                    room->addRent(newRent);
                }
            }
        }
        std::cout << "[Thành công] Đã nạp và đồng bộ " << list.size() << " hợp đồng từ " << dataFilePath << "\n";
    } catch (const std::exception& e) {
        std::cout << "[Lỗi] Parse JSON hợp đồng thất bại: " << e.what() << "\n";
    }
}

void RentManager::saveToFile() const {
    std::ofstream file(dataFilePath);
    if (!file.is_open()) {
        std::cout << "[Lỗi] Không thể mở file để ghi: " << dataFilePath << "\n";
        return;
    }

    nlohmann::json jArr = nlohmann::json::array();
    for (const Rent* r : list) {
        if (r != nullptr) {
            jArr.push_back(r->toJson());
        }
    }

    file << jArr.dump(2);
    std::cout << "[Thành công] Đã lưu " << list.size() << " hợp đồng vào " << dataFilePath << "\n";
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
        std::cout << "[Lỗi] Sinh viên hoặc Phòng không hợp lệ!\n";
        return nullptr;
    }

    if (student->getRent() != nullptr && student->getRent()->isActive()) {
        std::cout << "[Lỗi] Sinh viên " << student->getId() << " đang có hợp đồng thuê phòng đang hiệu lực!\n";
        return nullptr;
    }

    if (room->isFull()) {
        std::cout << "[Lỗi] Phòng " << room->getId() << " đã đầy chỗ!\n";
        return nullptr;
    }

    if (room->getStatus() == RoomStatus::MAINTENANCE) {
        std::cout << "[Lỗi] Phòng " << room->getId() << " đang bảo trì, không thể xếp phòng!\n";
        return nullptr;
    }

    Rent* newRent = new Rent(rentId, student, room, period, startDate, fee);
    if (!add(newRent)) {
        delete newRent;
        return nullptr;
    }

    student->setRent(newRent);
    room->addRent(newRent);

    std::cout << "[Thành công] Đã tạo hợp đồng " << rentId << " cho sinh viên "
              << student->getId() << " tại phòng " << room->getId() << "\n";
    return newRent;
}

bool RentManager::terminateRent(const std::string& rentId, const Date& actualEndDate) {
    Rent* r = findById(rentId);
    if (r == nullptr) {
        std::cout << "[Lỗi] Không tìm thấy hợp đồng: " << rentId << "\n";
        return false;
    }

    if (!r->isActive()) {
        std::cout << "[Cảnh báo] Hợp đồng " << rentId << " vốn đã không còn hiệu lực!\n";
        return false;
    }

    r->terminateEarly(actualEndDate);

    if (r->getRoom() != nullptr) {
        r->getRoom()->removeRent(r);
    }
    if (r->getTenant() != nullptr) {
        r->getTenant()->setRent(nullptr);
    }

    std::cout << "[Thành công] Đã thanh lý sớm hợp đồng " << rentId << " vào ngày "
              << actualEndDate.toString() << "\n";
    return true;
}

std::vector<Rent*> RentManager::findExpiringRents(const Date& currentDate, int daysThreshold) const {
    std::vector<Rent*> results;
    for (Rent* r : list) {
        if (r != nullptr && r->isActive()) {
            Date expDate = r->getExpectedEndDate();
            int daysLeft = currentDate.daysBetween(expDate);
            if (daysLeft >= 0 && daysLeft <= daysThreshold) {
                results.push_back(r);
            }
        }
    }
    return results;
}

void RentManager::printExpiringRents(const Date& currentDate, int daysThreshold) const {
    auto expiring = findExpiringRents(currentDate, daysThreshold);
    std::cout << "=== DANH SÁCH HỢP ĐỒNG SẮP HẾT HẠN (Trong vòng " << daysThreshold << " ngày tính đến " << currentDate.toString() << ") ===\n";
    if (expiring.empty()) {
        std::cout << "(Không có hợp đồng nào sắp hết hạn)\n";
        return;
    }
    for (Rent* r : expiring) {
        int daysLeft = currentDate.daysBetween(r->getExpectedEndDate());
        std::cout << "-> Hợp đồng: " << r->getId()
                  << " | Sinh viên: " << (r->getTenant() ? r->getTenant()->getFullName() : "(N/A)")
                  << " | Phòng: " << (r->getRoom() ? r->getRoom()->getId() : "(N/A)")
                  << " | Ngày hết hạn: " << r->getExpectedEndDate().toString()
                  << " | Còn lại: " << daysLeft << " ngày\n";
    }
}

int RentManager::checkAndExpireRents(const Date& currentDate) {
    int count = 0;
    for (Rent* r : list) {
        if (r != nullptr && r->isActive()) {
            if (r->getExpectedEndDate() < currentDate) {
                r->setStatus(RentStatus::EXPIRED);
                r->setActualEndDate(r->getExpectedEndDate());
                if (r->getRoom() != nullptr) {
                    r->getRoom()->removeRent(r);
                }
                if (r->getTenant() != nullptr) {
                    r->getTenant()->setRent(nullptr);
                }
                std::cout << "[Hết hạn] Hợp đồng " << r->getId() << " đã hết hạn vào ngày "
                          << r->getExpectedEndDate().toString() << ". Đã giải phóng phòng & cập nhật trạng thái.\n";
                count++;
            }
        }
    }
    return count;
}

double RentManager::calculateTotalMonthlyRevenue() const {
    double total = 0.0;
    for (const Rent* r : list) {
        if (r != nullptr && r->isActive()) {
            total += r->getRentFee();
        }
    }
    return total;
}
