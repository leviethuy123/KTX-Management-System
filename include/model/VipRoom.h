#ifndef VIP_ROOM_H
#define VIP_ROOM_H

#include "model/Room.h"

/**
 * @brief Lớp VipRoom (Khớp 100% với sơ đồ XML).
 */
class VipRoom : public Room {
private:
    bool showerHeater;    // Khớp XML: - showerHeater: bool = True
    bool internet;        // Khớp XML: - internet: bool = True
    bool airConditioner;  // Khớp XML: - airConditioner: bool = True
    double basePrice;
    double serviceFee;

public:
    VipRoom();
    // Constructor khớp XML: VipRoom(const string& roomId, const int& floor, const string& building)
    VipRoom(const std::string& rId, int fl, const std::string& bld,
            double price = 1200000.0, double sFee = 250000.0, RoomStatus st = RoomStatus::EMPTY);
    virtual ~VipRoom() = default;

    // Getters & Setters
    bool hasShowerHeater() const;
    bool hasInternet() const;
    bool hasAirConditioner() const;
    void setShowerHeater(bool sh);
    void setInternet(bool net);
    void setAirConditioner(bool ac);
    void setBasePrice(double price);
    void setServiceFee(double sFee);

    // Ghi đè phương thức từ Room
    std::string getRoomType() const override;
    double getBasePrice() const override;
    double calculateTotalFee(double electricUsage = 0, double waterUsage = 0) const override;
    std::string getAmenities() const override;

    std::string serialize() const override;
    static VipRoom deserialize(const std::string& line);
};

#endif // VIP_ROOM_H
