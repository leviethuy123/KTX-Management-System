#ifndef ROOM_H
#define ROOM_H

#include "common/IEntity.h"
#include "common/Enums.h"
#include <string>


class Room : public IEntity {
protected:
    std::string roomId;       
    std::string building;    
    int floor;                
    RoomStatus status;       
    int currentOccupants;    

public:
    Room();
    Room(const std::string& id, const std::string& bld, int fl,
         RoomStatus st = RoomStatus::EMPTY, int occ = 0);
    virtual ~Room() = default;

  
    std::string getRoomId() const;
    std::string getBuilding() const;
    int getFloor() const;
    RoomStatus getStatus() const;
    int getCurrentOccupants() const;

    void setRoomId(const std::string& id);
    void setBuilding(const std::string& bld);
    void setFloor(int fl);
    void setStatus(RoomStatus st);
    void setCurrentOccupants(int occ);

   
    bool isFull() const;
    bool isEmpty() const;
    bool addOccupant();
    bool removeOccupant();

  
    virtual int getMaxStudents() const = 0;
    virtual RoomType getRoomType() const = 0;
    virtual double getBasePrice() const = 0;
    virtual double calculateTotalFee(double electricUsage = 0, double waterUsage = 0) const = 0;
    virtual std::string getAmenities() const = 0;


    std::string getId() const override;
    void print() const override;
};

#endif 
