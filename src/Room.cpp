#include "Room.h"
#include "Rent.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

// =========================================================================
// 1. CÀI ĐẶT BROKENDEVICE (THIẾT BỊ HỎNG)
// =========================================================================

BrokenDevice::BrokenDevice()
    : deviceId(""), deviceName(""), description(""), reportDate(Date()), isRepaired(false) {}

BrokenDevice::BrokenDevice(const std::string& id, const std::string& name,
                           const std::string& desc, const Date& date, bool repaired)
    : deviceId(id), deviceName(name), description(desc), reportDate(date), isRepaired(repaired) {}

std::string BrokenDevice::getDeviceId() const { return deviceId; }
std::string BrokenDevice::getDeviceName() const { return deviceName; }
std::string BrokenDevice::getDescription() const { return description; }
Date BrokenDevice::getReportDate() const { return reportDate; }
bool BrokenDevice::isFixed() const { return isRepaired; }

void BrokenDevice::setDeviceId(const std::string& id) { deviceId = id; }
void BrokenDevice::setDeviceName(const std::string& name) { deviceName = name; }
void BrokenDevice::setDescription(const std::string& desc) { description = desc; }
void BrokenDevice::setReportDate(const Date& date) { reportDate = date; }
void BrokenDevice::setFixed(bool fixed) { isRepaired = fixed; }

void BrokenDevice::print() const {
    std::cout << "[" << std::setw(6) << deviceId << "] "
              << std::left << std::setw(15) << deviceName << " | "
              << "Mô tả: " << std::setw(25) << description << " | "
              << "Ngày báo: " << reportDate.toString() << " | "
              << "Trạng thái: " << (isRepaired ? "ĐÃ SỬA XONG" : "⚠️ ĐANG HỎNG") << "\n";
}

nlohmann::json BrokenDevice::toJson() const {
    return {
        {"deviceId", deviceId},
        {"deviceName", deviceName},
        {"description", description},
        {"reportDate", reportDate.toString()},
        {"isRepaired", isRepaired}
    };
}

BrokenDevice BrokenDevice::fromJson(const nlohmann::json& j) {
    BrokenDevice d;
    d.deviceId = j.value("deviceId", "");
    d.deviceName = j.value("deviceName", "");
    d.description = j.value("description", "");
    d.reportDate = Date::fromString(j.value("reportDate", "01/01/2026"));
    d.isRepaired = j.value("isRepaired", false);
    return d;
}

// =========================================================================
// 2. CÀI ĐẶT ROOM
// =========================================================================

Room::Room()
    : roomId(""), building("Khu A"), floor(1), maxStudents(8),
      basePrice(0.0), status(RoomStatus::EMPTY) {}

Room::Room(const std::string& id, int maxStd, int fl, const std::string& bld, double price, RoomStatus st)
    : roomId(id), building(bld), floor(fl), maxStudents(maxStd), basePrice(price), status(st) {}

Room::~Room() {
    activeRent.clear();
    brokenDevices.clear();
}

std::string Room::getRoomId() const { return roomId; }
int Room::getMaxStudents() const { return maxStudents; }
RoomStatus Room::getStatus() const { return status; }
int Room::getFloor() const { return floor; }
std::string Room::getBuilding() const { return building; }
double Room::getBasePrice() const { return basePrice; }
const std::vector<Rent*>& Room::getActiveRent() const { return activeRent; }

std::string Room::getAmenities() const {
    return getEquipmentList();
}

bool Room::hasEquipment(const std::string& deviceName) const {
    std::string eq = getEquipmentList();
    return eq.find(deviceName) != std::string::npos;
}

bool Room::isEquipmentWorking(const std::string& deviceName) const {
    if (!hasEquipment(deviceName)) return false;
    for (const auto& dev : brokenDevices) {
        if (dev.getDeviceName().find(deviceName) != std::string::npos && !dev.isFixed()) {
            return false;
        }
    }
    return true;
}

const std::vector<BrokenDevice>& Room::getBrokenDevices() const {
    return brokenDevices;
}

void Room::addBrokenDevice(const BrokenDevice& dev) {
    brokenDevices.push_back(dev);
}

bool Room::removeBrokenDevice(const std::string& deviceId) {
    for (auto it = brokenDevices.begin(); it != brokenDevices.end(); ++it) {
        if (it->getDeviceId() == deviceId || it->getDeviceName() == deviceId) {
            brokenDevices.erase(it);
            return true;
        }
    }
    return false;
}

bool Room::fixBrokenDevice(const std::string& deviceId) {
    for (auto& dev : brokenDevices) {
        if (dev.getDeviceId() == deviceId || dev.getDeviceName() == deviceId) {
            dev.setFixed(true);
            return true;
        }
    }
    return false;
}

int Room::countUnfixedDevices() const {
    int count = 0;
    for (const auto& dev : brokenDevices) {
        if (!dev.isFixed()) {
            count++;
        }
    }
    return count;
}

bool Room::hasBrokenDevices() const {
    return !brokenDevices.empty();
}

void Room::printBrokenDevices() const {
    if (brokenDevices.empty()) {
        std::cout << "  (Tất cả thiết bị phòng " << roomId << " đều hoạt động bình thường, không có báo hỏng)\n";
        return;
    }
    std::cout << "  --- DANH SÁCH THIẾT BỊ BÁO HỎNG PHÒNG " << roomId << " ---\n";
    for (size_t i = 0; i < brokenDevices.size(); ++i) {
        std::cout << "  " << (i + 1) << ". ";
        brokenDevices[i].print();
    }
}

