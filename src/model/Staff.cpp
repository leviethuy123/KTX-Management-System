#include "model/Staff.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

Staff::Staff()
    : Human(), employeeId(""), department(""), position("") {}

Staff::Staff(const std::string& id, const std::string& name, Gender g,
             const Date& birth, const std::string& p, const std::string& em,
             const std::string& addr, const std::string& dept,
             const std::string& pos)
    : Human(name, g, birth, p, em, addr),
      employeeId(id), department(dept), position(pos) {}

std::string Staff::getEmployeeId() const { return employeeId; }
std::string Staff::getDepartment() const { return department; }
std::string Staff::getPosition() const { return position; }

void Staff::setEmployeeId(const std::string& id) { employeeId = id; }
void Staff::setDepartment(const std::string& dept) { department = dept; }
void Staff::setPosition(const std::string& pos) { position = pos; }

std::string Staff::getId() const {
    return employeeId;
}

void Staff::print() const {
    std::cout << "[CB] " << std::left << std::setw(8) << employeeId
              << " | " << std::setw(20) << fullName
              << " | " << std::setw(6) << EnumHelper::genderToString(gender)
              << " | Phòng ban: " << std::setw(15) << department
              << " | Chức vụ: " << std::setw(12) << position
              << " | SĐT: " << phone << std::endl;
}

std::string Staff::serialize() const {
    std::ostringstream oss;
    oss << employeeId << "|"
        << fullName << "|"
        << EnumHelper::genderToString(gender) << "|"
        << birthDate.toString() << "|"
        << phone << "|"
        << email << "|"
        << address << "|"
        << department << "|"
        << position;
    return oss.str();
}

Staff Staff::deserialize(const std::string& line) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, '|')) {
        tokens.push_back(token);
    }

    if (tokens.size() < 9) {
        return Staff();
    }

    std::string id = tokens[0];
    std::string name = tokens[1];
    Gender g = EnumHelper::stringToGender(tokens[2]);
    Date birth = Date::fromString(tokens[3]);
    std::string phone = tokens[4];
    std::string email = tokens[5];
    std::string addr = tokens[6];
    std::string dept = tokens[7];
    std::string pos = tokens[8];

    return Staff(id, name, g, birth, phone, email, addr, dept, pos);
}
