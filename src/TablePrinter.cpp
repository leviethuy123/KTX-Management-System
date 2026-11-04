#include "TablePrinter.h"
#include "Human.h"
#include "Room.h"
#include "Rent.h"
#include "Account.h"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace {
    std::string formatMoney(double amount) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << amount << " đ";
        return oss.str();
    }
}

void TablePrinter::printStudents(const std::vector<Student*>& list) {
    if (list.empty()) {
        std::cout << "(Không có sinh viên nào)\n";
        return;
    }
    std::cout << "+--------+----------------------+--------+--------+-------------+-------+------------------------+------------+-------------------------+--------+\n";
    std::cout << "| MÃ SV  | HỌ VÀ TÊN            | GIỚI   | NĂM    | LỚP         | KHÓA  | KHOA                   | SĐT        | EMAIL                   | PHÒNG  |\n";
    std::cout << "+--------+----------------------+--------+--------+-------------+-------+------------------------+------------+-------------------------+--------+\n";
    for (const Student* s : list) {
        if (s == nullptr) continue;
        std::string genderStr = (s->getGender() == Gender::MALE) ? "Nam" : "Nu";
        std::string roomStr = (s->getRent() != nullptr && s->getRent()->isActive() && s->getRent()->getRoom() != nullptr)
                              ? s->getRent()->getRoom()->getId() : "(Chưa)";
        std::string nameSub = s->getFullName().substr(0, 20);
        std::string classSub = s->getClass().substr(0, 11);
        std::string facSub = s->getFaculty().substr(0, 22);
        std::string emSub = s->getEmail().substr(0, 23);

        std::cout << "| " << std::left << std::setw(6) << s->getId()
                  << " | " << std::setw(20) << nameSub
                  << " | " << std::setw(6) << genderStr
                  << " | " << std::setw(6) << s->getBirthYear()
                  << " | " << std::setw(11) << classSub
                  << " | " << std::setw(5) << s->getCohort()
                  << " | " << std::setw(22) << facSub
                  << " | " << std::setw(10) << s->getPhone()
                  << " | " << std::setw(23) << emSub
                  << " | " << std::setw(6) << roomStr
                  << " |\n";
    }
    std::cout << "+--------+----------------------+--------+--------+-------------+-------+------------------------+------------+-------------------------+--------+\n";
    std::cout << ">> TỔNG SỐ SINH VIÊN: " << list.size() << " sinh viên\n";
}

void TablePrinter::printRooms(const std::vector<Room*>& list) {
    if (list.empty()) {
        std::cout << "(Không có phòng nào)\n";
        return;
    }
    std::cout << "+--------+------------+------+----------+----------+--------+-----------+---------------+------------+--------------------+\n";
    std::cout << "| PHÒNG  | TÒA NHÀ    | TẦNG | LOẠI     | SỨC CHỨA | ĐANG Ở | CÒN TRỐNG | GIÁ PHÒNG     | TRẠNG THÁI | THIẾT BỊ HỎNG      |\n";
    std::cout << "+--------+------------+------+----------+----------+--------+-----------+---------------+------------+--------------------+\n";
    for (const Room* r : list) {
        if (r == nullptr) continue;
        int occupants = static_cast<int>(r->getActiveRent().size());
        int freeSlots = r->getMaxStudents() - occupants;
        int brokenCount = r->countUnfixedDevices();
        std::string brokenStr = (brokenCount > 0) ? ("Có " + std::to_string(brokenCount) + " thiết bị") : "Hoạt động tốt";

        std::cout << "| " << std::left << std::setw(6) << r->getId()
                  << " | " << std::setw(10) << r->getBuilding()
                  << " | " << std::setw(4) << r->getFloor()
                  << " | " << std::setw(8) << r->getRoomType()
                  << " | " << std::setw(8) << r->getMaxStudents()
                  << " | " << std::setw(6) << occupants
                  << " | " << std::setw(9) << freeSlots
                  << " | " << std::setw(13) << formatMoney(r->getBasePrice())
                  << " | " << std::setw(10) << EnumHelper::roomStatusToString(r->getStatus())
                  << " | " << std::setw(18) << brokenStr
                  << " |\n";
    }
    std::cout << "+--------+------------+------+----------+----------+--------+-----------+---------------+------------+--------------------+\n";
    std::cout << ">> TỔNG SỐ PHÒNG: " << list.size() << " phòng\n";
}

