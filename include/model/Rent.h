#ifndef RENT_H
#define RENT_H

#include "common/IEntity.h"
#include "common/Date.h"
#include "common/Enums.h"
#include <string>


class Rent : public IEntity {
private:
    std::string rentId;      
    std::string studentId;   
    std::string roomId;     
    Date startDate;         
    Date endDate;            
    int durationMonths;      
    double monthlyFee;      
    RentStatus status;      

public:
    Rent();
    Rent(const std::string& rId, const std::string& sId, const std::string& rmId,
         const Date& sDate, int months, double fee,
         RentStatus st = RentStatus::ACTIVE);

   
    std::string getRentId() const;
    std::string getStudentId() const;
    std::string getRoomId() const;
    Date getStartDate() const;
    Date getEndDate() const;
    int getDurationMonths() const;
    double getMonthlyFee() const;
    RentStatus getStatus() const;

    void setRentId(const std::string& rId);
    void setStudentId(const std::string& sId);
    void setRoomId(const std::string& rmId);
    void setStartDate(const Date& sDate);
    void setEndDate(const Date& eDate);
    void setDurationMonths(int months);
    void setMonthlyFee(double fee);
    void setStatus(RentStatus st);

    
    bool isActive() const;
    double calculateTotalAmount() const;
    void terminateEarly(const Date& actualEndDate);

   
    std::string getId() const override;
    void print() const override;
    std::string serialize() const override;

    static Rent deserialize(const std::string& line);
};

#endif 
