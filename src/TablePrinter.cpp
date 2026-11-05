#include "TablePrinter.h"
#include "Human.h"
#include "Room.h"
#include "Rent.h"
#include "Account.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// =========================================================================
// 1. TIỆN ÍCH TÍNH TOÁN ĐỘ RỘNG VISUAL VÀ OFFSET UTF-8 TIẾNG VIỆT
// =========================================================================

namespace StringUtils {
    int utf8Width(const std::string& str) {
        int w = 0;
        for (size_t i = 0; i < str.length(); ++i) {
            unsigned char c = static_cast<unsigned char>(str[i]);
            // Mỗi ký tự UTF-8 chỉ có 1 byte mở đầu (khác byte tiếp nối 10xxxxxx)
            if ((c & 0xC0) != 0x80) {
                w++;
            }
        }
        return w;
    }

    int utf8Offset(const std::string& str) {
        return static_cast<int>(str.length()) - utf8Width(str);
    }

    std::string padRight(const std::string& str, int visualWidth) {
        int w = utf8Width(str);
        if (w >= visualWidth) return str;
        return str + std::string(visualWidth - w, ' ');
    }

    std::string padLeft(const std::string& str, int visualWidth) {
        int w = utf8Width(str);
        if (w >= visualWidth) return str;
        return std::string(visualWidth - w, ' ') + str;
    }

    std::string padCenter(const std::string& str, int visualWidth) {
        int w = utf8Width(str);
        if (w >= visualWidth) return str;
        int left = (visualWidth - w) / 2;
        int right = visualWidth - w - left;
        return std::string(left, ' ') + str + std::string(right, ' ');
    }

    std::string repeat(const std::string& pattern, int count) {
        std::string res;
        for (int i = 0; i < count; ++i) res += pattern;
        return res;
    }

    std::string formatMoney(double amount) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << amount << " VND";
        return oss.str();
    }
}

using StringUtils::formatMoney;

// =========================================================================
// 2. CÁC HÀM IN BẢNG DỮ LIỆU ĐƯỢC CANH CHỈNH OFFSET CHUẨN XÁC
// =========================================================================

void TablePrinter::printStudents(const std::vector<Student*>& list) {
    if (list.empty()) {
        std::cout << "(Không có sinh viên nào)\n";
        return;
    }
    std::cout << "╔════════╦══════════════════════╦════════╦════════╦═════════════╦═══════╦════════════════════════╦════════════╦═════════════════════════╦════════╗\n";
    std::cout << "║ " << StringUtils::padRight("MÃ SV", 6)
              << " ║ " << StringUtils::padRight("HỌ VÀ TÊN", 20)
              << " ║ " << StringUtils::padRight("GIỚI", 6)
              << " ║ " << StringUtils::padRight("NĂM", 6)
              << " ║ " << StringUtils::padRight("LỚP", 11)
              << " ║ " << StringUtils::padRight("KHÓA", 5)
              << " ║ " << StringUtils::padRight("KHOA", 22)
              << " ║ " << StringUtils::padRight("SĐT", 10)
              << " ║ " << StringUtils::padRight("EMAIL", 23)
              << " ║ " << StringUtils::padRight("PHÒNG", 6) << " ║\n";
    std::cout << "╠════════╬══════════════════════╬════════╬════════╬═════════════╬═══════╬════════════════════════╬════════════╬═════════════════════════╬════════╣\n";
    for (const Student* s : list) {
        if (s == nullptr) continue;
        std::string genderStr = (s->getGender() == Gender::MALE) ? "Nam" : "Nu";
        std::string roomStr = (s->getRent() != nullptr && s->getRent()->isActive() && s->getRent()->getRoom() != nullptr)
                              ? s->getRent()->getRoom()->getId() : "(Chưa)";
        std::string nameSub = s->getFullName();
        if (StringUtils::utf8Width(nameSub) > 20) nameSub = nameSub.substr(0, 17) + "...";
        std::string classSub = s->getClass();
        if (StringUtils::utf8Width(classSub) > 11) classSub = classSub.substr(0, 11);
        std::string facSub = s->getFaculty();
        if (StringUtils::utf8Width(facSub) > 22) facSub = facSub.substr(0, 19) + "...";
        std::string emSub = s->getEmail();
        if (StringUtils::utf8Width(emSub) > 23) emSub = emSub.substr(0, 20) + "...";

        std::cout << "║ " << StringUtils::padRight(s->getId(), 6)
                  << " ║ " << StringUtils::padRight(nameSub, 20)
                  << " ║ " << StringUtils::padRight(genderStr, 6)
                  << " ║ " << StringUtils::padRight(s->getBirthYear(), 6)
                  << " ║ " << StringUtils::padRight(classSub, 11)
                  << " ║ " << StringUtils::padRight(s->getCohort(), 5)
                  << " ║ " << StringUtils::padRight(facSub, 22)
                  << " ║ " << StringUtils::padRight(s->getPhone(), 10)
                  << " ║ " << StringUtils::padRight(emSub, 23)
                  << " ║ " << StringUtils::padRight(roomStr, 6)
                  << " ║\n";
    }
    std::cout << "╚════════╩══════════════════════╩════════╩════════╩═════════════╩═══════╩════════════════════════╩════════════╩═════════════════════════╩════════╝\n";
    std::cout << ">> TỔNG SỐ SINH VIÊN: " << list.size() << " sinh viên\n";
}