void TablePrinter::printRents(const std::vector<Rent*>& list) {
    if (list.empty()) {
        std::cout << "(Không có hợp đồng nào)\n";
        return;
    }
    std::cout << "+--------+--------+----------------------+--------+------------+------------+----------+---------------+------------+\n";
    std::cout << "| MÃ HĐ  | MÃ SV  | TÊN SINH VIÊN        | PHÒNG  | NGÀY BĐ    | NGÀY KT    | THỜI HẠN | GIÁ THUÊ/THÁNG| TRẠNG THÁI |\n";
    std::cout << "+--------+--------+----------------------+--------+------------+------------+----------+---------------+------------+\n";
    for (const Rent* r : list) {
        if (r == nullptr) continue;
        std::string sId = r->getTenant() ? r->getTenant()->getId() : "N/A";
        std::string sName = r->getTenant() ? r->getTenant()->getFullName().substr(0, 20) : "N/A";
        std::string rId = r->getRoom() ? r->getRoom()->getId() : "N/A";

        std::cout << "| " << std::left << std::setw(6) << r->getId()
                  << " | " << std::setw(6) << sId
                  << " | " << std::setw(20) << sName
                  << " | " << std::setw(6) << rId
                  << " | " << std::setw(10) << r->getStartDate().toString()
                  << " | " << std::setw(10) << r->getActualEndDate().toString()
                  << " | " << std::setw(8) << EnumHelper::periodToString(r->getRentalPeriod())
                  << " | " << std::setw(13) << formatMoney(r->getRentFee())
                  << " | " << std::setw(10) << EnumHelper::rentStatusToString(r->getStatus())
                  << " |\n";
    }
    std::cout << "+--------+--------+----------------------+--------+------------+------------+----------+---------------+------------+\n";
    std::cout << ">> TỔNG SỐ HỢP ĐỒNG: " << list.size() << " hợp đồng\n";
}

void TablePrinter::printStaffs(const std::vector<Staff*>& list) {
    if (list.empty()) {
        std::cout << "(Không có cán bộ nào)\n";
        return;
    }
    std::cout << "+--------+----------------------+--------+------------------------+-------------------------+------------+\n";
    std::cout << "| MÃ CB  | HỌ VÀ TÊN            | GIỚI   | PHÒNG BAN              | CHỨC VỤ                 | SĐT        |\n";
    std::cout << "+--------+----------------------+--------+------------------------+-------------------------+------------+\n";
    for (const Staff* st : list) {
        if (st == nullptr) continue;
        std::string genderStr = (st->getGender() == Gender::MALE) ? "Nam" : "Nu";
        std::string nameSub = st->getFullName().substr(0, 20);
        std::string deptSub = st->getDepartment().substr(0, 22);
        std::string posSub = st->getPosition().substr(0, 23);

        std::cout << "| " << std::left << std::setw(6) << st->getId()
                  << " | " << std::setw(20) << nameSub
                  << " | " << std::setw(6) << genderStr
                  << " | " << std::setw(22) << deptSub
                  << " | " << std::setw(23) << posSub
                  << " | " << std::setw(10) << st->getPhone()
                  << " |\n";
    }
    std::cout << "+--------+----------------------+--------+------------------------+-------------------------+------------+\n";
    std::cout << ">> TỔNG SỐ CÁN BỘ: " << list.size() << " người\n";
}

void TablePrinter::printAccounts(const std::vector<Account*>& list) {
    if (list.empty()) {
        std::cout << "(Không có tài khoản nào)\n";
        return;
    }
    std::cout << "+----------+----------------------+------------+------------+--------------------+\n";
    std::cout << "| MÃ TK    | TÊN ĐĂNG NHẬP        | VAI TRÒ    | MÃ LIÊN KẾT| TRẠNG THÁI         |\n";
    std::cout << "+----------+----------------------+------------+------------+--------------------+\n";
    for (const Account* a : list) {
        if (a == nullptr) continue;
        std::string stStr = a->getStatus() ? "KÍCH HOẠT" : "BỊ KHÓA";

        std::cout << "| " << std::left << std::setw(8) << a->getId()
                  << " | " << std::setw(20) << a->getUsername()
                  << " | " << std::setw(10) << EnumHelper::roleToString(a->getRole())
                  << " | " << std::setw(10) << a->getOwnerId()
                  << " | " << std::setw(18) << stStr
                  << " |\n";
    }
    std::cout << "+----------+----------------------+------------+------------+--------------------+\n";
    std::cout << ">> TỔNG SỐ TÀI KHOẢN: " << list.size() << " tài khoản\n";
}

