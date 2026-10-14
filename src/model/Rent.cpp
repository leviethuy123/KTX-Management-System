#include "model/Rent.h"
#include "model/Student.h"
#include "model/Room.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

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

std::string Rent::serialize() const {
    std::string sId = (tenant != nullptr) ? tenant->getId() : "";
    std::string rId = (room != nullptr) ? room->getId() : "";

    std::ostringstream oss;
    oss << rentId << "|"
        << sId << "|"
        << rId << "|"
        << EnumHelper::periodToMonths(rentalPeriod) << "|"
        << startDate.toString() << "|"
        << actualEndDate.toString() << "|"
        << std::fixed << std::setprecision(0) << rentFee << "|"
        << EnumHelper::rentStatusToString(status);
    return oss.str();
}

Rent Rent::deserialize(const std::string& line, Student* s, Room* r) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, '|')) {
        tokens.push_back(token);
    }

    if (tokens.size() < 8) {
        return Rent();
    }

    std::string id = tokens[0];
    // tokens[1] là studentId, tokens[2] là roomId
    int months = std::stoi(tokens[3]);
    Date sDate = Date::fromString(tokens[4]);
    Date aDate = Date::fromString(tokens[5]);
    double fee = std::stod(tokens[6]);
    RentStatus st = EnumHelper::stringToRentStatus(tokens[7]);

    Rent rentObj(id, s, r, EnumHelper::intToPeriod(months), sDate, fee, st);
    rentObj.setActualEndDate(aDate);
    return rentObj;
}