void TablePrinter::printRooms(const std::vector<Room*>& list) {
    if (list.empty()) {
        std::cout << "(Không có phòng nào)\n";
        return;
    }
    std::cout << "╔════════╦════════════╦══════╦══════════╦══════════╦════════╦═══════════╦═══════════════╦════════════╦════════════════════╗\n";
    std::cout << "║ " << StringUtils::padRight("PHÒNG", 6)
              << " ║ " << StringUtils::padRight("TÒA NHÀ", 10)
              << " ║ " << StringUtils::padRight("TẦNG", 4)
              << " ║ " << StringUtils::padRight("LOẠI", 8)
              << " ║ " << StringUtils::padRight("SỨC CHỨA", 8)
              << " ║ " << StringUtils::padRight("ĐANG Ở", 6)
              << " ║ " << StringUtils::padRight("CÒN TRỐNG", 9)
              << " ║ " << StringUtils::padRight("GIÁ PHÒNG", 13)
              << " ║ " << StringUtils::padRight("TRẠNG THÁI", 10)
              << " ║ " << StringUtils::padRight("THIẾT BỊ HỎNG", 18) << " ║\n";
    std::cout << "╠════════╬════════════╬══════╬══════════╬══════════╬════════╬═══════════╬═══════════════╬════════════╬════════════════════╣\n";
    for (const Room* r : list) {
        if (r == nullptr) continue;
        int occupants = static_cast<int>(r->getActiveRent().size());
        int freeSlots = r->getMaxStudents() - occupants;
        int brokenCount = r->countUnfixedDevices();
        std::string brokenStr = (brokenCount > 0) ? ("Có " + std::to_string(brokenCount) + " thiết bị") : "Hoạt động tốt";

        std::cout << "║ " << StringUtils::padRight(r->getId(), 6)
                  << " ║ " << StringUtils::padRight(r->getBuilding(), 10)
                  << " ║ " << StringUtils::padRight(std::to_string(r->getFloor()), 4)
                  << " ║ " << StringUtils::padRight(r->getRoomType(), 8)
                  << " ║ " << StringUtils::padRight(std::to_string(r->getMaxStudents()), 8)
                  << " ║ " << StringUtils::padRight(std::to_string(occupants), 6)
                  << " ║ " << StringUtils::padRight(std::to_string(freeSlots), 9)
                  << " ║ " << StringUtils::padRight(formatMoney(r->getBasePrice()), 13)
                  << " ║ " << StringUtils::padRight(EnumHelper::roomStatusToString(r->getStatus()), 10)
                  << " ║ " << StringUtils::padRight(brokenStr, 18)
                  << " ║\n";
    }
    std::cout << "╚════════╩════════════╩══════╩══════════╩══════════╩════════╩═══════════╩═══════════════╩════════════╩════════════════════╝\n";
    std::cout << ">> TỔNG SỐ PHÒNG: " << list.size() << " phòng\n";
}

