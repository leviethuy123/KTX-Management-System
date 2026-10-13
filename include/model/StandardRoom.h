#ifndef STANDARD_ROOM_H
#define STANDARD_ROOM_H

#include "model/Room.h"


class StandardRoom : public Room {
private:
    double basePrice; 

public:
    StandardRoom();
    StandardRoom(const std::string& id, const std::string& bld, int fl,
                 double price = 400000.0, RoomStatus st = RoomStatus::EMPTY, int occ = 0);

    
    double getBasePrice() const override;
    void setBasePrice(double price);

    int getMaxStudents() const override;
    RoomType getRoomType() const override;
    double calculateTotalFee(double electricUsage = 0, double waterUsage = 0) const override;
    std::string getAmenities() const override;

    std::string serialize() const override;
    static StandardRoom deserialize(const std::string& line);
};

#endif
