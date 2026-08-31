#include "Student.h"
#include "Room.h"

using namespace std;


Student::Student() :studentId(""),studentName(""),studentClass(""),studentRoom(nullptr){}
Student::Student(const string &id,const string &name,const string &sClass, Room* room):studentId(id),studentName(name),studentClass(sClass),studentRoom(room){}
Student::~Student()
{
    
}
string Student::getStudentId() const{
    return studentId;
}
string Student::getStudentName() const{
    return studentName;
}
string Student::getStudentClass() const{
    return studentClass;
}
Room* Student::getStudentRoom() const{
    return studentRoom;
}
void Student::setStudentId(const string &id)
{
    studentId=id;
}
void Student::setStudentName(const string &name)
{
    studentName=name;
}
void Student::setStudentRoom(Room* room)
{
    studentRoom=room;
}
void Student::setStudentClass(const string &sClass) {
    studentClass = sClass;
}