void TablePrinter::printRoomAllocationMap(const std::vector<Room*>& list) {
    if (list.empty()) {
        std::cout << "(Không có phòng nào trong KTX)\n";
        return;
    }
    std::cout << "+--------+----------+--------+--------+-------------------------------------------------------------+\n";
    std::cout << "| PHÒNG  | TÒA NHÀ  | SỨC CHỨ| ĐANG Ở | DANH SÁCH SINH VIÊN ĐANG LƯU TRÚ                            |\n";
    std::cout << "+--------+----------+--------+--------+-------------------------------------------------------------+\n";
    for (const Room* r : list) {
        if (r == nullptr) continue;
        int occupants = static_cast<int>(r->getActiveRent().size());
        std::string stdStr = "";
        for (size_t i = 0; i < r->getActiveRent().size(); ++i) {
            const Rent* rent = r->getActiveRent()[i];
            if (rent != nullptr && rent->getTenant() != nullptr) {
                if (!stdStr.empty()) stdStr += ", ";
                stdStr += rent->getTenant()->getFullName() + " (" + rent->getTenant()->getId() + ")";
            }
        }
        if (stdStr.empty()) {
            stdStr = "(Phòng trống)";
        }
        if (stdStr.length() > 59) {
            stdStr = stdStr.substr(0, 56) + "...";
        }

        std::cout << "| " << std::left << std::setw(6) << r->getId()
                  << " | " << std::setw(8) << r->getBuilding()
                  << " | " << std::setw(6) << r->getMaxStudents()
                  << " | " << std::setw(6) << occupants
                  << " | " << std::setw(59) << stdStr
                  << " |\n";
    }
    std::cout << "+--------+----------+--------+--------+-------------------------------------------------------------+\n";
}

void TablePrinter::printBrokenDevices(const std::string& roomId, const std::vector<BrokenDevice>& list) {
    if (list.empty()) {
        std::cout << "  (Tất cả thiết bị phòng " << roomId << " đều hoạt động tốt, không có thiết bị hỏng)\n";
        return;
    }
    std::cout << "  +--------+----------------------+--------------------------------+------------+----------------+\n";
    std::cout << "  | MÃ TB  | TÊN THIẾT BỊ         | MÔ TẢ HƯ HỎNG                  | NGÀY BÁO   | TRẠNG THÁI     |\n";
    std::cout << "  +--------+----------------------+--------------------------------+------------+----------------+\n";
    for (const auto& dev : list) {
        std::string st = dev.isFixed() ? "Đã sửa xong" : "Đang hỏng";
        std::string nameSub = dev.getDeviceName().substr(0, 20);
        std::string descSub = dev.getDescription().substr(0, 30);

        std::cout << "  | " << std::left << std::setw(6) << dev.getDeviceId()
                  << " | " << std::setw(20) << nameSub
                  << " | " << std::setw(30) << descSub
                  << " | " << std::setw(10) << dev.getReportDate().toString()
                  << " | " << std::setw(14) << st
                  << " |\n";
    }
    std::cout << "  +--------+----------------------+--------------------------------+------------+----------------+\n";
}