void TablePrinter::printRents(const std::vector<Rent*>& list) {
    if (list.empty()) {
        std::cout << "(Không có hợp đồng nào)\n";
        return;
    }
    std::cout << "╔════════╦════════╦══════════════════════╦════════╦════════════╦════════════╦══════════╦═══════════════╦════════════╗\n";
    std::cout << "║ " << StringUtils::padRight("MÃ HĐ", 6)
              << " ║ " << StringUtils::padRight("MÃ SV", 6)
              << " ║ " << StringUtils::padRight("TÊN SINH VIÊN", 20)
              << " ║ " << StringUtils::padRight("PHÒNG", 6)
              << " ║ " << StringUtils::padRight("NGÀY BĐ", 10)
              << " ║ " << StringUtils::padRight("NGÀY KT", 10)
              << " ║ " << StringUtils::padRight("THỜI HẠN", 8)
              << " ║ " << StringUtils::padRight("GIÁ THUÊ/THÁNG", 13)
              << " ║ " << StringUtils::padRight("TRẠNG THÁI", 10) << " ║\n";
    std::cout << "╠════════╬════════╬══════════════════════╬════════╬════════════╬════════════╬══════════╬═══════════════╬════════════╣\n";
    for (const Rent* r : list) {
        if (r == nullptr) continue;
        std::string sId = r->getTenant() ? r->getTenant()->getId() : "N/A";
        std::string sName = r->getTenant() ? r->getTenant()->getFullName() : "N/A";
        if (StringUtils::utf8Width(sName) > 20) sName = sName.substr(0, 17) + "...";
        std::string rId = r->getRoom() ? r->getRoom()->getId() : "N/A";

        std::cout << "║ " << StringUtils::padRight(r->getId(), 6)
                  << " ║ " << StringUtils::padRight(sId, 6)
                  << " ║ " << StringUtils::padRight(sName, 20)
                  << " ║ " << StringUtils::padRight(rId, 6)
                  << " ║ " << StringUtils::padRight(r->getStartDate().toString(), 10)
                  << " ║ " << StringUtils::padRight(r->getActualEndDate().toString(), 10)
                  << " ║ " << StringUtils::padRight(EnumHelper::periodToString(r->getRentalPeriod()), 8)
                  << " ║ " << StringUtils::padRight(formatMoney(r->getRentFee()), 13)
                  << " ║ " << StringUtils::padRight(EnumHelper::rentStatusToString(r->getStatus()), 10)
                  << " ║\n";
    }
    std::cout << "╚════════╩════════╩══════════════════════╩════════╩════════════╩════════════╩══════════╩═══════════════╩════════════╝\n";
    std::cout << ">> TỔNG SỐ HỢP ĐỒNG: " << list.size() << " hợp đồng\n";
}

void TablePrinter::printStaffs(const std::vector<Staff*>& list) {
    if (list.empty()) {
        std::cout << "(Không có cán bộ nào)\n";
        return;
    }
    std::cout << "╔════════╦══════════════════════╦════════╦════════════════════════╦═════════════════════════╦════════════╗\n";
    std::cout << "║ " << StringUtils::padRight("MÃ CB", 6)
              << " ║ " << StringUtils::padRight("HỌ VÀ TÊN", 20)
              << " ║ " << StringUtils::padRight("GIỚI", 6)
              << " ║ " << StringUtils::padRight("PHÒNG BAN", 22)
              << " ║ " << StringUtils::padRight("CHỨC VỤ", 23)
              << " ║ " << StringUtils::padRight("SĐT", 10) << " ║\n";
    std::cout << "╠════════╬══════════════════════╬════════╬════════════════════════╬═════════════════════════╬════════════╣\n";
    for (const Staff* st : list) {
        if (st == nullptr) continue;
        std::string genderStr = (st->getGender() == Gender::MALE) ? "Nam" : "Nu";
        std::string nameSub = st->getFullName();
        if (StringUtils::utf8Width(nameSub) > 20) nameSub = nameSub.substr(0, 17) + "...";
        std::string deptSub = st->getDepartment();
        if (StringUtils::utf8Width(deptSub) > 22) deptSub = deptSub.substr(0, 19) + "...";
        std::string posSub = st->getPosition();
        if (StringUtils::utf8Width(posSub) > 23) posSub = posSub.substr(0, 20) + "...";

        std::cout << "║ " << StringUtils::padRight(st->getId(), 6)
                  << " ║ " << StringUtils::padRight(nameSub, 20)
                  << " ║ " << StringUtils::padRight(genderStr, 6)
                  << " ║ " << StringUtils::padRight(deptSub, 22)
                  << " ║ " << StringUtils::padRight(posSub, 23)
                  << " ║ " << StringUtils::padRight(st->getPhone(), 10)
                  << " ║\n";
    }
    std::cout << "╚════════╩══════════════════════╩════════╩════════════════════════╩═════════════════════════╩════════════╝\n";
    std::cout << ">> TỔNG SỐ CÁN BỘ: " << list.size() << " người\n";
}

