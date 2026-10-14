#ifndef STANDARD_ROOM_H
#define STANDARD_ROOM_H

#include "model/Room.h"

/**
 * @brief Lớp StandardRoom (Khớp 100% với sơ đồ XML).
 */
class StandardRoom : public Room {
private:
    bool showerHeater;    // Khớp XML: - showerHeater: bool = False
    bool internet;        // Khớp XML: - internet: bool = False
    bool airConditioner;  // Khớp XML: - airConditioner: bool = False
    double basePrice;

public:
    StandardRoom();
    // Constructor khớp XML: StandardRoom(const string& roomId, const int& floor, const string& building)
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

    std::string serialize() const override;
    static StandardRoom deserialize(const std::string& line);
};

#endif // STANDARD_ROOM_H
