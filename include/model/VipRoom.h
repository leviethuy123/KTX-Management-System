#ifndef VIP_ROOM_H
#define VIP_ROOM_H

#include "model/Room.h"

class VipRoom : public Room {
private:
    double basePrice;        
    double serviceFee;      
    bool hasAirConditioner;  

public:
    VipRoom();
    VipRoom(const std::string& id, const std::string& bld, int fl,
            double price = 1000000.0, double sFee = 200000.0, bool airCon = true,
            RoomStatus st = RoomStatus::EMPTY, int occ = 0);

    double getBasePrice() const override;
    double getServiceFee() const;
    bool getHasAirConditioner() const;

    void setBasePrice(double price);
    void setServiceFee(double sFee);
    void setHasAirConditioner(bool airCon);

    int getMaxStudents() const override;
    RoomType getRoomType() const override;
    double calculateTotalFee(double electricUsage = 0, double waterUsage = 0) const override;
    std::string getAmenities() const override;

    std::string serialize() const override;
    static VipRoom deserialize(const std::string& line);
};

#endif 
