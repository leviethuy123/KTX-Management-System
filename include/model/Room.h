#ifndef ROOM_H
#define ROOM_H

#include "common/IEntity.h"
#include "common/Enums.h"
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class Rent; // Forward declaration

/**
 * @brief Lớp cơ sở trừu tượng Room (Khớp sơ đồ XML, hỗ trợ lưu trữ JSON).
 */
class Room : public IEntity {
protected:
    std::string roomId;
    int maxStudents;
    std::vector<Rent*> activeRent;
    RoomStatus status;
    int floor;
    std::string building;
    bool chair;
    bool desk;
    bool fan;

public:
    Room();
    Room(const std::string& id, int maxStd, int fl, const std::string& bld,
         RoomStatus st = RoomStatus::EMPTY);
    virtual ~Room();

    // Getters & Setters
    std::string getRoomId() const;
    int getMaxStudents() const;
    RoomStatus getStatus() const;
    int getFloor() const;
    std::string getBuilding() const;
    bool hasChair() const;
    bool hasDesk() const;
    bool hasFan() const;
    const std::vector<Rent*>& getActiveRent() const;

    void setRoomId(const std::string& id);
    void setMaxStudents(int maxStd);
    void setStatus(RoomStatus st);
    void setFloor(int fl);
    void setBuilding(const std::string& bld);
    void setChair(bool c);
    void setDesk(bool d);
    void setFan(bool f);

    // Nghiệp vụ
    void addRent(Rent* rent);
    bool removeRent(Rent* rent);
    bool isEmpty() const;
    bool isFull() const;

    // Các hàm ảo hỗ trợ đa hình và tính phí
    virtual std::string getRoomType() const = 0;
    virtual double getBasePrice() const = 0;
    virtual double calculateTotalFee(double electricUsage = 0, double waterUsage = 0) const = 0;
    virtual std::string getAmenities() const = 0;

    // IEntity & JSON
    std::string getId() const override;
    void print() const override;
    virtual nlohmann::json toJson() const override = 0;
    std::string serialize() const override {
        return toJson().dump(2);
    }
};

#endif // ROOM_H
