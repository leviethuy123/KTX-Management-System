#include "model/VipRoom.h"
#include <sstream>
#include <vector>
#include <iomanip>

VipRoom::VipRoom()
    : Room(), basePrice(1000000.0), serviceFee(200000.0), hasAirConditioner(true) {}

VipRoom::VipRoom(const std::string& id, const std::string& bld, int fl,
                 double price, double sFee, bool airCon,
                 RoomStatus st, int occ)
    : Room(id, bld, fl, st, occ),
      basePrice(price), serviceFee(sFee), hasAirConditioner(airCon) {}

double VipRoom::getBasePrice() const {
    return basePrice;
}

double VipRoom::getServiceFee() const {
    return serviceFee;
}

bool VipRoom::getHasAirConditioner() const {
    return hasAirConditioner;
}

void VipRoom::setBasePrice(double price) {
    basePrice = price;
}

void VipRoom::setServiceFee(double sFee) {
    serviceFee = sFee;
}

void VipRoom::setHasAirConditioner(bool airCon) {
    hasAirConditioner = airCon;
}

int VipRoom::getMaxStudents() const {
    return 4; 
}

RoomType VipRoom::getRoomType() const {
    return RoomType::VIP;
}

double VipRoom::calculateTotalFee(double electricUsage, double waterUsage) const {
    return basePrice + serviceFee + (electricUsage * 3000.0) + (waterUsage * 10000.0);
}

std::string VipRoom::getAmenities() const {
    return "Điều hòa Inverter, Nóng lạnh, Wifi riêng, Tủ lạnh mini, Bàn học riêng";
}

std::string VipRoom::serialize() const {
    std::ostringstream oss;
    oss << "VIP|"
        << roomId << "|"
        << building << "|"
        << floor << "|"
        << EnumHelper::roomStatusToString(status) << "|"
        << currentOccupants << "|"
        << std::fixed << std::setprecision(0) << basePrice << "|"
        << serviceFee << "|"
        << (hasAirConditioner ? "1" : "0");
    return oss.str();
}

VipRoom VipRoom::deserialize(const std::string& line) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, '|')) {
        tokens.push_back(token);
    }

    if (tokens.size() < 9) {
        return VipRoom();
    }

    std::string id = tokens[1];
    std::string bld = tokens[2];
    int fl = std::stoi(tokens[3]);
    RoomStatus st = EnumHelper::stringToRoomStatus(tokens[4]);
    int occ = std::stoi(tokens[5]);
    double price = std::stod(tokens[6]);
    double sFee = std::stod(tokens[7]);
    bool airCon = (tokens[8] == "1");

    return VipRoom(id, bld, fl, price, sFee, airCon, st, occ);
}
