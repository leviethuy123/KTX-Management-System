#include "Room.h"
#include "Student.h"

using namespace std;

Room::Room() :roomNumber("") , maxStudents(0) , basePrice(0.0) , waterUsage(0.0) , electricityUsage(0.0)
{

}
Room::~Room()
{
    studentListInRoon.clear();
}
void Room::addStudent(const Student* &studentNum)
{
    studentListInRoon.push_back(const_cast<Student*>(studentNum));
}
void Room::removeStudent(const Student* &studentNum)
{
    for(auto i=studentListInRoon.begin(); i != studentListInRoon.end();i++)
        if(*i==studentNum)
        {
            studentListInRoon.erase(i);
            break;
        }
}
double Room::calculateTotalFee()
{
    return basePrice+waterUsage+electricityUsage;
}
bool Room::isEmpty() const
{
    return studentListInRoon.empty();
}
double Room::getWaterUssage() const
{
    return waterUsage;
}
double Room::getElectricityUssage() const 
{
    return electricityUsage;
}
bool Room::isStudentInRoom(const string studentNum) const{
    for(Student* s:studentListInRoon)
    {
        if (s != nullptr && s->getStudentId() ==studentNum)
            return true;
    }
    return false;
}