void TablePrinter::printAccounts(const std::vector<Account*>& list) {
    if (list.empty()) {
        std::cout << "(Không có tài khoản nào)\n";
        return;
    }
    std::cout << "╔══════════╦══════════════════════╦════════════╦════════════╦════════════════════╗\n";
    std::cout << "║ " << StringUtils::padRight("MÃ TK", 8)
              << " ║ " << StringUtils::padRight("TÊN ĐĂNG NHẬP", 20)
              << " ║ " << StringUtils::padRight("VAI TRÒ", 10)
              << " ║ " << StringUtils::padRight("MÃ LIÊN KẾT", 10)
              << " ║ " << StringUtils::padRight("TRẠNG THÁI", 18) << " ║\n";
    std::cout << "╠══════════╬══════════════════════╬════════════╬════════════╬════════════════════╣\n";
    for (const Account* a : list) {
        if (a == nullptr) continue;
        std::string stStr = a->getStatus() ? "KÍCH HOẠT" : "BỊ KHÓA";

        std::cout << "║ " << StringUtils::padRight(a->getId(), 8)
                  << " ║ " << StringUtils::padRight(a->getUsername(), 20)
                  << " ║ " << StringUtils::padRight(EnumHelper::roleToString(a->getRole()), 10)
                  << " ║ " << StringUtils::padRight(a->getOwnerId(), 10)
                  << " ║ " << StringUtils::padRight(stStr, 18)
                  << " ║\n";
    }
    std::cout << "╚══════════╩══════════════════════╩════════════╩════════════╩════════════════════╝\n";
    std::cout << ">> TỔNG SỐ TÀI KHOẢN: " << list.size() << " tài khoản\n";
}

void TablePrinter::printRoomAllocationMap(const std::vector<Room*>& list) {
    if (list.empty()) {
        std::cout << "(Không có phòng nào trong KTX)\n";
        return;
    }
    std::cout << "╔════════╦══════════╦════════╦════════╦═════════════════════════════════════════════════════════════╗\n";
    std::cout << "║ " << StringUtils::padRight("PHÒNG", 6)
              << " ║ " << StringUtils::padRight("TÒA NHÀ", 8)
              << " ║ " << StringUtils::padRight("SỨC CHỨA", 6)
              << " ║ " << StringUtils::padRight("ĐANG Ở", 6)
              << " ║ " << StringUtils::padRight("DANH SÁCH SINH VIÊN ĐANG LƯU TRÚ", 59) << " ║\n";
    std::cout << "╠════════╬══════════╬════════╬════════╬═════════════════════════════════════════════════════════════╣\n";
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
        if (StringUtils::utf8Width(stdStr) > 59) {
            stdStr = stdStr.substr(0, 56) + "...";
        }

        std::cout << "║ " << StringUtils::padRight(r->getId(), 6)
                  << " ║ " << StringUtils::padRight(r->getBuilding(), 8)
                  << " ║ " << StringUtils::padRight(std::to_string(r->getMaxStudents()), 6)
                  << " ║ " << StringUtils::padRight(std::to_string(occupants), 6)
                  << " ║ " << StringUtils::padRight(stdStr, 59)
                  << " ║\n";
    }
    std::cout << "╚════════╩══════════╩════════╩════════╩═════════════════════════════════════════════════════════════╝\n";
}

