#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
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
        if (str == "Nu" || str == "FEMALE" || str == "0") return Gender::FEMALE;
        return Gender::OTHER;
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
        if (str == "STUDENT") return Role::STUDENT;
        if (str == "STAFF") return Role::STAFF;
        return Role::ADMIN;
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
        if (str == "OCCUPIED" || str == "Dang o") return RoomStatus::OCCUPIED;
        if (str == "FULL" || str == "Day") return RoomStatus::FULL;
        if (str == "MAINTENANCE" || str == "Bao tri") return RoomStatus::MAINTENANCE;
        return RoomStatus::EMPTY;
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
        if (str == "EXPIRED" || str == "Het han") return RentStatus::EXPIRED;
        if (str == "TERMINATED" || str == "Da cham dut") return RentStatus::TERMINATED;
        return RentStatus::ACTIVE;
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
            default: return "1 thang";
        }
    }

    inline Period intToPeriod(int months) {
        switch (months) {
            case 3: return Period::THREE_MONTHS;
            case 6: return Period::SIX_MONTHS;
            case 12: return Period::ONE_YEAR;
            default: return Period::ONE_MONTH;
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

#endif // COMMON_H
