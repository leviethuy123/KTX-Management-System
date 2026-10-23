#include "Room.h"
#include "Rent.h"
#include <algorithm>

// =========================================================================
// 1. CÀI ĐẶT ROOM
// =========================================================================

Room::Room()
    : roomId(""), maxStudents(8), status(RoomStatus::EMPTY), floor(1),
      building("Khu A"), chair(true), desk(true), fan(true) {}

Room::Room(const std::string& id, int maxStd, int fl, const std::string& bld, RoomStatus st)
    : roomId(id), maxStudents(maxStd), status(st), floor(fl), building(bld),
      chair(true), desk(true), fan(true) {}

Room::~Room() {
    activeRent.clear();
}

std::string Room::getRoomId() const { return roomId; }
int Room::getMaxStudents() const { return maxStudents; }
RoomStatus Room::getStatus() const { return status; }
int Room::getFloor() const { return floor; }
std::string Room::getBuilding() const { return building; }
bool Room::hasChair() const { return chair; }
bool Room::hasDesk() const { return desk; }
bool Room::hasFan() const { return fan; }
const std::vector<Rent*>& Room::getActiveRent() const { return activeRent; }

void Room::setRoomId(const std::string& id) { roomId = id; }
void Room::setMaxStudents(int maxStd) { maxStudents = maxStd; }
void Room::setStatus(RoomStatus st) { status = st; }
void Room::setFloor(int fl) { floor = fl; }
void Room::setBuilding(const std::string& bld) { building = bld; }
void Room::setChair(bool c) { chair = c; }
void Room::setDesk(bool d) { desk = d; }
void Room::setFan(bool f) { fan = f; }

void Room::addRent(Rent* rent) {
    if (rent != nullptr) {
        activeRent.push_back(rent);
        if (static_cast<int>(activeRent.size()) >= maxStudents) {
            status = RoomStatus::FULL;
        } else {
            status = RoomStatus::OCCUPIED;
        }
    }
}

bool Room::removeRent(Rent* rent) {
    auto it = std::find(activeRent.begin(), activeRent.end(), rent);
    if (it != activeRent.end()) {
        activeRent.erase(it);
        if (activeRent.empty()) {
            status = RoomStatus::EMPTY;
        } else {
            status = RoomStatus::OCCUPIED;
        }
        return true;
    }
    return false;
}

bool Room::isEmpty() const { return activeRent.empty(); }
bool Room::isFull() const { return static_cast<int>(activeRent.size()) >= maxStudents; }
std::string Room::getId() const { return roomId; }

void Room::print() const {
    std::cout << "[PHÒNG " << std::left << std::setw(6) << roomId << "] "
              << "Tòa: " << std::setw(8) << building
              << " | Tầng: " << floor
              << " | Loại: " << std::setw(9) << getRoomType()
              << " | Đang ở: " << activeRent.size() << "/" << maxStudents
              << " | Trạng thái: " << std::setw(10) << EnumHelper::roomStatusToString(status)
              << " | Tiện ích: " << getAmenities() << std::endl;
}

// =========================================================================
// 2. CÀI ĐẶT STANDARDROOM
// =========================================================================

StandardRoom::StandardRoom()
    : Room("", 8, 1, "Khu A"), showerHeater(false), internet(false),
      airConditioner(false), basePrice(400000.0) {}

StandardRoom::StandardRoom(const std::string& rId, int fl, const std::string& bld,
                           double price, RoomStatus st)
    : Room(rId, 8, fl, bld, st), showerHeater(false), internet(false),
      airConditioner(false), basePrice(price) {}

bool StandardRoom::hasShowerHeater() const { return showerHeater; }
bool StandardRoom::hasInternet() const { return internet; }
bool StandardRoom::hasAirConditioner() const { return airConditioner; }

void StandardRoom::setShowerHeater(bool sh) { showerHeater = sh; }
void StandardRoom::setInternet(bool net) { internet = net; }
void StandardRoom::setAirConditioner(bool ac) { airConditioner = ac; }
void StandardRoom::setBasePrice(double price) { basePrice = price; }

std::string StandardRoom::getRoomType() const { return "STANDARD"; }
double StandardRoom::getBasePrice() const { return basePrice; }

double StandardRoom::calculateTotalFee(double electricUsage, double waterUsage) const {
    return basePrice + (electricUsage * 3000.0) + (waterUsage * 10000.0);
}

std::string StandardRoom::getAmenities() const {
    std::string am = "Quạt, Bàn học, Ghế";
    if (showerHeater) am += ", Bình nóng lạnh";
    if (internet) am += ", Wifi";
    if (airConditioner) am += ", Điều hòa";
    return am;
}

