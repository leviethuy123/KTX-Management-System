#include "model/Room.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

Room::Room()
    : roomId(""), maxStudents(0), status(RoomStatus::EMPTY),
      floor(1), building(""), chair(true), desk(true), fan(true) {}

Room::Room(const std::string& id, int maxStd, int fl, const std::string& bld, RoomStatus st)
    : roomId(id), maxStudents(maxStd), status(st),
      floor(fl), building(bld), chair(true), desk(true), fan(true) {}

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
        if (isFull()) {
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

bool Room::isEmpty() const {
    return activeRent.empty();
}

bool Room::isFull() const {
    return static_cast<int>(activeRent.size()) >= maxStudents;
}

std::string Room::getId() const {
    return roomId;
}

void Room::print() const {
    std::cout << "[PHÒNG " << std::left << std::setw(6) << roomId << "] "
              << "Tòa: " << std::setw(8) << building
              << " | Tầng: " << floor
              << " | Loại: " << std::setw(9) << getRoomType()
              << " | Đang ở: " << activeRent.size() << "/" << maxStudents
              << " | Trạng thái: " << std::setw(10) << EnumHelper::roomStatusToString(status)
              << " | Tiện ích: " << getAmenities() << std::endl;
}
