#ifndef STUDENT_MANAGER_H
#define STUDENT_MANAGER_H

#include<vector>
#include<string>

class Student;

class StudentManager
{
    private:
        std::vector<Student*> studentsList;
    public:
        StudentManager()
        virtual ~StudentManager();

        Student* registerStudent();
        bool studentExist(const std::string &studentNum) const;
        Student* findStudent(const std::string &studentNum) const;
        void addStudent();
        void removeStudent(const string& studentNum);
        void addStudent(Student *s);
        const std::vector<Student*> getStudentsList() const;
}

#endif