nlohmann::json StandardRoom::toJson() const {
    return {
        {"roomId", roomId},
        {"roomType", "STANDARD"},
        {"building", building},
        {"floor", floor},
        {"maxStudents", maxStudents},
        {"status", EnumHelper::roomStatusToString(status)},
        {"basePrice", basePrice},
        {"chair", chair},
        {"desk", desk},
        {"fan", fan},
        {"showerHeater", showerHeater},
        {"internet", internet},
        {"airConditioner", airConditioner}
    };
}

std::string StandardRoom::serialize() const { return toJson().dump(2); }

StandardRoom StandardRoom::fromJson(const nlohmann::json& j) {
    StandardRoom r;
    r.roomId = j.value("roomId", "");
    r.building = j.value("building", "Khu A");
    r.floor = j.value("floor", 1);
    r.maxStudents = j.value("maxStudents", 8);
    r.status = EnumHelper::stringToRoomStatus(j.value("status", "EMPTY"));
    r.basePrice = j.value("basePrice", 400000.0);
    r.chair = j.value("chair", true);
    r.desk = j.value("desk", true);
    r.fan = j.value("fan", true);
    r.showerHeater = j.value("showerHeater", false);
    r.internet = j.value("internet", false);
    r.airConditioner = j.value("airConditioner", false);
    return r;
}

StandardRoom StandardRoom::deserialize(const std::string& jsonStr) {
    try {
        nlohmann::json j = nlohmann::json::parse(jsonStr);
        return fromJson(j);
    } catch (...) {
        return StandardRoom();
    }
}

// =========================================================================
// 3. CÀI ĐẶT VIPROOM
// =========================================================================

VipRoom::VipRoom()
    : Room("", 4, 1, "Khu VIP"), showerHeater(true), internet(true),
      airConditioner(true), basePrice(1200000.0), serviceFee(250000.0) {}

VipRoom::VipRoom(const std::string& rId, int fl, const std::string& bld,
                 double price, double sFee, RoomStatus st)
    : Room(rId, 4, fl, bld, st), showerHeater(true), internet(true),
      airConditioner(true), basePrice(price), serviceFee(sFee) {}

bool VipRoom::hasShowerHeater() const { return showerHeater; }
bool VipRoom::hasInternet() const { return internet; }
bool VipRoom::hasAirConditioner() const { return airConditioner; }
double VipRoom::getServiceFee() const { return serviceFee; }

void VipRoom::setShowerHeater(bool sh) { showerHeater = sh; }
void VipRoom::setInternet(bool net) { internet = net; }
void VipRoom::setAirConditioner(bool ac) { airConditioner = ac; }
void VipRoom::setBasePrice(double price) { basePrice = price; }
void VipRoom::setServiceFee(double sFee) { serviceFee = sFee; }

std::string VipRoom::getRoomType() const { return "VIP"; }
double VipRoom::getBasePrice() const { return basePrice; }

double VipRoom::calculateTotalFee(double electricUsage, double waterUsage) const {
    return basePrice + serviceFee + (electricUsage * 3000.0) + (waterUsage * 10000.0);
}

std::string VipRoom::getAmenities() const {
    return "Điều hòa, Bình nóng lạnh, Wifi cáp quang, Bàn ghế cao cấp, Dịch vụ dọn dẹp";
}

nlohmann::json VipRoom::toJson() const {
    return {
        {"roomId", roomId},
        {"roomType", "VIP"},
        {"building", building},
        {"floor", floor},
        {"maxStudents", maxStudents},
        {"status", EnumHelper::roomStatusToString(status)},
        {"basePrice", basePrice},
        {"serviceFee", serviceFee},
        {"chair", chair},
        {"desk", desk},
        {"fan", fan},
        {"showerHeater", showerHeater},
        {"internet", internet},
        {"airConditioner", airConditioner}
    };
}

std::string VipRoom::serialize() const { return toJson().dump(2); }

VipRoom VipRoom::fromJson(const nlohmann::json& j) {
    VipRoom r;
    r.roomId = j.value("roomId", "");
    r.building = j.value("building", "Khu VIP");
    r.floor = j.value("floor", 1);
    r.maxStudents = j.value("maxStudents", 4);
    r.status = EnumHelper::stringToRoomStatus(j.value("status", "EMPTY"));
    r.basePrice = j.value("basePrice", 1200000.0);
    r.serviceFee = j.value("serviceFee", 250000.0);
    r.chair = j.value("chair", true);
    r.desk = j.value("desk", true);
    r.fan = j.value("fan", true);
    r.showerHeater = j.value("showerHeater", true);
    r.internet = j.value("internet", true);
    r.airConditioner = j.value("airConditioner", true);
    return r;
}

VipRoom VipRoom::deserialize(const std::string& jsonStr) {
    try {
        nlohmann::json j = nlohmann::json::parse(jsonStr);
        return fromJson(j);
    } catch (...) {
        return VipRoom();
    }
}
