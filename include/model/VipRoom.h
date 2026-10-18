#ifndef VIP_ROOM_H
#define VIP_ROOM_H

#include "model/Room.h"
#include <nlohmann/json.hpp>

/**
 * @brief Lớp VipRoom (Khớp sơ đồ XML, hỗ trợ lưu trữ JSON).
 */
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

    // JSON
    nlohmann::json toJson() const override;
    std::string serialize() const override;
    static VipRoom fromJson(const nlohmann::json& j);
    static VipRoom deserialize(const std::string& jsonStr);
};

#endif // VIP_ROOM_H
