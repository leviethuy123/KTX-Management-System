#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

#include "manager/Manager.h"
#include "model/Room.h"
#include "model/StandardRoom.h"
#include "model/VipRoom.h"
#include <vector>
#include <string>

/**
 * @brief Lớp quản lý Phòng KTX (Kế thừa từ Manager<Room>).
 * Khớp đầy đủ các phương thức trong sơ đồ XML.
 */
class RoomManager : public Manager<Room> {
public:
    RoomManager(const std::string& filePath = "data/rooms.json");
    virtual ~RoomManager() = default;

    // Đọc / Ghi file JSON đa hình
    void loadFromFile() override;
    void saveToFile() const override;

    // Các nghiệp vụ đặc thù khớp XML
    std::vector<Room*> findAvailableRoom() const;
    void printAvailableRoom() const;

    // Nghiệp vụ lọc mở rộng
    std::vector<Room*> findByBuilding(const std::string& building) const;
    std::vector<Room*> findByType(const std::string& roomType) const;

    // Nghiệp vụ nâng cao Giai đoạn 3: Thống kê lấp đầy & Tính tiền phòng đa hình
    void getOccupancyStats(int& totalCapacity, int& totalOccupants, double& rate) const;
    void printOccupancyReport() const;
    void printRoomBill(const std::string& roomId, double electricUsage, double waterUsage) const;
};

#endif // ROOM_MANAGER_H
