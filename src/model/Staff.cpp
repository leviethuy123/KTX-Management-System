#include "model/Staff.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

Staff::Staff()
    : Human(), employeeId(""), department(""), position("") {}

Staff::Staff(const std::string& id, const std::string& name, bool g,
             const std::string& birth, const std::string& p, const std::string& em,
             const std::string& addr, const std::string& dept,
             const std::string& pos)
    : Human(name, g, birth, p, em, addr),
      employeeId(id), department(dept), position(pos) {}

std::string Staff::getEmployeeId() const { return employeeId; }
std::string Staff::getDepartment() const { return department; }
std::string Staff::getPosition() const { return position; }

void Staff::setEmployeeId(const std::string& id) { employeeId = id; }
void Staff::setDepartment(const std::string& newDepartment) { department = newDepartment; }
void Staff::setPosition(const std::string& newPosition) { position = newPosition; }

std::string Staff::getId() const {
    return employeeId;
}

void Staff::print() const {
    std::cout << "[CB] " << std::left << std::setw(8) << employeeId
              << " | " << std::setw(20) << fullName
              << " | " << std::setw(6) << (gender ? "Nam" : "Nu")
              << " | Phòng ban: " << std::setw(18) << department
              << " | Chức vụ: " << std::setw(15) << position
              << " | SĐT: " << phone << std::endl;
}

std::string Staff::serialize() const {
    std::ostringstream oss;
    oss << employeeId << "|"
        << fullName << "|"
        << (gender ? "1" : "0") << "|"
        << birthYear << "|"
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
    bool g = (tokens[2] == "1" || tokens[2] == "Nam");
    std::string birth = tokens[3];
    std::string phone = tokens[4];
    std::string email = tokens[5];
    std::string addr = tokens[6];
    std::string dept = tokens[7];
    std::string pos = tokens[8];

    return Staff(id, name, g, birth, phone, email, addr, dept, pos);
}
