#ifndef MANAGERS_H
#define MANAGERS_H

#include "Common.h"
#include "Human.h"
#include "Room.h"
#include "Rent.h"
#include "Account.h"
#include <fstream>
#include <algorithm>

// =========================================================================
// 1. LỚP KHUÔN MẪU GENERIC MANAGER<T> (REPOSITORY PATTERN)
// =========================================================================

template <typename T>
class Manager {
protected:
    std::vector<T*> list;
    std::string dataFilePath;

public:
    Manager(const std::string& filePath = "") : dataFilePath(filePath) {}

    virtual ~Manager() {
        clear();
    }

    void clear() {
        for (T* item : list) {
            delete item;
        }
        list.clear();
    }

    virtual bool add(T* item) {
        if (item == nullptr) return false;
        if (findById(item->getId()) != nullptr) {
            std::cout << "[Cảnh báo] Mã ID '" << item->getId() << "' đã tồn tại!\n";
            return false;
        }
        list.push_back(item);
        return true;
    }

    virtual bool remove(const std::string& id) {
        for (auto it = list.begin(); it != list.end(); ++it) {
            if ((*it)->getId() == id) {
                delete *it;
                list.erase(it);
                return true;
            }
        }
        return false;
    }

    virtual T* findById(const std::string& id) const {
        for (T* item : list) {
            if (item != nullptr && item->getId() == id) {
                return item;
            }
        }
        return nullptr;
    }

    virtual void printById(const std::string& id) const {
        T* item = findById(id);
        if (item != nullptr) {
            item->print();
        } else {
            std::cout << "Không tìm thấy bản ghi có ID: " << id << "\n";
        }
    }

    virtual void printAll() const {
        if (list.empty()) {
            std::cout << "(Danh sách trống)\n";
            return;
        }
        for (const T* item : list) {
            if (item != nullptr) {
                item->print();
            }
        }
    }

    const std::vector<T*>& getAll() const {
        return list;
    }

    size_t getCount() const {
        return list.size();
    }

    virtual void loadFromFile() = 0;
    virtual void saveToFile() const = 0;
};

// =========================================================================
// 2. LỚP QUẢN LÝ SINH VIÊN (STUDENTMANAGER)
// =========================================================================

class StudentManager : public Manager<Student> {
public:
    StudentManager(const std::string& filePath = "data/students.json");
    virtual ~StudentManager() = default;

    void loadFromFile() override;
    void saveToFile() const override;

    std::vector<Student*> findByClass(const std::string& className) const;
    std::vector<Student*> findByFaculty(const std::string& faculty) const;
};

// =========================================================================
// 3. LỚP QUẢN LÝ PHÒNG Ở (ROOMMANAGER)
// =========================================================================

class RoomManager : public Manager<Room> {
public:
    RoomManager(const std::string& filePath = "data/rooms.json");
    virtual ~RoomManager() = default;

    void loadFromFile() override;
    void saveToFile() const override;

    std::vector<Room*> findAvailableRoom() const;
    void printAvailableRoom() const;
    std::vector<Room*> findByBuilding(const std::string& building) const;
    std::vector<Room*> findByType(const std::string& roomType) const;

    void getOccupancyStats(int& totalCapacity, int& totalOccupants, double& rate) const;
    void printOccupancyReport() const;
    void printRoomBill(const std::string& roomId, double electricUsage, double waterUsage) const;
};

// =========================================================================
// 4. LỚP QUẢN LÝ CÁN BỘ (STAFFMANAGER)
// =========================================================================

class StaffManager : public Manager<Staff> {
public:
    StaffManager(const std::string& filePath = "data/staffs.json");
    virtual ~StaffManager() = default;

    void loadFromFile() override;
    void saveToFile() const override;

    std::vector<Staff*> findByDepartment(const std::string& dept) const;
};

// =========================================================================
// 5. LỚP QUẢN LÝ HỢP ĐỒNG (RENTMANAGER)
// =========================================================================

class RentManager : public Manager<Rent> {
public:
    RentManager(const std::string& filePath = "data/rents.json");
    virtual ~RentManager() = default;

    void loadFromFile() override;
    void loadFromFile(StudentManager& sm, RoomManager& rm);
    void saveToFile() const override;

    std::vector<Rent*> findByStatus(RentStatus status) const;
    std::vector<Rent*> findByStudent(const std::string& studentId) const;
    std::vector<Rent*> findByRoom(const std::string& roomId) const;

    Rent* createRent(const std::string& rentId, Student* student, Room* room,
                     Period period, const Date& startDate, double fee);
    bool terminateRent(const std::string& rentId, const Date& actualEndDate);

    std::vector<Rent*> findExpiringRents(const Date& currentDate, int daysThreshold = 30) const;
    void printExpiringRents(const Date& currentDate, int daysThreshold = 30) const;
    int checkAndExpireRents(const Date& currentDate);
    double calculateTotalMonthlyRevenue() const;
};

// =========================================================================
// 6. LỚP QUẢN LÝ TÀI KHOẢN (ACCOUNTMANAGER)
// =========================================================================

class AccountManager : public Manager<Account> {
public:
    AccountManager(const std::string& filePath = "data/accounts.json");
    virtual ~AccountManager() = default;

    void loadFromFile() override;
    void saveToFile() const override;

    Account* authenticate(const std::string& username, const std::string& pass);
    Account* findByUsername(const std::string& username) const;
    Account* findByOwnerId(const std::string& ownerId) const;
    Account* createAccount(const std::string& accId, const std::string& username,
                           const std::string& password, Role role,
                           const std::string& ownerId);
};

#endif // MANAGERS_H
