#include "RoomManager.h"
#include "Room.h"
#include<bits/stdc++.h>

using namespace std;

RoomManager::RoomManager(){}
RoomManager::~RoomManager()
{
    for(Room* room : roomList)
        delete room;
    roomList.clear();
}
void RoomManager::addRoom(Room* newRoom)
{
    if (newRoom != nullptr)
        roomList.push_back(newRoom);
}
Room* RoomManager::findRoom(const string roomId) const
{
    for(Room* room : roomList)
    {
        if (room != nullptr && room->getRoomNumber()==roomId)
            return room;
    }
    return nullptr;
}
void RoomManager::printAllRoom() const
{
    if (roomList.empty())
        cout<<"Danh sach phong trong "<<endl;
    else
    {
        for(Room* room : roomList)
        {
            if (room != nullptr)
            {
                cout<<"Phong: "<<room->getRoomNumber()<< " | Dien: "<<room->getElectricityUssage()<< " | Nuoc: "<<room->getWaterUssage()<<endl;
            }
        }
    }
}
void RoomManager::printEmptyRoom() const
{
    cout<<"Danh sach cac phong con trong"<<endl;
    bool f=false;
    for(Room* room : roomList)
    {
        if (room != nullptr && room->isEmpty())
        {
            cout<<"Phong : "<<room->getRoomNumber()<<endl;
            f=true;
        }
    }
    if (!f)
        cout<<"Khong con phong nao trong "<<endl;
}
void RoomManager::printNotEmptyRoom() const
{
    cout<<"Danh sach phong da co ng o"<<endl;
    bool f=false;
    for(Room* room : roomList)
    {
        if (room != nullptr && !room->isEmpty())
        {
            cout<<"Phong : "<<room->getRoomNumber()<<endl;
            f=true;
        }
    }
    if (!f)
        cout<<"Khong co phong nao co dang co ng o"<<endl;
}
double RoomManager::calculateWaterCost(const string roomId) const
{  
}
double RoomManager::calculateElectricityCost(const string roomId) const
{
   
}
double RoomManager::calculateUtilityCost(const string roomId) const
{
    return calculateElectricityCost(roomId)+calculateWaterCost(roomId);
}