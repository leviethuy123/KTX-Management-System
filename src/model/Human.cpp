#include "model/Human.h"

Human::Human()
    : fullName(""), gender(true), birthYear(""), phone(""), email(""), address("") {}

Human::Human(const std::string& name, bool g, const std::string& birth,
             const std::string& p, const std::string& em, const std::string& addr)
    : fullName(name), gender(g), birthYear(birth),
      phone(p), email(em), address(addr) {}

std::string Human::getFullName() const { return fullName; }
bool Human::getGender() const { return gender; }
std::string Human::getBirthYear() const { return birthYear; }
std::string Human::getPhone() const { return phone; }
std::string Human::getEmail() const { return email; }
std::string Human::getAddress() const { return address; }

void Human::setFullName(const std::string& name) { fullName = name; }
void Human::setGender(bool g) { gender = g; }
void Human::setBirthYear(const std::string& birth) { birthYear = birth; }
void Human::setPhone(const std::string& p) { phone = p; }
void Human::setEmail(const std::string& em) { email = em; }
void Human::setAddress(const std::string& addr) { address = addr; }
