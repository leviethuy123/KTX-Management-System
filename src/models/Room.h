#ifndef ROOM_H
#define ROOM_H


#include<string>
#include<vector>
class Student;
class Room
{
    private:
        string roomNumber;
        vector<Student*> studentListInRoon;
        int maxStudents;
        double basePrice;
        double waterUsage;
        double electricityUsage;
    public:
        Room();
        ~Room();

        void addStudent(const Student* &studentNum);
        void removeStudent(const Student* &studentNum);
        double calculateTotalFee();
        bool isEmpty() const;
        double getWaterUssage() const;
        double getElectricityUssage() const;
        string getRoomNumber() const;
        bool isStudentInRoom(const string studentNum) const;
};
#endif


