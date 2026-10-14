#include "model/VipRoom.h"
#include <sstream>
#include <vector>
#include <iomanip>

VipRoom::VipRoom()
    : Room("", 4, 1, ""),
      showerHeater(true), internet(true), airConditioner(true),
      basePrice(1200000.0), serviceFee(250000.0) {}

VipRoom::VipRoom(const std::string& rId, int fl, const std::string& bld,
                 double price, double sFee, RoomStatus st)
    : Room(rId, 4, fl, bld, st),
      showerHeater(true), internet(true), airConditioner(true),
      basePrice(price), serviceFee(sFee) {}

bool VipRoom::hasShowerHeater() const { return showerHeater; }
bool VipRoom::hasInternet() const { return internet; }
bool VipRoom::hasAirConditioner() const { return airConditioner; }

void VipRoom::setShowerHeater(bool sh) { showerHeater = sh; }
void VipRoom::setInternet(bool net) { internet = net; }
void VipRoom::setAirConditioner(bool ac) { airConditioner = ac; }
void VipRoom::setBasePrice(double price) { basePrice = price; }
void VipRoom::setServiceFee(double sFee) { serviceFee = sFee; }

std::string VipRoom::getRoomType() const {
    return "VIP";
}

double VipRoom::getBasePrice() const {
    return basePrice;
}

double VipRoom::calculateTotalFee(double electricUsage, double waterUsage) const {
    return basePrice + serviceFee + (electricUsage * 3000.0) + (waterUsage * 10000.0);
}

std::string VipRoom::getAmenities() const {
    return "Điều hòa Inverter, Nóng lạnh, Wifi riêng, Bàn học riêng";
}

std::string VipRoom::serialize() const {
    std::ostringstream oss;
    oss << "VIP|"
        << roomId << "|"
        << building << "|"
        << floor << "|"
        << EnumHelper::roomStatusToString(status) << "|"
        << maxStudents << "|"
        << std::fixed << std::setprecision(0) << basePrice << "|"
        << serviceFee;
    return oss.str();
}

VipRoom VipRoom::deserialize(const std::string& line) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, '|')) {
        tokens.push_back(token);
    }

    if (tokens.size() < 8) {
        return VipRoom();
    }

    std::string id = tokens[1];
    std::string bld = tokens[2];
    int fl = std::stoi(tokens[3]);
    RoomStatus st = EnumHelper::stringToRoomStatus(tokens[4]);
    double price = std::stod(tokens[6]);
    double sFee = std::stod(tokens[7]);

    return VipRoom(id, fl, bld, price, sFee, st);
}
