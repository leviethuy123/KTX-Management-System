#ifndef ENUMS_H
#define ENUMS_H

#include <string>

// Giới tính
enum class Gender {
    MALE,
    FEMALE,
    OTHER
};

// Vai trò người dùng
enum class Role {
    STUDENT,
    STAFF,
    ADMIN
};

// Trạng thái phòng (Theo sơ đồ XML: EMPTY, OCCUPIED, FULL, MAINTENANCE)
enum class RoomStatus {
    EMPTY,       // Còn trống
    OCCUPIED,    // Đang có người ở
    FULL,        // Đã đầy chỗ
    MAINTENANCE  // Đang bảo trì
};

// Trạng thái hợp đồng thuê (Theo sơ đồ XML: ACTIVE, EXPIRED, TERMINATED)
enum class RentStatus {
    ACTIVE,      // Đang hiệu lực
    EXPIRED,     // Hết hạn
    TERMINATED   // Chấm dứt sớm
};

// Thời hạn hợp đồng thuê (Period theo sơ đồ XML)
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

#endif // ENUMS_H
