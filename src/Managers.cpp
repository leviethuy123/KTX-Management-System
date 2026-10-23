#include "Managers.h"

// =========================================================================
// 1. CÀI ĐẶT STUDENTMANAGER
// =========================================================================

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

// =========================================================================
// 2. CÀI ĐẶT ROOMMANAGER
// =========================================================================

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
    std::cout << "=== DANH SÁCH PHÒNG CÒN CHỖ TRỐNG (" << available.size() << " phòng) ===\n";
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
    std::cout << "                         BÁO CÁO TỶ LỆ LẤP ĐẦY PHÒNG KÝ TÚC XÁ                          \n";
    std::cout << "========================================================================================\n";
    std::cout << std::left << std::setw(12) << "MÃ PHÒNG"
              << std::setw(14) << "TÒA NHÀ"
              << std::setw(14) << "LOẠI PHÒNG"
              << std::setw(12) << "SỨC CHỨA"
              << std::setw(12) << "ĐANG Ở"
              << std::setw(14) << "TỶ LỆ (%)"
              << "TRẠNG THÁI\n";
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

    std::cout << "-> TỔNG SỨC CHỨA TOÀN KTX : " << totalCap << " chỗ\n";
    std::cout << "-> TỔNG SINH VIÊN ĐANG Ở : " << totalOcc << " sinh viên\n";
    std::cout << "-> SỐ CHỖ CÒN TRỐNG       : " << (totalCap - totalOcc) << " chỗ\n";
    std::cout << "-> TỶ LỆ LẤP ĐẦY TOÀN KTX : " << std::fixed << std::setprecision(2) << overallRate << " %\n";
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

// =========================================================================
// 3. CÀI ĐẶT STAFFMANAGER
// =========================================================================

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

// =========================================================================
// 4. CÀI ĐẶT RENTMANAGER
// =========================================================================

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

// =========================================================================
// 5. CÀI ĐẶT ACCOUNTMANAGER
// =========================================================================

AccountManager::AccountManager(const std::string& filePath)
    : Manager<Account>(filePath) {}

void AccountManager::loadFromFile() {
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
            Account acc = Account::fromJson(item);
            list.push_back(new Account(acc));
        }
        std::cout << "[Thành công] Đã nạp " << list.size() << " tài khoản từ " << dataFilePath << "\n";
    } catch (const std::exception& e) {
        std::cout << "[Lỗi] Parse JSON tài khoản thất bại: " << e.what() << "\n";
    }
}

void AccountManager::saveToFile() const {
    std::ofstream file(dataFilePath);
    if (!file.is_open()) {
        std::cout << "[Lỗi] Không thể mở file để ghi: " << dataFilePath << "\n";
        return;
    }

    nlohmann::json jArr = nlohmann::json::array();
    for (const Account* acc : list) {
        if (acc != nullptr) {
            jArr.push_back(acc->toJson());
        }
    }

    file << jArr.dump(2);
    std::cout << "[Thành công] Đã lưu " << list.size() << " tài khoản vào " << dataFilePath << "\n";
}

Account* AccountManager::authenticate(const std::string& username, const std::string& pass) {
    Account* acc = findByUsername(username);
    if (acc == nullptr) {
        std::cout << "[Xác thực thất bại] Không tồn tại tài khoản với tên đăng nhập: '" << username << "'\n";
        return nullptr;
    }

    if (!acc->getStatus()) {
        std::cout << "[Xác thực thất bại] Tài khoản '" << username << "' đang bị khóa!\n";
        return nullptr;
    }

    if (!acc->verifyPassword(pass)) {
        std::cout << "[Xác thực thất bại] Sai mật khẩu!\n";
        return nullptr;
    }

    std::cout << "[Xác thực thành công] Xin chào " << username
              << " (" << EnumHelper::roleToString(acc->getRole()) << ")!\n";
    return acc;
}

Account* AccountManager::findByUsername(const std::string& username) const {
    for (Account* acc : list) {
        if (acc != nullptr && acc->getUsername() == username) {
            return acc;
        }
    }
    return nullptr;
}

Account* AccountManager::findByOwnerId(const std::string& ownerId) const {
    for (Account* acc : list) {
        if (acc != nullptr && acc->getOwnerId() == ownerId) {
            return acc;
        }
    }
    return nullptr;
}

Account* AccountManager::createAccount(const std::string& accId, const std::string& username,
                                      const std::string& password, Role role,
                                      const std::string& ownerId) {
    if (findById(accId) != nullptr) {
        std::cout << "[Lỗi] Mã tài khoản '" << accId << "' đã tồn tại!\n";
        return nullptr;
    }

    if (findByUsername(username) != nullptr) {
        std::cout << "[Lỗi] Tên đăng nhập '" << username << "' đã được sử dụng!\n";
        return nullptr;
    }

    Account* newAcc = new Account(accId, username, password, role, ownerId, true);
    list.push_back(newAcc);
    std::cout << "[Thành công] Đã tạo tài khoản '" << username << "' ("
              << EnumHelper::roleToString(role) << ")\n";
    return newAcc;
}