void TablePrinter::printRevenueTable(const std::vector<RoomRevenueRow>& rows, const std::string& monthYear) {
    std::cout << "\n===================================================================================================================\n";
    std::cout << "                                  BẢNG DOANH THU KÝ TÚC XÁ THÁNG " << monthYear << "                                  \n";
    std::cout << "===================================================================================================================\n";
    std::cout << "+--------+----------+--------+----------------+--------------+----------------+----------------+--------------------+\n";
    std::cout << "| PHÒNG  | LOẠI     | SỐ NGƯỜI| TIỀN PHÒNG     | PHÍ DỊCH VỤ  | TIỀN ĐIỆN      | TIỀN NƯỚC      | TỔNG DOANH THU     |\n";
    std::cout << "+--------+----------+--------+----------------+--------------+----------------+----------------+--------------------+\n";

    double totalAll = 0.0;
    double totalRentAll = 0.0;
    double totalServiceAll = 0.0;
    double totalElecAll = 0.0;
    double totalWaterAll = 0.0;

    for (const auto& row : rows) {
        totalAll += row.totalRevenue;
        totalRentAll += row.rentFeeTotal;
        totalServiceAll += row.serviceFee;
        totalElecAll += row.electricCost;
        totalWaterAll += row.waterCost;

        std::cout << "| " << std::left << std::setw(6) << row.roomId
                  << " | " << std::setw(8) << row.roomType
                  << " | " << std::setw(6) << row.occupants
                  << " | " << std::setw(14) << formatMoney(row.rentFeeTotal)
                  << " | " << std::setw(12) << formatMoney(row.serviceFee)
                  << " | " << std::setw(14) << formatMoney(row.electricCost)
                  << " | " << std::setw(14) << formatMoney(row.waterCost)
                  << " | " << std::setw(18) << formatMoney(row.totalRevenue)
                  << " |\n";
    }
    std::cout << "+--------+----------+--------+----------------+--------------+----------------+----------------+--------------------+\n";
    std::cout << "| TỔNG CỘNG TOÀN KTX         | "
              << std::setw(14) << formatMoney(totalRentAll) << " | "
              << std::setw(12) << formatMoney(totalServiceAll) << " | "
              << std::setw(14) << formatMoney(totalElecAll) << " | "
              << std::setw(14) << formatMoney(totalWaterAll) << " | "
              << std::setw(18) << formatMoney(totalAll) << " |\n";
    std::cout << "+----------------------------+----------------+--------------+----------------+----------------+--------------------+\n";
    std::cout << ">> TỔNG DOANH THU KÝ TÚC XÁ THÁNG " << monthYear << ": " << formatMoney(totalAll) << "\n";
}

void TablePrinter::printSingleRoomRevenue(const RoomRevenueRow& row, const std::string& monthYear) {
    std::cout << "\n====================================================================\n";
    std::cout << "            PHIẾU TÍNH DOANH THU CHI TIẾT PHÒNG " << row.roomId << " (THÁNG " << monthYear << ")\n";
    std::cout << "====================================================================\n";
    std::cout << " Mã phòng           : " << row.roomId << " (Loại: " << row.roomType << ")\n";
    std::cout << " Số người đang ở    : " << row.occupants << "/" << row.maxCapacity << " sinh viên\n";
    std::cout << " Giá thuê cơ bản    : " << formatMoney(row.basePrice) << " / tháng\n";
    if (row.roomType == "VIP") {
        std::cout << " Phí dịch vụ VIP    : " << formatMoney(row.serviceFee) << " / tháng\n";
    }
    std::cout << "--------------------------------------------------------------------\n";
    std::cout << " 1. Tiền phòng từ hợp đồng  : " << formatMoney(row.rentFeeTotal) << "\n";
    if (row.roomType == "VIP") {
        std::cout << " 2. Phí dịch vụ cộng thêm   : " << formatMoney(row.serviceFee) << "\n";
    }
    std::cout << " 3. Tiền điện sinh hoạt     : " << formatMoney(row.electricCost) << "\n";
    std::cout << " 4. Tiền nước sinh hoạt     : " << formatMoney(row.waterCost) << "\n";
    std::cout << "--------------------------------------------------------------------\n";
    std::cout << " >> TỔNG DOANH THU CỦA PHÒNG: " << formatMoney(row.totalRevenue) << "\n";
    if (row.occupants > 0) {
        double avg = row.totalRevenue / row.occupants;
        std::cout << " >> Bình quân mỗi bạn nộp   : " << formatMoney(avg) << " / người\n";
    } else {
        std::cout << " (Phòng hiện đang trống - Chưa phát sinh thu từ sinh viên)\n";
    }
    std::cout << "====================================================================\n";
}
