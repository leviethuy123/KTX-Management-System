#ifndef RENT_H
#define RENT_H

#include "Common.h"

class Student;
class Room;

class Rent : public IEntity {
private:
    std::string rentId;
    Student* tenant;
    Room* room;
    Period rentalPeriod;
    Date startDate;
    Date actualEndDate;
    double rentFee;
    RentStatus status;

public:
    Rent();
    Rent(const std::string& id, Student* student, Room* rm,
         Period pe, const Date& start, double fee,
         RentStatus st = RentStatus::ACTIVE);
    virtual ~Rent();

    std::string getId() const override;
    RentStatus getStatus() const;
    void setStatus(RentStatus st);
    Student* getTenant() const;
    Room* getRoom() const;
    void setRoom(Room* rm);
    Period getRentalPeriod() const;
    Date getStartDate() const;
    Date getActualEndDate() const;
    double getRentFee() const;

    void setRentId(const std::string& id);
    void setTenant(Student* s);
    void setRentalPeriod(Period pe);
    void setStartDate(const Date& sDate);
    void setActualEndDate(const Date& aDate);
    void setRentFee(double fee);

    Date getExpectedEndDate() const;
    void terminateEarly(Date endDate);
    bool isActive() const;
    double calculateTotalAmount() const;

    void print() const override;
    nlohmann::json toJson() const override;
    std::string serialize() const override;

    static Rent fromJson(const nlohmann::json& j, Student* s = nullptr, Room* r = nullptr);
    static Rent deserialize(const std::string& jsonStr, Student* s = nullptr, Room* r = nullptr);
};

#endif // RENT_H
