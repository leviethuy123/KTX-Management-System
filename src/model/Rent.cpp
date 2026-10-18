#include "model/Rent.h"
#include "model/Student.h"
#include "model/Room.h"
#include <iostream>
#include <iomanip>

Rent::Rent()
    : rentId(""), tenant(nullptr), room(nullptr),
      rentalPeriod(Period::ONE_MONTH), startDate(Date()),
      actualEndDate(Date()), rentFee(0.0), status(RentStatus::ACTIVE) {}

Rent::Rent(const std::string& id, Student* student, Room* rm,
           Period pe, const Date& start, double fee, RentStatus st)
    : rentId(id), tenant(student), room(rm),
      rentalPeriod(pe), startDate(start),
      actualEndDate(start.addMonths(EnumHelper::periodToMonths(pe))),
      rentFee(fee), status(st) {}

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
    return startDate.addMonths(EnumHelper::periodToMonths(rentalPeriod));
}

void Rent::terminateEarly(Date endDate) {
    actualEndDate = endDate;
    status = RentStatus::TERMINATED;
}

bool Rent::isActive() const {
    return status == RentStatus::ACTIVE;
}

double Rent::calculateTotalAmount() const {
    return EnumHelper::periodToMonths(rentalPeriod) * rentFee;
}

void Rent::print() const {
    std::string sId = (tenant != nullptr) ? tenant->getId() : "N/A";
    std::string rId = (room != nullptr) ? room->getId() : "N/A";

    std::cout << "[HĐ " << std::left << std::setw(6) << rentId << "] "
              << "Sinh viên: " << std::setw(8) << sId
              << " | Phòng: " << std::setw(6) << rId
              << " | Thời hạn: " << std::setw(8) << EnumHelper::periodToString(rentalPeriod)
              << " | Từ: " << startDate.toString()
              << " Đến: " << actualEndDate.toString()
              << " | Đơn giá: " << static_cast<long long>(rentFee) << "đ/tháng"
              << " | Trạng thái: " << EnumHelper::rentStatusToString(status)
              << std::endl;
}

nlohmann::json Rent::toJson() const {
    nlohmann::json j;
    j["rentId"] = rentId;
    j["studentId"] = (tenant != nullptr) ? tenant->getId() : "";
    j["roomId"] = (room != nullptr) ? room->getId() : "";
    j["rentalPeriod"] = EnumHelper::periodToMonths(rentalPeriod);
    j["startDate"] = startDate.toString();
    j["actualEndDate"] = actualEndDate.toString();
    j["rentFee"] = rentFee;
    j["status"] = EnumHelper::rentStatusToString(status);
    return j;
}

std::string Rent::serialize() const {
    return toJson().dump(2);
}

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
