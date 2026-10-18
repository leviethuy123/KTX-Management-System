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

nlohmann::json VipRoom::toJson() const {
    nlohmann::json j;
    j["roomId"] = roomId;
    j["roomType"] = "VIP";
    j["building"] = building;
    j["floor"] = floor;
    j["maxStudents"] = maxStudents;
    j["status"] = EnumHelper::roomStatusToString(status);
    j["basePrice"] = basePrice;
    j["serviceFee"] = serviceFee;
    j["chair"] = chair;
    j["desk"] = desk;
    j["fan"] = fan;
    j["showerHeater"] = showerHeater;
    j["internet"] = internet;
    j["airConditioner"] = airConditioner;
    return j;
}

std::string VipRoom::serialize() const {
    return toJson().dump(2);
}

VipRoom VipRoom::fromJson(const nlohmann::json& j) {
    std::string id = j.value("roomId", "");
    std::string bld = j.value("building", "");
    int fl = j.value("floor", 1);
    RoomStatus st = EnumHelper::stringToRoomStatus(j.value("status", "EMPTY"));
    double price = j.value("basePrice", 1200000.0);
    double sFee = j.value("serviceFee", 250000.0);

    VipRoom room(id, fl, bld, price, sFee, st);
    room.setChair(j.value("chair", true));
    room.setDesk(j.value("desk", true));
    room.setFan(j.value("fan", true));
    room.setShowerHeater(j.value("showerHeater", true));
    room.setInternet(j.value("internet", true));
    room.setAirConditioner(j.value("airConditioner", true));
    return room;
}

VipRoom VipRoom::deserialize(const std::string& jsonStr) {
    try {
        nlohmann::json j = nlohmann::json::parse(jsonStr);
        return fromJson(j);
    } catch (...) {
        return VipRoom();
    }
}
