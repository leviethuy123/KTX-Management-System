#include "model/Human.h"

Human::Human()
    : fullName(""), gender(Gender::OTHER), birthDate(Date()),
      phone(""), email(""), address("") {}

Human::Human(const std::string& name, Gender g, const Date& birth,
             const std::string& p, const std::string& em, const std::string& addr)
    : fullName(name), gender(g), birthDate(birth),
      phone(p), email(em), address(addr) {}

std::string Human::getFullName() const { return fullName; }
Gender Human::getGender() const { return gender; }
Date Human::getBirthDate() const { return birthDate; }
std::string Human::getPhone() const { return phone; }
std::string Human::getEmail() const { return email; }
std::string Human::getAddress() const { return address; }

void Human::setFullName(const std::string& name) { fullName = name; }
void Human::setGender(Gender g) { gender = g; }
void Human::setBirthDate(const Date& birth) { birthDate = birth; }
void Human::setPhone(const std::string& p) { phone = p; }
void Human::setEmail(const std::string& em) { email = em; }
void Human::setAddress(const std::string& addr) { address = addr; }
