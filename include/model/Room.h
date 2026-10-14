#ifndef ROOM_H
#define ROOM_H

#include "common/IEntity.h"
#include "common/Enums.h"
#include <string>
#include <vector>

class Rent; // Forward declaration

/**
 * @brief Lớp cơ sở trừu tượng Room (Khớp 100% với sơ đồ XML).
 */
class Room : public IEntity {
protected:
    std::string roomId;
    int maxStudents;
    std::vector<Rent*> activeRent; // Khớp XML: - activeRent: vector<Rent*>
    RoomStatus status;
    int floor;
    std::string building;
    bool chair;                    // Khớp XML: - chair: bool = True
    bool desk;                     // Khớp XML: - desk: bool = True
    bool fan;                      // Khớp XML: - fan: bool = True

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

    // Nghiệp vụ theo đúng sơ đồ XML
    void addRent(Rent* rent);
    bool removeRent(Rent* rent);
    bool isEmpty() const;
    bool isFull() const;

    // Các hàm ảo hỗ trợ đa hình và tính phí
    virtual std::string getRoomType() const = 0;
    virtual double getBasePrice() const = 0;
    virtual double calculateTotalFee(double electricUsage = 0, double waterUsage = 0) const = 0;
    virtual std::string getAmenities() const = 0;

    // IEntity
    std::string getId() const override;
    void print() const override;
};

#endif // ROOM_H
