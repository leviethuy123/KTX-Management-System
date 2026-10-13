#ifndef HUMAN_H
#define HUMAN_H

#include "common/IEntity.h"
#include "common/Date.h"
#include "common/Enums.h"
#include <string>


class Human : public IEntity {
protected:
    std::string fullName;
    Gender gender;
    Date birthDate;
    std::string phone;
    std::string email;
    std::string address;

public:
    Human();
    Human(const std::string& name, Gender g, const Date& birth,
          const std::string& p, const std::string& em, const std::string& addr);
    virtual ~Human() = default;

  
    std::string getFullName() const;
    Gender getGender() const;
    Date getBirthDate() const;
    std::string getPhone() const;
    std::string getEmail() const;
    std::string getAddress() const;


    void setFullName(const std::string& name);
    void setGender(Gender g);
    void setBirthDate(const Date& birth);
    void setPhone(const std::string& p);
    void setEmail(const std::string& em);
    void setAddress(const std::string& addr);

    std::string getId() const override = 0;
};

#endif 
