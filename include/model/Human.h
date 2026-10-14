#ifndef HUMAN_H
#define HUMAN_H

#include "common/IEntity.h"
#include "common/Date.h"
#include "common/Enums.h"
#include <string>

/**
 * @brief Lớp cơ sở trừu tượng Human (theo sơ đồ XML).
 * Chứa các thông tin cá nhân cơ bản: họ tên, giới tính, năm sinh/ngày sinh, SĐT, email, địa chỉ.
 */
class Human : public IEntity {
protected:
    std::string fullName;
    bool gender;              // true: Nam, false: Nu (khớp XML: gender: bool)
    std::string birthYear;    // Khớp XML: birthYear: string
    std::string phone;
    std::string email;
    std::string address;

public:
    Human();
    Human(const std::string& name, bool g, const std::string& birth,
          const std::string& p, const std::string& em = "", const std::string& addr = "");
    virtual ~Human() = default;

    // Getters
    std::string getFullName() const;
    bool getGender() const;
    std::string getBirthYear() const;
    std::string getPhone() const;
    std::string getEmail() const;
    std::string getAddress() const;

    // Setters
    void setFullName(const std::string& name);
    void setGender(bool g);
    void setBirthYear(const std::string& birth);
    void setPhone(const std::string& p);
    void setEmail(const std::string& em);
    void setAddress(const std::string& addr);

    // getId() là thuần ảo để Student và Staff cài đặt
    std::string getId() const override = 0;
};

#endif // HUMAN_H