void Room::setRoomId(const std::string& id) { roomId = id; }
void Room::setMaxStudents(int maxStd) { maxStudents = maxStd; }
void Room::setStatus(RoomStatus st) { status = st; }
void Room::setFloor(int fl) { floor = fl; }
void Room::setBuilding(const std::string& bld) { building = bld; }
void Room::setBasePrice(double price) { basePrice = price; }

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
              << " | Giá: " << std::fixed << std::setprecision(0) << basePrice << "đ"
              << " | Trạng thái: " << std::setw(10) << EnumHelper::roomStatusToString(status)
              << "\n   >> Trang thiết bị : " << getEquipmentList();
    int unfixed = countUnfixedDevices();
    if (unfixed > 0) {
        std::cout << "\n   >> ⚠️ Thiết bị hỏng: " << unfixed << " thiết bị đang chờ sửa chữa!";
    }
    std::cout << std::endl;
}

// =========================================================================
// 3. CÀI ĐẶT STANDARDROOM
// =========================================================================

StandardRoom::StandardRoom()
    : Room("", 8, 1, "Khu A", 400000.0) {}

StandardRoom::StandardRoom(const std::string& rId, int fl, const std::string& bld,
                           double price, RoomStatus st)
    : Room(rId, 8, fl, bld, price, st) {}

std::string StandardRoom::getRoomType() const { return "STANDARD"; }

std::string StandardRoom::getEquipmentList() const {
    return "Bàn học, Ghế, Quạt trần";
}

double StandardRoom::calculateTotalFee(double electricUsage, double waterUsage) const {
    return basePrice + (electricUsage * 3000.0) + (waterUsage * 10000.0);
}

nlohmann::json StandardRoom::toJson() const {
    nlohmann::json jDevices = nlohmann::json::array();
    for (const auto& dev : brokenDevices) {
        jDevices.push_back(dev.toJson());
    }
    return {
        {"roomId", roomId},
        {"roomType", "STANDARD"},
        {"building", building},
        {"floor", floor},
        {"maxStudents", maxStudents},
        {"basePrice", basePrice},
        {"status", EnumHelper::roomStatusToString(status)},
        {"brokenDevices", jDevices}
    };
}

std::string StandardRoom::serialize() const { return toJson().dump(2); }

StandardRoom StandardRoom::fromJson(const nlohmann::json& j) {
    StandardRoom r;
    r.roomId = j.value("roomId", "");
    r.building = j.value("building", "Khu A");
    r.floor = j.value("floor", 1);
    r.maxStudents = j.value("maxStudents", 8);
    r.basePrice = j.value("basePrice", 400000.0);
    r.status = EnumHelper::stringToRoomStatus(j.value("status", "EMPTY"));
    if (j.contains("brokenDevices") && j["brokenDevices"].is_array()) {
        for (const auto& dItem : j["brokenDevices"]) {
            r.brokenDevices.push_back(BrokenDevice::fromJson(dItem));
        }
    }
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
// 4. CÀI ĐẶT VIPROOM
// =========================================================================

VipRoom::VipRoom()
    : Room("", 4, 1, "Khu VIP", 1200000.0), serviceFee(250000.0) {}

VipRoom::VipRoom(const std::string& rId, int fl, const std::string& bld,
                 double price, double sFee, RoomStatus st)
    : Room(rId, 4, fl, bld, price, st), serviceFee(sFee) {}

double VipRoom::getServiceFee() const { return serviceFee; }
void VipRoom::setServiceFee(double sFee) { serviceFee = sFee; }

std::string VipRoom::getRoomType() const { return "VIP"; }

std::string VipRoom::getEquipmentList() const {
    return "Bàn ghế cao cấp, Quạt, Điều hòa, Bình nóng lạnh, Wifi";
}

double VipRoom::calculateTotalFee(double electricUsage, double waterUsage) const {
    return basePrice + serviceFee + (electricUsage * 3000.0) + (waterUsage * 10000.0);
}

nlohmann::json VipRoom::toJson() const {
    nlohmann::json jDevices = nlohmann::json::array();
    for (const auto& dev : brokenDevices) {
        jDevices.push_back(dev.toJson());
    }
    return {
        {"roomId", roomId},
        {"roomType", "VIP"},
        {"building", building},
        {"floor", floor},
        {"maxStudents", maxStudents},
        {"basePrice", basePrice},
        {"serviceFee", serviceFee},
        {"status", EnumHelper::roomStatusToString(status)},
        {"brokenDevices", jDevices}
    };
}

std::string VipRoom::serialize() const { return toJson().dump(2); }

VipRoom VipRoom::fromJson(const nlohmann::json& j) {
    VipRoom r;
    r.roomId = j.value("roomId", "");
    r.building = j.value("building", "Khu VIP");
    r.floor = j.value("floor", 1);
    r.maxStudents = j.value("maxStudents", 4);
    r.basePrice = j.value("basePrice", 1200000.0);
    r.serviceFee = j.value("serviceFee", 250000.0);
    r.status = EnumHelper::stringToRoomStatus(j.value("status", "EMPTY"));
    if (j.contains("brokenDevices") && j["brokenDevices"].is_array()) {
        for (const auto& dItem : j["brokenDevices"]) {
            r.brokenDevices.push_back(BrokenDevice::fromJson(dItem));
        }
    }
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
