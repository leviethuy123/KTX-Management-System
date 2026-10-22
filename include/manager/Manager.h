#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <nlohmann/json.hpp>
#include "common/IEntity.h"

/**
 * @brief Lớp khuôn mẫu generic Manager<T> (Repository Pattern).
 * Thể hiện tính Trừu tượng và Tái sử dụng code qua C++ Template.
 * Quản lý danh sách con trỏ T*, yêu cầu T phải kế thừa từ IEntity.
 */
template <typename T>
class Manager {
protected:
    std::vector<T*> list;        // Khớp XML: # list: vector<T*>
    std::string dataFilePath;    // Đường dẫn file JSON

public:
    Manager(const std::string& filePath = "") : dataFilePath(filePath) {}

    virtual ~Manager() {
        clear();
    }

    // Giải phóng toàn bộ bộ nhớ các phần tử trong danh sách
    void clear() {
        for (T* item : list) {
            delete item;
        }
        list.clear();
    }

    // Thêm đối tượng mới (khớp XML: + add(T* item): void/bool)
    virtual bool add(T* item) {
        if (item == nullptr) return false;
        if (findById(item->getId()) != nullptr) {
            std::cout << "[Canh bao] Ma ID '" << item->getId() << "' da ton tai!\n";
            return false;
        }
        list.push_back(item);
        return true;
    }

    // Xóa đối tượng theo ID (khớp XML: + remove(const string& id): bool)
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

    // Tìm kiếm theo ID (khớp XML: + findById(const string& id): T* {const})
    virtual T* findById(const std::string& id) const {
        for (T* item : list) {
            if (item != nullptr && item->getId() == id) {
                return item;
            }
        }
        return nullptr;
    }

    // In thông tin theo ID (khớp XML: + printById(const string& id): void {const})
    virtual void printById(const std::string& id) const {
        T* item = findById(id);
        if (item != nullptr) {
            item->print();
        } else {
            std::cout << "Khong tim thay ban ghi co ID: " << id << "\n";
        }
    }

    // In toàn bộ danh sách (khớp XML: + printAll(): void {const})
    virtual void printAll() const {
        if (list.empty()) {
            std::cout << "(Danh sach trong)\n";
            return;
        }
        for (const T* item : list) {
            if (item != nullptr) {
                item->print();
            }
        }
    }

    // Lấy toàn bộ danh sách (khớp XML: + getAll(): const vector<T*>& {const})
    const std::vector<T*>& getAll() const {
        return list;
    }

    // Đếm số lượng
    size_t getCount() const {
        return list.size();
    }

    std::string getDataFilePath() const { return dataFilePath; }
    void setDataFilePath(const std::string& path) { dataFilePath = path; }

    // Đọc / Ghi file JSON thuần ảo cho từng Manager con cài đặt
    virtual void loadFromFile() = 0;
    virtual void saveToFile() const = 0;
};

#endif // MANAGER_H