void TablePrinter::printBrokenDevices(const std::string& roomId, const std::vector<BrokenDevice>& list) {
    if (list.empty()) {
        std::cout << "  (Tất cả thiết bị phòng " << roomId << " đều hoạt động tốt, không có thiết bị hỏng)\n";
        return;
    }
    std::cout << "  ╔════════╦══════════════════════╦════════════════════════════════╦════════════╦════════════════╗\n";
    std::cout << "  ║ " << StringUtils::padRight("MÃ TB", 6)
              << " ║ " << StringUtils::padRight("TÊN THIẾT BỊ", 20)
              << " ║ " << StringUtils::padRight("MÔ TẢ HƯ HỎNG", 30)
              << " ║ " << StringUtils::padRight("NGÀY BÁO", 10)
              << " ║ " << StringUtils::padRight("TRẠNG THÁI", 14) << " ║\n";
    std::cout << "  ╠════════╬══════════════════════╬════════════════════════════════╬════════════╬════════════════╣\n";
    for (const auto& dev : list) {
        std::string st = dev.isFixed() ? "Đã sửa xong" : "Đang hỏng";
        std::string nameSub = dev.getDeviceName();
        if (StringUtils::utf8Width(nameSub) > 20) nameSub = nameSub.substr(0, 17) + "...";
        std::string descSub = dev.getDescription();
        if (StringUtils::utf8Width(descSub) > 30) descSub = descSub.substr(0, 27) + "...";

        std::cout << "  ║ " << StringUtils::padRight(dev.getDeviceId(), 6)
                  << " ║ " << StringUtils::padRight(nameSub, 20)
                  << " ║ " << StringUtils::padRight(descSub, 30)
                  << " ║ " << StringUtils::padRight(dev.getReportDate().toString(), 10)
                  << " ║ " << StringUtils::padRight(st, 14)
                  << " ║\n";
    }
    std::cout << "  ╚════════╩══════════════════════╩════════════════════════════════╩════════════╩════════════════╝\n";
}

// Bảng doanh thu toàn KTX: Tổng độ rộng = 119 ký tự
void TablePrinter::printRevenueTable(const std::vector<RoomRevenueRow>& rows, const std::string& monthYear) {
    int totalWidth = 119; // 8 + 1 + 10 + 1 + 11 + 1 + 16 + 1 + 15 + 1 + 16 + 1 + 16 + 1 + 20 = 119

    std::cout << "\n╔" << StringUtils::repeat("═", totalWidth) << "╗\n";
    std::cout << "║" << StringUtils::padCenter("BẢNG DOANH THU KÝ TÚC XÁ THÁNG " + monthYear, totalWidth) << "║\n";
    std::cout << "╠════════╦══════════╦═══════════╦════════════════╦═══════════════╦════════════════╦════════════════╦════════════════════╣\n";
    std::cout << "║ " << StringUtils::padRight("PHÒNG", 6)
              << " ║ " << StringUtils::padRight("LOẠI", 8)
              << " ║ " << StringUtils::padRight("SỐ NGƯỜI", 9)
              << " ║ " << StringUtils::padRight("TIỀN PHÒNG", 14)
              << " ║ " << StringUtils::padRight("PHÍ DỊCH VỤ", 13)
              << " ║ " << StringUtils::padRight("TIỀN ĐIỆN", 14)
              << " ║ " << StringUtils::padRight("TIỀN NƯỚC", 14)
              << " ║ " << StringUtils::padRight("TỔNG DOANH THU", 18) << " ║\n";
    std::cout << "╠════════╬══════════╬═══════════╬════════════════╬═══════════════╬════════════════╬════════════════╬════════════════════╣\n";

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

        std::cout << "║ " << StringUtils::padRight(row.roomId, 6)
                  << " ║ " << StringUtils::padRight(row.roomType, 8)
                  << " ║ " << StringUtils::padRight(std::to_string(row.occupants), 9)
                  << " ║ " << StringUtils::padRight(formatMoney(row.rentFeeTotal), 14)
                  << " ║ " << StringUtils::padRight(formatMoney(row.serviceFee), 13)
                  << " ║ " << StringUtils::padRight(formatMoney(row.electricCost), 14)
                  << " ║ " << StringUtils::padRight(formatMoney(row.waterCost), 14)
                  << " ║ " << StringUtils::padRight(formatMoney(row.totalRevenue), 18)
                  << " ║\n";
    }
    // Hàng tổng kết: 3 cột đầu gộp lại = 8 + 1 + 10 + 1 + 11 = 31 ký tự
    std::cout << "╠════════╩══════════╩═══════════╬════════════════╬═══════════════╬════════════════╬════════════════╬════════════════════╣\n";
    std::cout << "║ " << StringUtils::padRight("TỔNG CỘNG TOÀN KTX", 29)
              << " ║ " << StringUtils::padRight(formatMoney(totalRentAll), 14)
              << " ║ " << StringUtils::padRight(formatMoney(totalServiceAll), 13)
              << " ║ " << StringUtils::padRight(formatMoney(totalElecAll), 14)
              << " ║ " << StringUtils::padRight(formatMoney(totalWaterAll), 14)
              << " ║ " << StringUtils::padRight(formatMoney(totalAll), 18) << " ║\n";
    std::cout << "╚═══════════════════════════════╩════════════════╩═══════════════╩════════════════╩════════════════╩════════════════════╝\n";
    std::cout << ">> TỔNG DOANH THU KÝ TÚC XÁ THÁNG " << monthYear << ": " << formatMoney(totalAll) << "\n";
}

