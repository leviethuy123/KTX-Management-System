#include "model/StandardRoom.h"
#include <sstream>
#include <vector>
#include <iomanip>

StandardRoom::StandardRoom()
    : Room(), basePrice(400000.0) {}

StandardRoom::StandardRoom(const std::string& id, const std::string& bld, int fl,
                           double price, RoomStatus st, int occ)
    : Room(id, bld, fl, st, occ), basePrice(price) {}

double StandardRoom::getBasePrice() const {
    return basePrice;
}

void StandardRoom::setBasePrice(double price) {
    basePrice = price;
}

int StandardRoom::getMaxStudents() const {
    return 8; 
}

RoomType StandardRoom::getRoomType() const {
    return RoomType::STANDARD;
}

double StandardRoom::calculateTotalFee(double electricUsage, double waterUsage) const {
  
    return basePrice + (electricUsage * 3000.0) + (waterUsage * 10000.0);
}

std::string StandardRoom::getAmenities() const {
    return "Quạt trần, Giường tầng, Bàn học chung";
}

std::string StandardRoom::serialize() const {
    std::ostringstream oss;
    oss << "STANDARD|"
        << roomId << "|"
        << building << "|"
        << floor << "|"
        << EnumHelper::roomStatusToString(status) << "|"
        << currentOccupants << "|"
        << std::fixed << std::setprecision(0) << basePrice;
    return oss.str();
}

StandardRoom StandardRoom::deserialize(const std::string& line) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, '|')) {
        tokens.push_back(token);
    }

    if (tokens.size() < 7) {
        return StandardRoom();
    }

    std::string id = tokens[1];
    std::string bld = tokens[2];
    int fl = std::stoi(tokens[3]);
    RoomStatus st = EnumHelper::stringToRoomStatus(tokens[4]);
    int occ = std::stoi(tokens[5]);
    double price = std::stod(tokens[6]);

    return StandardRoom(id, bld, fl, price, st, occ);
}
