#include "model/Staff.h"
#include <iostream>
#include <iomanip>

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

nlohmann::json Staff::toJson() const {
    nlohmann::json j;
    j["employeeId"] = employeeId;
    j["fullName"] = fullName;
    j["gender"] = gender;
    j["birthYear"] = birthYear;
    j["phone"] = phone;
    j["email"] = email;
    j["address"] = address;
    j["department"] = department;
    j["position"] = position;
    return j;
}

std::string Staff::serialize() const {
    return toJson().dump(2);
}

Staff Staff::fromJson(const nlohmann::json& j) {
    std::string id = j.value("employeeId", "");
    std::string name = j.value("fullName", "");
    bool g = j.value("gender", true);
    std::string birth = j.value("birthYear", "");
    std::string phone = j.value("phone", "");
    std::string email = j.value("email", "");
    std::string addr = j.value("address", "");
    std::string dept = j.value("department", "");
    std::string pos = j.value("position", "");

    return Staff(id, name, g, birth, phone, email, addr, dept, pos);
}

Staff Staff::deserialize(const std::string& jsonStr) {
    try {
        nlohmann::json j = nlohmann::json::parse(jsonStr);
        return fromJson(j);
    } catch (...) {
        return Staff();
    }
}
