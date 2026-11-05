#ifndef RENT_MANAGER_H
#define RENT_MANAGER_H

#include "manager/Manager.h"
#include "Rent.h"
#include "manager/StudentManager.h"
#include "manager/RoomManager.h"
#include <vector>
#include <string>

/**
 * @brief Lớp quản lý Hợp đồng thuê phòng KTX (Kế thừa từ Manager<Rent>).
 */
class RentManager : public Manager<Rent> {
public:
    RentManager(const std::string& filePath = "data/rents.json");
    virtual ~RentManager() = default;

    // Đọc / Ghi file JSON
    void loadFromFile() override;
    void loadFromFile(StudentManager& sm, RoomManager& rm);
    void saveToFile() const override;

    // Tìm kiếm theo tiêu chí
    std::vector<Rent*> findByStatus(RentStatus status) const;
    std::vector<Rent*> findByStudent(const std::string& studentId) const;
    std::vector<Rent*> findByRoom(const std::string& roomId) const;

    // Nghiệp vụ vòng đời hợp đồng
    Rent* createRent(const std::string& rentId, Student* student, Room* room,
                     Period period, const Date& startDate, double fee);
    bool terminateRent(const std::string& rentId, const Date& actualEndDate);
    bool extendRent(const std::string& rentId, int additionalMonths);
    bool transferRoom(const std::string& studentId, const std::string& newRoomId, RoomManager& roomMgr);

    // Cảnh báo & Tự động quét hợp đồng
    std::vector<Rent*> findExpiringRents(const Date& currentDate, int daysThreshold = 30) const;
    void printExpiringRents(const Date& currentDate, int daysThreshold = 30) const;
    int checkAndExpireRents(const Date& currentDate);
    double calculateTotalMonthlyRevenue() const;
};

#endif // RENT_MANAGER_H
