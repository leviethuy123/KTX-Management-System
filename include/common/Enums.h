#ifndef ENUMS_H
#define ENUMS_H

#include <string>

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
    EMPTY,       
    OCCUPIED,    
    FULL,        
    MAINTENANCE  
};

enum class RoomType {
    STANDARD,
    VIP
};


enum class RentStatus {
    ACTIVE,      
    EXPIRED,     
    TERMINATED  
};

namespace EnumHelper {
    inline std::string genderToString(Gender g) {
        switch (g) {
            case Gender::MALE: return "Nam";
            case Gender::FEMALE: return "Nu";
            case Gender::OTHER: return "Khac";
            default: return "Khong xac dinh";
        }
    }

    inline Gender stringToGender(const std::string& str) {
        if (str == "Nam" || str == "MALE") return Gender::MALE;
        if (str == "Nu" || str == "FEMALE") return Gender::FEMALE;
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
            case RoomStatus::EMPTY: return "Trong";
            case RoomStatus::OCCUPIED: return "Dang o";
            case RoomStatus::FULL: return "Day";
            case RoomStatus::MAINTENANCE: return "Bao tri";
            default: return "Khong xac dinh";
        }
    }

    inline RoomStatus stringToRoomStatus(const std::string& str) {
        if (str == "Trong" || str == "EMPTY") return RoomStatus::EMPTY;
        if (str == "Dang o" || str == "OCCUPIED") return RoomStatus::OCCUPIED;
        if (str == "Day" || str == "FULL") return RoomStatus::FULL;
        return RoomStatus::MAINTENANCE;
    }

    inline std::string roomTypeToString(RoomType t) {
        switch (t) {
            case RoomType::STANDARD: return "STANDARD";
            case RoomType::VIP: return "VIP";
            default: return "STANDARD";
        }
    }

    inline RoomType stringToRoomType(const std::string& str) {
        if (str == "VIP") return RoomType::VIP;
        return RoomType::STANDARD;
    }

    inline std::string rentStatusToString(RentStatus s) {
        switch (s) {
            case RentStatus::ACTIVE: return "Hieu luc";
            case RentStatus::EXPIRED: return "Het han";
            case RentStatus::TERMINATED: return "Da thanh ly";
            default: return "Khong xac dinh";
        }
    }

    inline RentStatus stringToRentStatus(const std::string& str) {
        if (str == "Hieu luc" || str == "ACTIVE") return RentStatus::ACTIVE;
        if (str == "Het han" || str == "EXPIRED") return RentStatus::EXPIRED;
        return RentStatus::TERMINATED;
    }
}

#endif 
