#ifndef STANDARD_ROOM_H
#define STANDARD_ROOM_H

#include "model/Room.h"
#include <nlohmann/json.hpp>

/**
 * @brief Lớp StandardRoom (Khớp sơ đồ XML, hỗ trợ lưu trữ JSON).
 */
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

    // Getters & Setters
    bool hasShowerHeater() const;
    bool hasInternet() const;
    bool hasAirConditioner() const;
    void setShowerHeater(bool sh);
    void setInternet(bool net);
    void setAirConditioner(bool ac);
    void setBasePrice(double price);

    // Ghi đè phương thức từ Room
    std::string getRoomType() const override;
    double getBasePrice() const override;
    double calculateTotalFee(double electricUsage = 0, double waterUsage = 0) const override;
    std::string getAmenities() const override;

    // JSON
    nlohmann::json toJson() const override;
    std::string serialize() const override;
    static StandardRoom fromJson(const nlohmann::json& j);
    static StandardRoom deserialize(const std::string& jsonStr);
};

#endif // STANDARD_ROOM_H
