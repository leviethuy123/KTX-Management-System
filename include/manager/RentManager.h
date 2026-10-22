#ifndef RENT_MANAGER_H
#define RENT_MANAGER_H

#include "manager/Manager.h"
#include "model/Rent.h"
#include "manager/StudentManager.h"
#include "manager/RoomManager.h"
#include <vector>
#include <string>

/**
 * @brief Lớp quản lý Hợp đồng thuê KTX (Kế thừa từ Manager<Rent>).
 * Khớp đầy đủ các phương thức trong sơ đồ XML.
 */
class RentManager : public Manager<Rent> {
public:
    RentManager(const std::string& filePath = "data/rents.json");
    virtual ~RentManager() = default;

    // Đọc / Ghi file JSON
    void loadFromFile() override;
    void loadFromFile(StudentManager& sm, RoomManager& rm);
    void saveToFile() const override;

    // Nghiệp vụ khớp XML
    std::vector<Rent*> findByStatus(RentStatus status) const;
    std::vector<Rent*> findByStudent(const std::string& studentId) const;
    std::vector<Rent*> findByRoom(const std::string& roomId) const;

    // Tạo mới và thanh lý hợp đồng
    Rent* createRent(const std::string& rentId, Student* student, Room* room,
                     Period period, const Date& startDate, double fee);
    bool terminateRent(const std::string& rentId, const Date& actualEndDate);
};

#endif // RENT_MANAGER_H