void TablePrinter::printSingleRoomRevenue(const RoomRevenueRow& row, const std::string& monthYear) {
    int innerWidth = 72;
    std::cout << "\n╔" << StringUtils::repeat("═", innerWidth) << "╗\n";
    std::cout << "║" << StringUtils::padCenter("PHIẾU TÍNH DOANH THU CHI TIẾT PHÒNG " + row.roomId + " (THÁNG " + monthYear + ")", innerWidth) << "║\n";
    std::cout << "╠" << StringUtils::repeat("═", innerWidth) << "╣\n";
    std::cout << "║ " << StringUtils::padRight("Mã phòng", 28) << ": " << StringUtils::padRight(row.roomId + " (Loại: " + row.roomType + ")", 40) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("Số người đang ở", 28) << ": " << StringUtils::padRight(std::to_string(row.occupants) + "/" + std::to_string(row.maxCapacity) + " sinh viên", 40) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("Giá thuê cơ bản", 28) << ": " << StringUtils::padRight(formatMoney(row.basePrice) + " / tháng", 40) << " ║\n";
    if (row.roomType == "VIP") {
        std::cout << "║ " << StringUtils::padRight("Phí dịch vụ VIP", 28) << ": " << StringUtils::padRight(formatMoney(row.serviceFee) + " / tháng", 40) << " ║\n";
    }
    std::cout << "╠" << StringUtils::repeat("═", innerWidth) << "╣\n";
    std::cout << "║ " << StringUtils::padRight("1. Tiền phòng từ hợp đồng", 28) << ": " << StringUtils::padRight(formatMoney(row.rentFeeTotal), 40) << " ║\n";
    if (row.roomType == "VIP") {
        std::cout << "║ " << StringUtils::padRight("2. Phí dịch vụ cộng thêm", 28) << ": " << StringUtils::padRight(formatMoney(row.serviceFee), 40) << " ║\n";
    }
    std::cout << "║ " << StringUtils::padRight("3. Tiền điện sinh hoạt", 28) << ": " << StringUtils::padRight(formatMoney(row.electricCost), 40) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("4. Tiền nước sinh hoạt", 28) << ": " << StringUtils::padRight(formatMoney(row.waterCost), 40) << " ║\n";
    std::cout << "╠" << StringUtils::repeat("═", innerWidth) << "╣\n";
    std::cout << "║ " << StringUtils::padRight(">> TỔNG DOANH THU CỦA PHÒNG", 28) << ": " << StringUtils::padRight(formatMoney(row.totalRevenue), 40) << " ║\n";
    if (row.occupants > 0) {
        double avg = row.totalRevenue / row.occupants;
        std::cout << "║ " << StringUtils::padRight(">> Bình quân mỗi bạn nộp", 28) << ": " << StringUtils::padRight(formatMoney(avg) + " / người", 40) << " ║\n";
    } else {
        std::cout << "║ " << StringUtils::padCenter("(Phòng hiện đang trống - Chưa phát sinh thu từ sinh viên)", 70) << " ║\n";
    }
    std::cout << "╚" << StringUtils::repeat("═", innerWidth) << "╝\n";
}

