#include "manager/AccountManager.h"
#include <fstream>
#include <iostream>

AccountManager::AccountManager(const std::string& filePath)
    : Manager<Account>(filePath) {}

void AccountManager::loadFromFile() {
    clear();
    std::ifstream file(dataFilePath);
    if (!file.is_open()) {
        std::cout << "[Thông báo] Không thể mở file: " << dataFilePath << ". Bắt đầu với danh sách trống.\n";
        return;
    }

    try {
        nlohmann::json jArr;
        file >> jArr;
        for (const auto& item : jArr) {
            Account acc = Account::fromJson(item);
            list.push_back(new Account(acc));
        }
        std::cout << "[Thành công] Đã nạp " << list.size() << " tài khoản từ " << dataFilePath << "\n";
    } catch (const std::exception& e) {
        std::cout << "[Lỗi] Parse JSON tài khoản thất bại: " << e.what() << "\n";
    }
}

void AccountManager::saveToFile() const {
    std::ofstream file(dataFilePath);
    if (!file.is_open()) {
        std::cout << "[Lỗi] Không thể mở file để ghi: " << dataFilePath << "\n";
        return;
    }

    nlohmann::json jArr = nlohmann::json::array();
    for (const Account* acc : list) {
        if (acc != nullptr) {
            jArr.push_back(acc->toJson());
        }
    }

    file << jArr.dump(2);
    std::cout << "[Thành công] Đã lưu " << list.size() << " tài khoản vào " << dataFilePath << "\n";
}

Account* AccountManager::authenticate(const std::string& username, const std::string& pass) {
    Account* acc = findByUsername(username);
    if (acc == nullptr) {
        std::cout << "[Xác thực thất bại] Không tồn tại tài khoản với tên đăng nhập: '" << username << "'\n";
        return nullptr;
    }

    if (!acc->getStatus()) {
        std::cout << "[Xác thực thất bại] Tài khoản '" << username << "' đang bị khóa!\n";
        return nullptr;
    }

    if (!acc->verifyPassword(pass)) {
        std::cout << "[Xác thực thất bại] Sai mật khẩu!\n";
        return nullptr;
    }

    std::cout << "[Xác thực thành công] Xin chào " << username
              << " (" << EnumHelper::roleToString(acc->getRole()) << ")!\n";
    return acc;
}

Account* AccountManager::findByUsername(const std::string& username) const {
    for (Account* acc : list) {
        if (acc != nullptr && acc->getUsername() == username) {
            return acc;
        }
    }
    return nullptr;
}

Account* AccountManager::findByOwnerId(const std::string& ownerId) const {
    for (Account* acc : list) {
        if (acc != nullptr && acc->getOwnerId() == ownerId) {
            return acc;
        }
    }
    return nullptr;
}

Account* AccountManager::createAccount(const std::string& accId, const std::string& username,
                                      const std::string& password, Role role,
                                      const std::string& ownerId) {
    if (findById(accId) != nullptr) {
        std::cout << "[Lỗi] Mã tài khoản '" << accId << "' đã tồn tại!\n";
        return nullptr;
    }

    if (findByUsername(username) != nullptr) {
        std::cout << "[Lỗi] Tên đăng nhập '" << username << "' đã được sử dụng!\n";
        return nullptr;
    }

    Account* newAcc = new Account(accId, username, password, role, ownerId, true);
    list.push_back(newAcc);
    std::cout << "[Thành công] Đã tạo tài khoản '" << username << "' ("
              << EnumHelper::roleToString(role) << ")\n";
    return newAcc;
}
