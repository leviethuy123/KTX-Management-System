#ifndef TABLE_PRINTER_H
#define TABLE_PRINTER_H

#include "Common.h"
#include <vector>
#include <string>

class Student;
class Room;
class Rent;
class Staff;
class Account;
class BrokenDevice;

/**
 * @brief Tiện ích tính toán độ rộng visual và offset chuỗi UTF-8 tiếng Việt
 */
namespace StringUtils {
    int utf8Width(const std::string& str);
    int utf8Offset(const std::string& str);
    std::string padRight(const std::string& str, int visualWidth);
    std::string padLeft(const std::string& str, int visualWidth);
    std::string padCenter(const std::string& str, int visualWidth);
    std::string repeat(const std::string& pattern, int count);
    std::string formatMoney(double amount);
}

/**
 * @brief Dòng dữ liệu thống kê doanh thu phòng trong một tháng
 */
struct RoomRevenueRow {
    std::string roomId;
    std::string roomType;
    int occupants;
    int maxCapacity;
    double basePrice;
    double serviceFee;
    double rentFeeTotal;
    double electricCost;
    double waterCost;
    double totalRevenue;
};

/**
 * @brief Lớp tiện ích in dữ liệu hệ thống dạng BẢNG ASCII chuyên nghiệp
 */
class TablePrinter {
public:
    static void printStudents(const std::vector<Student*>& list);
    static void printRooms(const std::vector<Room*>& list);
    static void printRents(const std::vector<Rent*>& list);
    static void printStaffs(const std::vector<Staff*>& list);
    static void printAccounts(const std::vector<Account*>& list);
    static void printRoomAllocationMap(const std::vector<Room*>& list);
    static void printBrokenDevices(const std::string& roomId, const std::vector<BrokenDevice>& list);
    static void printRevenueTable(const std::vector<RoomRevenueRow>& rows, const std::string& monthYear);
    static void printSingleRoomRevenue(const RoomRevenueRow& row, const std::string& monthYear);
    static void printDetailedMonthlyRevenueReport(const std::vector<Rent*>& rents, const std::vector<Room*>& rooms);
};

#endif // TABLE_PRINTER_H
