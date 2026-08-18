#ifndef STUDENT_H
#define STUDENT_H

#include <string>
class Room;

class Student {
private:
    std::string studentId;
    std::string studentName;
    std::string studentClass;
    Room* studentRoom;

public:
    Student();
    Student(const std::string &id, const std::string &name, const std::string &sClass, Room* room=nullptr);

    virtual ~Student();

    std::string getStudentId() const;
    std::string getStudentName() const;
    std::string getStudentClass() const;
    Room* getStudentRoom() const;

    void setStudentId(const std::string &id);
    void setStudentName(const std::string &name);
    void setStudentClass(const std::string &sClass);
    void setStudentRoom(Room* room);
};
#endif 