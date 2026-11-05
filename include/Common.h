#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <nlohmann/json.hpp>

// =========================================================================
// 1. CÁC KIỂU LIỆT KÊ (ENUMS) & TIỆN ÍCH CHUYỂN ĐỔI CHUỖI
// =========================================================================

enum class Gender {
    MALE,
    FEMALE,
    OTHER
};

enum class Role {
    STUDENT,
    STAFF,
    ADMIN
};

enum class RoomStatus {
    EMPTY,       // Còn trống
    OCCUPIED,    // Đang có người ở
    FULL,        // Đã đầy chỗ
    MAINTENANCE  // Đang bảo trì
};

enum class RentStatus {
    ACTIVE,      // Đang hiệu lực
    EXPIRED,     // Hết hạn
    TERMINATED   // Chấm dứt sớm
};

enum class Period {
    ONE_MONTH = 1,
    THREE_MONTHS = 3,
    SIX_MONTHS = 6,
    ONE_YEAR = 12
};

namespace EnumHelper {
    inline std::string genderToString(Gender g) {
        switch (g) {
            case Gender::MALE: return "Nam";
            case Gender::FEMALE: return "Nu";
            default: return "Khac";
        }
    }

    inline Gender stringToGender(const std::string& str) {
        if (str == "Nam" || str == "MALE" || str == "1") return Gender::MALE;
        if (str == "Nu" || str == "FEMALE" || str == "0" || str == "2") return Gender::FEMALE;
        if (str == "Khac" || str == "OTHER" || str == "3") return Gender::OTHER;
        throw std::invalid_argument("Giới tính '" + str + "' không hợp lệ! Vui lòng chọn 1 (Nam) hoặc 2 (Nữ).");
    }

    inline std::string roleToString(Role r) {
        switch (r) {
            case Role::STUDENT: return "STUDENT";
            case Role::STAFF: return "STAFF";
            case Role::ADMIN: return "ADMIN";
            default: return "UNKNOWN";
        }
    }

    inline Role stringToRole(const std::string& str) {
        if (str == "STUDENT" || str == "Sinh vien" || str == "1") return Role::STUDENT;
        if (str == "STAFF" || str == "Can bo" || str == "2") return Role::STAFF;
        if (str == "ADMIN" || str == "Quan tri" || str == "3") return Role::ADMIN;
        throw std::invalid_argument("Vai trò '" + str + "' không hợp lệ! Vui lòng chọn 1: STUDENT, 2: STAFF, hoặc 3: ADMIN.");
    }

    inline std::string roomStatusToString(RoomStatus s) {
        switch (s) {
            case RoomStatus::EMPTY: return "EMPTY";
            case RoomStatus::OCCUPIED: return "OCCUPIED";
            case RoomStatus::FULL: return "FULL";
            case RoomStatus::MAINTENANCE: return "MAINTENANCE";
            default: return "UNKNOWN";
        }
    }

    inline RoomStatus stringToRoomStatus(const std::string& str) {
        if (str == "EMPTY" || str == "Trong" || str == "1") return RoomStatus::EMPTY;
        if (str == "OCCUPIED" || str == "Dang o" || str == "2") return RoomStatus::OCCUPIED;
        if (str == "FULL" || str == "Day" || str == "3") return RoomStatus::FULL;
        if (str == "MAINTENANCE" || str == "Bao tri" || str == "4") return RoomStatus::MAINTENANCE;
        throw std::invalid_argument("Trạng thái phòng '" + str + "' không hợp lệ! Hợp lệ: EMPTY, OCCUPIED, FULL, MAINTENANCE.");
    }

    inline std::string rentStatusToString(RentStatus s) {
        switch (s) {
            case RentStatus::ACTIVE: return "ACTIVE";
            case RentStatus::EXPIRED: return "EXPIRED";
            case RentStatus::TERMINATED: return "TERMINATED";
            default: return "UNKNOWN";
        }
    }

