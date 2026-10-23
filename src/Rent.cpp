#include "Rent.h"
#include "Human.h"
#include "Room.h"

Rent::Rent()
    : rentId(""), tenant(nullptr), room(nullptr),
      rentalPeriod(Period::ONE_MONTH), startDate(Date()), actualEndDate(Date()),
      rentFee(0.0), status(RentStatus::ACTIVE) {}

Rent::Rent(const std::string& id, Student* student, Room* rm,
           Period pe, const Date& start, double fee, RentStatus st)
    : rentId(id), tenant(student), room(rm),
      rentalPeriod(pe), startDate(start), rentFee(fee), status(st) {
    actualEndDate = getExpectedEndDate();
}

Rent::~Rent() {
    tenant = nullptr;
    room = nullptr;
}

std::string Rent::getId() const { return rentId; }
RentStatus Rent::getStatus() const { return status; }
void Rent::setStatus(RentStatus st) { status = st; }
Student* Rent::getTenant() const { return tenant; }
Room* Rent::getRoom() const { return room; }
void Rent::setRoom(Room* rm) { room = rm; }
Period Rent::getRentalPeriod() const { return rentalPeriod; }
Date Rent::getStartDate() const { return startDate; }
Date Rent::getActualEndDate() const { return actualEndDate; }
double Rent::getRentFee() const { return rentFee; }

void Rent::setRentId(const std::string& id) { rentId = id; }
void Rent::setTenant(Student* s) { tenant = s; }
void Rent::setRentalPeriod(Period pe) { rentalPeriod = pe; }
void Rent::setStartDate(const Date& sDate) { startDate = sDate; }
void Rent::setActualEndDate(const Date& aDate) { actualEndDate = aDate; }
void Rent::setRentFee(double fee) { rentFee = fee; }

Date Rent::getExpectedEndDate() const {
    int months = EnumHelper::periodToMonths(rentalPeriod);
    return startDate.addMonths(months);
}

void Rent::terminateEarly(Date endDate) {
    actualEndDate = endDate;
    status = RentStatus::TERMINATED;
}

bool Rent::isActive() const {
    return status == RentStatus::ACTIVE;
}

double Rent::calculateTotalAmount() const {
    int months = EnumHelper::periodToMonths(rentalPeriod);
    return months * rentFee;
}

void Rent::print() const {
    std::cout << "[HỢP ĐỒNG " << std::left << std::setw(7) << rentId << "] "
              << "SV: " << std::setw(8) << (tenant ? tenant->getId() : "N/A")
              << " | Phòng: " << std::setw(6) << (room ? room->getId() : "N/A")
              << " | Bắt đầu: " << startDate.toString()
              << " | Hết hạn: " << actualEndDate.toString()
              << " | Thời hạn: " << std::setw(8) << EnumHelper::periodToString(rentalPeriod)
              << " | Giá thuê: " << std::fixed << std::setprecision(0) << rentFee << " VNĐ"
              << " | Trạng thái: " << EnumHelper::rentStatusToString(status) << std::endl;
}

nlohmann::json Rent::toJson() const {
    return {
        {"rentId", rentId},
        {"studentId", tenant ? tenant->getId() : ""},
        {"roomId", room ? room->getId() : ""},
        {"rentalPeriod", EnumHelper::periodToMonths(rentalPeriod)},
        {"startDate", startDate.toString()},
        {"actualEndDate", actualEndDate.toString()},
        {"rentFee", rentFee},
        {"status", EnumHelper::rentStatusToString(status)}
    };
}

std::string Rent::serialize() const { return toJson().dump(2); }

Rent Rent::fromJson(const nlohmann::json& j, Student* s, Room* r) {
    std::string id = j.value("rentId", "");
    int months = j.value("rentalPeriod", 1);
    Date sDate = Date::fromString(j.value("startDate", "01/01/2026"));
    Date aDate = Date::fromString(j.value("actualEndDate", "01/01/2026"));
    double fee = j.value("rentFee", 0.0);
    RentStatus st = EnumHelper::stringToRentStatus(j.value("status", "ACTIVE"));

    Rent rentObj(id, s, r, EnumHelper::intToPeriod(months), sDate, fee, st);
    rentObj.setActualEndDate(aDate);
    return rentObj;
}

Rent Rent::deserialize(const std::string& jsonStr, Student* s, Room* r) {
    try {
        nlohmann::json j = nlohmann::json::parse(jsonStr);
        return fromJson(j, s, r);
    } catch (...) {
        return Rent();
    }
}
