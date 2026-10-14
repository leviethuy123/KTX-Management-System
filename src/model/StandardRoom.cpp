#include "model/StandardRoom.h"
#include <sstream>
#include <vector>
#include <iomanip>

StandardRoom::StandardRoom()
    : Room("", 8, 1, ""),
      showerHeater(false), internet(false), airConditioner(false), basePrice(400000.0) {}

StandardRoom::StandardRoom(const std::string& rId, int fl, const std::string& bld,
                           double price, RoomStatus st)
    : Room(rId, 8, fl, bld, st),
      showerHeater(false), internet(false), airConditioner(false), basePrice(price) {}

bool StandardRoom::hasShowerHeater() const { return showerHeater; }
bool StandardRoom::hasInternet() const { return internet; }
bool StandardRoom::hasAirConditioner() const { return airConditioner; }

void StandardRoom::setShowerHeater(bool sh) { showerHeater = sh; }
void StandardRoom::setInternet(bool net) { internet = net; }
void StandardRoom::setAirConditioner(bool ac) { airConditioner = ac; }
void StandardRoom::setBasePrice(double price) { basePrice = price; }

std::string StandardRoom::getRoomType() const {
    return "STANDARD";
}

double StandardRoom::getBasePrice() const {
    return basePrice;
}

double StandardRoom::calculateTotalFee(double electricUsage, double waterUsage) const {
    return basePrice + (electricUsage * 3000.0) + (waterUsage * 10000.0);
}

std::string StandardRoom::getAmenities() const {
    return "Bàn học, Ghế, Quạt trần (Tiêu chuẩn)";
}

std::string StandardRoom::serialize() const {
    std::ostringstream oss;
    oss << "STANDARD|"
        << roomId << "|"
        << building << "|"
        << floor << "|"
        << EnumHelper::roomStatusToString(status) << "|"
        << maxStudents << "|"
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
    double price = std::stod(tokens[6]);

    return StandardRoom(id, fl, bld, price, st);
}