    inline RentStatus stringToRentStatus(const std::string& str) {
        if (str == "ACTIVE" || str == "Hieu luc" || str == "1") return RentStatus::ACTIVE;
        if (str == "EXPIRED" || str == "Het han" || str == "2") return RentStatus::EXPIRED;
        if (str == "TERMINATED" || str == "Da cham dut" || str == "3") return RentStatus::TERMINATED;
        throw std::invalid_argument("Trạng thái hợp đồng '" + str + "' không hợp lệ! Hợp lệ: ACTIVE, EXPIRED, TERMINATED.");
    }

    inline int periodToMonths(Period p) {
        return static_cast<int>(p);
    }

    inline std::string periodToString(Period p) {
        switch (p) {
            case Period::ONE_MONTH: return "1 thang";
            case Period::THREE_MONTHS: return "3 thang";
            case Period::SIX_MONTHS: return "6 thang";
            case Period::ONE_YEAR: return "12 thang";
            default: return std::to_string(static_cast<int>(p)) + " thang";
        }
    }

    inline Period intToPeriod(int months) {
        switch (months) {
            case 1: return Period::ONE_MONTH;
            case 3: return Period::THREE_MONTHS;
            case 6: return Period::SIX_MONTHS;
            case 12: return Period::ONE_YEAR;
            default:
                throw std::invalid_argument("Kỳ hạn thuê '" + std::to_string(months) + "' tháng không hợp lệ! Chỉ chấp nhận 1, 3, 6, hoặc 12 tháng.");
        }
    }
}

// =========================================================================
// 2. LỚP XỬ LÝ NGÀY THÁNG (DATE)
// =========================================================================

class Date {
private:
    int day;
    int month;
    int year;

    static bool isLeapYear(int y);
    static int daysInMonth(int m, int y);

public:
    Date();
    Date(int d, int m, int y);

    int getDay() const;
    int getMonth() const;
    int getYear() const;

    void setDay(int d);
    void setMonth(int m);
    void setYear(int y);

    bool isValid() const;
    std::string toString() const;
    static Date fromString(const std::string& str);
    static bool tryParse(const std::string& str, Date& outDate);

    bool isBefore(const Date& other) const;
    Date addMonths(int months) const;
    int toDays() const;
    int daysBetween(const Date& other) const;

    bool operator<(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator>=(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Date& dt);
};

// =========================================================================
// 3. INTERFACE CƠ SỞ (IENTITY)
// =========================================================================

class IEntity {
public:
    virtual ~IEntity() = default;

    virtual std::string getId() const = 0;
    virtual void print() const = 0;
    virtual nlohmann::json toJson() const = 0;

    virtual std::string serialize() const {
        return toJson().dump(2);
    }
};


// =========================================================================
// 4. LỚP QUẢN LÝ KHOA ĐỒNG BỘ (FACULTY - ĐỒNG BỘ 10 KHOA)
// =========================================================================

class Faculty {
public:
    static const std::vector<std::string>& getList();
    static void printMenu();
    static std::string getByIndex(int index); // 1..10
    static bool isValidIndex(int index);
    static std::string normalizeFaculty(const std::string& input);
};

// =========================================================================
// 5. LỚP QUY CHUẨN LỚP SINH HOẠT & TÍNH KHÓA (CLASSVALIDATOR)
// =========================================================================

class ClassValidator {
public:
    // Tự động tính toán ra Khóa từ className (ví dụ: "22T_DT1" -> "K22", "21D_DT2" -> "K21")
    static std::string extractCohort(const std::string& className);

    // Bẫy lỗi và quy định rõ tính chính xác của className
    static bool isValidClassName(const std::string& className, std::string& errorReason);
    static bool isValidClassName(const std::string& className);

    // Rà soát danh mục lớp chuẩn từ file data/classes.json
    static std::vector<std::string> loadClasses(const std::string& filePath = "data/classes.json");
    static bool isClassRegistered(const std::string& className, const std::string& filePath = "data/classes.json");
    static void registerClass(const std::string& className, const std::string& filePath = "data/classes.json");
};

#endif // COMMON_H
