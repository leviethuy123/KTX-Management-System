#include "model/Rent.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

Rent::Rent()
    : rentId(""), studentId(""), roomId(""),
      startDate(Date()), endDate(Date()),
      durationMonths(0), monthlyFee(0.0), status(RentStatus::ACTIVE) {}

Rent::Rent(const std::string& rId, const std::string& sId, const std::string& rmId,
           const Date& sDate, int months, double fee, RentStatus st)
    : rentId(rId), studentId(sId), roomId(rmId),
      startDate(sDate), endDate(sDate.addMonths(months)),
      durationMonths(months), monthlyFee(fee), status(st) {}

std::string Rent::getRentId() const { return rentId; }
std::string Rent::getStudentId() const { return studentId; }
std::string Rent::getRoomId() const { return roomId; }
Date Rent::getStartDate() const { return startDate; }
Date Rent::getEndDate() const { return endDate; }
int Rent::getDurationMonths() const { return durationMonths; }
double Rent::getMonthlyFee() const { return monthlyFee; }
RentStatus Rent::getStatus() const { return status; }

void Rent::setRentId(const std::string& rId) { rentId = rId; }
void Rent::setStudentId(const std::string& sId) { studentId = sId; }
void Rent::setRoomId(const std::string& rmId) { roomId = rmId; }
void Rent::setStartDate(const Date& sDate) { startDate = sDate; }
void Rent::setEndDate(const Date& eDate) { endDate = eDate; }
void Rent::setDurationMonths(int months) { durationMonths = months; }
void Rent::setMonthlyFee(double fee) { monthlyFee = fee; }
void Rent::setStatus(RentStatus st) { status = st; }

bool Rent::isActive() const {
    return status == RentStatus::ACTIVE;
}

double Rent::calculateTotalAmount() const {
    return durationMonths * monthlyFee;
}

void Rent::terminateEarly(const Date& actualEndDate) {
    endDate = actualEndDate;
    status = RentStatus::TERMINATED;
}

std::string Rent::getId() const {
    return rentId;
}

void Rent::print() const {
    std::cout << "[HĐ " << std::left << std::setw(6) << rentId << "] "
              << "Sinh viên: " << std::setw(8) << studentId
              << " | Phòng: " << std::setw(6) << roomId
              << " | Từ: " << startDate.toString()
              << " Đến: " << endDate.toString()
              << " (" << durationMonths << " tháng)"
              << " | Giá/tháng: " << static_cast<long long>(monthlyFee) << "đ"
              << " | Trạng thái: " << EnumHelper::rentStatusToString(status)
              << std::endl;
}

std::string Rent::serialize() const {
    std::ostringstream oss;
    oss << rentId << "|"
        << studentId << "|"
        << roomId << "|"
        << startDate.toString() << "|"
        << endDate.toString() << "|"
        << durationMonths << "|"
        << std::fixed << std::setprecision(0) << monthlyFee << "|"
        << EnumHelper::rentStatusToString(status);
    return oss.str();
}

Rent Rent::deserialize(const std::string& line) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, '|')) {
        tokens.push_back(token);
    }

    if (tokens.size() < 8) {
        return Rent();
    }

    std::string rId = tokens[0];
    std::string sId = tokens[1];
    std::string rmId = tokens[2];
    Date sDate = Date::fromString(tokens[3]);
    Date eDate = Date::fromString(tokens[4]);
    int months = std::stoi(tokens[5]);
    double fee = std::stod(tokens[6]);
    RentStatus st = EnumHelper::stringToRentStatus(tokens[7]);

    Rent r(rId, sId, rmId, sDate, months, fee, st);
    r.setEndDate(eDate);
    return r;
}
