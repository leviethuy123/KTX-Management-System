#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

#include "manager/Manager.h"
#include "Room.h"
#include <string>

/**
 * @brief Lớp quản lý Phòng ở KTX (Kế thừa từ Manager<Room>).
 */
class RoomManager : public Manager<Room> {
public:
    RoomManager(const std::string& filePath = "data/rooms.json");
    virtual ~RoomManager() = default;

    // Đọc / Ghi file JSON
    void loadFromFile() override;
    void saveToFile() const override;

    // Nghiệp vụ phòng ở
    Room* createRoom(const std::string& rId, const std::string& type, int fl, const std::string& bld,
                     int maxStd = 0, double price = 0.0, double sFee = 0.0);
    Vector<Room*> findAvailableRoom() const;
    Vector<Room*> findAvailableRoomByGender(Gender g) const;
    void printAvailableRoom() const;
    Vector<Room*> findByBuilding(const std::string& building) const;
    Vector<Room*> findByType(const std::string& roomType) const;

    // Thống kê & Báo cáo doanh thu
    void getOccupancyStats(int& totalCapacity, int& totalOccupants, double& rate) const;
    void printOccupancyReport() const;
    void printRoomBill(const std::string& roomId, double electricUsage, double waterUsage) const;

    RoomRevenueRow calculateRoomMonthlyRevenue(const std::string& roomId, double electricUsage, double waterUsage) const;
    Vector<RoomRevenueRow> calculateAllRoomsMonthlyRevenue(double defaultElectric = 50.0, double defaultWater = 5.0) const;
    void printMonthlyRevenueReport(const std::string& monthYear, double defaultElec = 50.0, double defaultWater = 5.0) const;
    void printSpecificRoomRevenue(const std::string& roomId, const std::string& monthYear, double electricUsage, double waterUsage) const;
};

#endif // ROOM_MANAGER_H
