#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H
#include <string>
#include <vector>
class Room; 
class RoomManager
{
    private:
        std::vector<Room*> roomList;
    public:
        RoomManager();
        ~RoomManager();

        void addRoom(Room* newRoom);
        Room* findRoom(const std::string roomld) const;
        Room* findEmptyRoom() const;

        void printRoom(const std::string roomld) const;
        void printAllRoom() const;
        void printEmptyRoom() const;
        void printNotEmptyRoom() const;

        double calculateWaterCost(const std::string roomld) const;
        double calculateElectricityCost(const std::string roomld) const;
        double calculateUtilityCost(const std::string roomld) const;
};

#endif