// Báo cáo chi tiết doanh thu tiền phòng hàng tháng theo từng hợp đồng & phân bổ loại phòng
void TablePrinter::printDetailedMonthlyRevenueReport(const std::vector<Rent*>& rents, const std::vector<Room*>& rooms) {
    int totalWidth = 102;

    std::cout << "\n╔" << StringUtils::repeat("═", totalWidth) << "╗\n";
    std::cout << "║" << StringUtils::padCenter("BÁO CÁO DOANH THU TIỀN PHÒNG HÀNG THÁNG (BẢN CHI TIẾT)", totalWidth) << "║\n";
    std::cout << "╚" << StringUtils::repeat("═", totalWidth) << "╝\n";

    // Lọc các hợp đồng đang hiệu lực
    std::vector<Rent*> activeRents;
    double totalRevenue = 0.0;
    double standardRev = 0.0;
    double vipRev = 0.0;
    int stdStudents = 0;
    int vipStudents = 0;

    for (Rent* r : rents) {
        if (r != nullptr && r->isActive()) {
            activeRents.push_back(r);
            totalRevenue += r->getRentFee();
            if (r->getRoom() != nullptr && r->getRoom()->getRoomType() == "VIP") {
                vipRev += r->getRentFee();
                vipStudents++;
            } else {
                standardRev += r->getRentFee();
                stdStudents++;
            }
        }
    }

    std::cout << "\n--- PHẦN 1: BẢNG CHI TIẾT TỪNG HỢP ĐỒNG ĐANG PHÁT SINH DOANH THU ---\n";
    if (activeRents.empty()) {
        std::cout << "(Hiện tại chưa có hợp đồng nào đang hiệu lực phát sinh doanh thu)\n";
    } else {
        std::cout << "╔═════╦════════╦══════════╦════════════════════════╦════════╦══════════╦════════════════════╦══════════╗\n";
        std::cout << "║ " << StringUtils::padRight("STT", 3)
                  << " ║ " << StringUtils::padRight("MÃ HĐ", 6)
                  << " ║ " << StringUtils::padRight("MÃ SV", 8)
                  << " ║ " << StringUtils::padRight("HỌ VÀ TÊN SINH VIÊN", 22)
                  << " ║ " << StringUtils::padRight("PHÒNG", 6)
                  << " ║ " << StringUtils::padRight("LOẠI", 8)
                  << " ║ " << StringUtils::padRight("TIỀN THUÊ / THÁNG", 18)
                  << " ║ " << StringUtils::padRight("KỲ HẠN", 8) << " ║\n";
        std::cout << "╠═════╬════════╬══════════╬════════════════════════╬════════╬══════════╬════════════════════╬══════════╣\n";

        for (size_t i = 0; i < activeRents.size(); ++i) {
            Rent* r = activeRents[i];
            std::string sId = r->getTenant() ? r->getTenant()->getId() : "N/A";
            std::string sName = r->getTenant() ? r->getTenant()->getFullName() : "N/A";
            if (StringUtils::utf8Width(sName) > 22) sName = sName.substr(0, 19) + "...";
            std::string rId = r->getRoom() ? r->getRoom()->getId() : "N/A";
            std::string rType = r->getRoom() ? r->getRoom()->getRoomType() : "STANDARD";
            std::string periodStr = EnumHelper::periodToString(r->getRentalPeriod());

            std::cout << "║ " << StringUtils::padRight(std::to_string(i + 1), 3)
                      << " ║ " << StringUtils::padRight(r->getId(), 6)
                      << " ║ " << StringUtils::padRight(sId, 8)
                      << " ║ " << StringUtils::padRight(sName, 22)
                      << " ║ " << StringUtils::padRight(rId, 6)
                      << " ║ " << StringUtils::padRight(rType, 8)
                      << " ║ " << StringUtils::padRight(formatMoney(r->getRentFee()), 18)
                      << " ║ " << StringUtils::padRight(periodStr, 8) << " ║\n";
        }
        std::cout << "╚═════╩════════╩══════════╩════════════════════════╩════════╩══════════╩════════════════════╩══════════╝\n";
    }

    std::cout << "\n--- PHẦN 2: CƠ CẤU DOANH THU THEO LOẠI PHÒNG (STANDARD vs VIP) ---\n";
    int totalRooms = static_cast<int>(rooms.size());
    int stdRooms = 0, vipRooms = 0;
    for (const Room* rm : rooms) {
        if (rm != nullptr) {
            if (rm->getRoomType() == "VIP") vipRooms++;
            else stdRooms++;
        }
    }
    double stdPct = totalRevenue > 0 ? (standardRev * 100.0 / totalRevenue) : 0.0;
    double vipPct = totalRevenue > 0 ? (vipRev * 100.0 / totalRevenue) : 0.0;

    std::cout << "╔════════════╦══════════╦══════════════╦════════════════════╦════════════════════╗\n";
    std::cout << "║ " << StringUtils::padRight("LOẠI PHÒNG", 10)
              << " ║ " << StringUtils::padRight("SỐ PHÒNG", 8)
              << " ║ " << StringUtils::padRight("SỐ SINH VIÊN", 12)
              << " ║ " << StringUtils::padRight("DOANH THU / THÁNG", 18)
              << " ║ " << StringUtils::padRight("TỶ TRỌNG (%)", 18) << " ║\n";
    std::cout << "╠════════════╬══════════╬══════════════╬════════════════════╬════════════════════╣\n";
    std::cout << "║ " << StringUtils::padRight("STANDARD", 10)
              << " ║ " << StringUtils::padRight(std::to_string(stdRooms), 8)
              << " ║ " << StringUtils::padRight(std::to_string(stdStudents) + " sinh viên", 12)
              << " ║ " << StringUtils::padRight(formatMoney(standardRev), 18)
              << " ║ " << StringUtils::padRight(std::to_string(static_cast<int>(stdPct + 0.5)) + " %", 18) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("VIP", 10)
              << " ║ " << StringUtils::padRight(std::to_string(vipRooms), 8)
              << " ║ " << StringUtils::padRight(std::to_string(vipStudents) + " sinh viên", 12)
              << " ║ " << StringUtils::padRight(formatMoney(vipRev), 18)
              << " ║ " << StringUtils::padRight(std::to_string(static_cast<int>(vipPct + 0.5)) + " %", 18) << " ║\n";
    std::cout << "╠════════════╩══════════╩══════════════╬════════════════════╬════════════════════╣\n";
    std::cout << "║ " << StringUtils::padRight("TỔNG CỘNG DOANH THU THUÊ PHÒNG", 36)
              << " ║ " << StringUtils::padRight(formatMoney(totalRevenue), 18)
              << " ║ " << StringUtils::padRight("100 %", 18) << " ║\n";
    std::cout << "╚══════════════════════════════════════╩════════════════════╩════════════════════╝\n";

    std::cout << "\n--- PHẦN 3: TỔNG HỢP CÁC CHỈ SỐ TÀI CHÍNH KÝ TÚC XÁ ---\n";
    int totalOccupants = static_cast<int>(activeRents.size());
    double avgPerStudent = totalOccupants > 0 ? (totalRevenue / totalOccupants) : 0.0;
    double avgPerRoom = totalRooms > 0 ? (totalRevenue / totalRooms) : 0.0;
    double yearlyProjected = totalRevenue * 12.0;

    int cardWidth = 76;
    std::cout << "╔" << StringUtils::repeat("═", cardWidth) << "╗\n";
    std::cout << "║" << StringUtils::padCenter("TỔNG KẾT TÀI CHÍNH TOÀN KÝ TÚC XÁ", cardWidth) << "║\n";
    std::cout << "╠" << StringUtils::repeat("═", cardWidth) << "╣\n";
    std::cout << "║ " << StringUtils::padRight("1. Tổng doanh thu tiền phòng thực thu", 44) << ": " << StringUtils::padRight(formatMoney(totalRevenue) + " / tháng", 28) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("2. Dự kiến doanh thu cả năm học (12 tháng)", 44) << ": " << StringUtils::padRight(formatMoney(yearlyProjected) + " / năm", 28) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("3. Tổng số hợp đồng hiệu lực đang thu tiền", 44) << ": " << StringUtils::padRight(std::to_string(totalOccupants) + " hợp đồng", 28) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("4. Doanh thu bình quân trên mỗi sinh viên", 44) << ": " << StringUtils::padRight(formatMoney(avgPerStudent) + " / SV / tháng", 28) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("5. Doanh thu bình quân trên mỗi phòng KTX", 44) << ": " << StringUtils::padRight(formatMoney(avgPerRoom) + " / phòng / tháng", 28) << " ║\n";
    std::cout << "╚" << StringUtils::repeat("═", cardWidth) << "╝\n";
}
