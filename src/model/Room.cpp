#include "model/Room.h"
#include <iostream>
#include <iomanip>

Room::Room()
    : roomId(""), building(""), floor(1), status(RoomStatus::EMPTY), currentOccupants(0) {}

Room::Room(const std::string& id, const std::string& bld, int fl,
           RoomStatus st, int occ)
    : roomId(id), building(bld), floor(fl), status(st), currentOccupants(occ) {}

std::string Room::getRoomId() const { return roomId; }
std::string Room::getBuilding() const { return building; }
int Room::getFloor() const { return floor; }
RoomStatus Room::getStatus() const { return status; }
int Room::getCurrentOccupants() const { return currentOccupants; }

void Room::setRoomId(const std::string& id) { roomId = id; }
void Room::setBuilding(const std::string& bld) { building = bld; }
void Room::setFloor(int fl) { floor = fl; }
void Room::setStatus(RoomStatus st) { status = st; }
void Room::setCurrentOccupants(int occ) { currentOccupants = occ; }

bool Room::isFull() const {
    return currentOccupants >= getMaxStudents();
}

bool Room::isEmpty() const {
    return currentOccupants == 0;
}

bool Room::addOccupant() {
    if (isFull() || status == RoomStatus::MAINTENANCE) {
        return false;
    }
    currentOccupants++;
    if (isFull()) {
        status = RoomStatus::FULL;
    } else {
        status = RoomStatus::OCCUPIED;
    }
    return true;
}

bool Room::removeOccupant() {
    if (isEmpty()) {
        return false;
    }
    currentOccupants--;
    if (isEmpty()) {
        status = RoomStatus::EMPTY;
    } else {
        status = RoomStatus::OCCUPIED;
    }
    return true;
}

std::string Room::getId() const {
    return roomId;
}

void Room::print() const {
    std::cout << "[PHÒNG " << std::left << std::setw(6) << roomId << "] "
              << "Tòa: " << std::setw(8) << building
              << " | Tầng: " << floor
              << " | Loại: " << std::setw(9) << EnumHelper::roomTypeToString(getRoomType())
              << " | Đang ở: " << currentOccupants << "/" << getMaxStudents()
              << " | Trạng thái: " << std::setw(10) << EnumHelper::roomStatusToString(status)
              << " | Tiện ích: " << getAmenities() << std::endl;
}
