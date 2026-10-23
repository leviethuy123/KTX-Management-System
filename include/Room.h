#ifndef ROOM_H
#define ROOM_H

#include "Common.h"

class Rent; // Forward declaration tránh phụ thuộc vòng

// =========================================================================
// 1. LỚP CƠ SỞ TRỪU TƯỢNG PHÒNG (ROOM)
// =========================================================================

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

    void addRent(Rent* rent);
    bool removeRent(Rent* rent);
    bool isEmpty() const;
    bool isFull() const;

    virtual std::string getRoomType() const = 0;
    virtual double getBasePrice() const = 0;
    virtual double calculateTotalFee(double electricUsage = 0, double waterUsage = 0) const = 0;
    virtual std::string getAmenities() const = 0;

    std::string getId() const override;
    void print() const override;
    virtual nlohmann::json toJson() const override = 0;
    std::string serialize() const override {
        return toJson().dump(2);
    }
};

// =========================================================================
// 2. LỚP PHÒNG TIÊU CHUẨN (STANDARDROOM - KẾ THỪA ROOM)
// =========================================================================

class StandardRoom : public Room {
private:
    bool showerHeater;
    bool internet;
    bool airConditioner;
    double basePrice;

public:
    StandardRoom();
    StandardRoom(const std::string& rId, int fl, const std::string& bld,
                 double price = 400000.0, RoomStatus st = RoomStatus::EMPTY);
    virtual ~StandardRoom() = default;

    bool hasShowerHeater() const;
    bool hasInternet() const;
    bool hasAirConditioner() const;
    void setShowerHeater(bool sh);
    void setInternet(bool net);
    void setAirConditioner(bool ac);
    void setBasePrice(double price);

    std::string getRoomType() const override;
    double getBasePrice() const override;
    double calculateTotalFee(double electricUsage = 0, double waterUsage = 0) const override;
    std::string getAmenities() const override;

    nlohmann::json toJson() const override;
    std::string serialize() const override;
    static StandardRoom fromJson(const nlohmann::json& j);
    static StandardRoom deserialize(const std::string& jsonStr);
};

// =========================================================================
// 3. LỚP PHÒNG VIP DỊCH VỤ CAO CẤP (VIPROOM - KẾ THỪA ROOM)
// =========================================================================

class VipRoom : public Room {
private:
    bool showerHeater;
    bool internet;
    bool airConditioner;
    double basePrice;
    double serviceFee;

public:
    VipRoom();
    VipRoom(const std::string& rId, int fl, const std::string& bld,
            double price = 1200000.0, double sFee = 250000.0, RoomStatus st = RoomStatus::EMPTY);
    virtual ~VipRoom() = default;

    bool hasShowerHeater() const;
    bool hasInternet() const;
    bool hasAirConditioner() const;
    double getServiceFee() const;

    void setShowerHeater(bool sh);
    void setInternet(bool net);
    void setAirConditioner(bool ac);
    void setBasePrice(double price);
    void setServiceFee(double sFee);

    std::string getRoomType() const override;
    double getBasePrice() const override;
    double calculateTotalFee(double electricUsage = 0, double waterUsage = 0) const override;
    std::string getAmenities() const override;

    nlohmann::json toJson() const override;
    std::string serialize() const override;
    static VipRoom fromJson(const nlohmann::json& j);
    static VipRoom deserialize(const std::string& jsonStr);
};

#endif // ROOM_H
