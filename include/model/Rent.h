#ifndef RENT_H
#define RENT_H

#include "common/IEntity.h"
#include "common/Date.h"
#include "common/Enums.h"
#include <string>

class Student;
class Room;

/**
 * @brief Lớp Hợp đồng thuê phòng Rent (Khớp 100% với sơ đồ XML).
 */
class Rent : public IEntity {
private:
    std::string rentId;
    Student* tenant;          // Khớp XML: - tenant: Student*
    Room* room;               // Khớp XML: - room: Room*
    Period rentalPeriod;      // Khớp XML: - rentalPeriod: Period
    Date startDate;           // Khớp XML: - startDate: date
    Date actualEndDate;       // Khớp XML: - actualEndDate: date
    double rentFee;           // Khớp XML: - rentFee: double
    RentStatus status;        // Khớp XML: - status: enum {ACTIVE, EXPIRED, TERMINATED}

public:
    Rent();
    // Constructor khớp XML: Rent(string id, Student* student, Room* room, Period pe, date start, double fee)
    Rent(const std::string& id, Student* student, Room* rm,
         Period pe, const Date& start, double fee,
         RentStatus st = RentStatus::ACTIVE);
    virtual ~Rent();

    // Getters & Setters khớp XML
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

    // Nghiệp vụ khớp XML
    Date getExpectedEndDate() const;
    void terminateEarly(Date endDate);
    bool isActive() const;
    double calculateTotalAmount() const;

    // IEntity
    void print() const override;
    std::string serialize() const override;
    static Rent deserialize(const std::string& line, Student* s = nullptr, Room* r = nullptr);
};

#endif // RENT_H
