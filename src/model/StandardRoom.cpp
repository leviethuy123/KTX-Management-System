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

nlohmann::json StandardRoom::toJson() const {
    nlohmann::json j;
    j["roomId"] = roomId;
    j["roomType"] = "STANDARD";
    j["building"] = building;
    j["floor"] = floor;
    j["maxStudents"] = maxStudents;
    j["status"] = EnumHelper::roomStatusToString(status);
    j["basePrice"] = basePrice;
    j["chair"] = chair;
    j["desk"] = desk;
    j["fan"] = fan;
    j["showerHeater"] = showerHeater;
    j["internet"] = internet;
    j["airConditioner"] = airConditioner;
    return j;
}

std::string StandardRoom::serialize() const {
    return toJson().dump(2);
}

StandardRoom StandardRoom::fromJson(const nlohmann::json& j) {
    std::string id = j.value("roomId", "");
    std::string bld = j.value("building", "");
    int fl = j.value("floor", 1);
    RoomStatus st = EnumHelper::stringToRoomStatus(j.value("status", "EMPTY"));
    double price = j.value("basePrice", 400000.0);

    StandardRoom room(id, fl, bld, price, st);
    room.setChair(j.value("chair", true));
    room.setDesk(j.value("desk", true));
    room.setFan(j.value("fan", true));
    room.setShowerHeater(j.value("showerHeater", false));
    room.setInternet(j.value("internet", false));
    room.setAirConditioner(j.value("airConditioner", false));
    return room;
}

StandardRoom StandardRoom::deserialize(const std::string& jsonStr) {
    try {
        nlohmann::json j = nlohmann::json::parse(jsonStr);
        return fromJson(j);
    } catch (...) {
        return StandardRoom();
    }
}
