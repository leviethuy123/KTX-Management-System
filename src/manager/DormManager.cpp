#include "manager/DormManager.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <cctype>
#include <cstdlib>

DormManager::DormManager()
    : currentAccount(nullptr) {
    roomManage = new RoomManager("data/rooms.json");
    studentManage = new StudentManager("data/students.json");
    staffManager = new StaffManager("data/staffs.json");
    rentManager = new RentManager("data/rents.json");
    accountManager = new AccountManager("data/accounts.json");
    loadAll();
}

DormManager::~DormManager() {
    saveAll();
    delete roomManage;
    delete studentManage;
    delete staffManager;
    delete rentManager;
    delete accountManager;
}

void DormManager::loadAll() {
    studentManage->loadFromFile();
    roomManage->loadFromFile();
    staffManager->loadFromFile();
    accountManager->loadFromFile();
    rentManager->loadFromFile(*studentManage, *roomManage);
}

void DormManager::saveAll() const {
    studentManage->saveToFile();
    roomManage->saveToFile();
    staffManager->saveToFile();
    rentManager->saveToFile();
    accountManager->saveToFile();
}

bool DormManager::login(const std::string& user, const std::string& pass) {
    Account* acc = accountManager->authenticate(user, pass);
    if (acc != nullptr) {
        currentAccount = acc;
        return true;
    }
    return false;
}

void DormManager::logout() {
    if (currentAccount != nullptr) {
        std::cout << "\n[Thành công] Đã đăng xuất khỏi tài khoản '" << currentAccount->getUsername() << "'.\n";
        currentAccount = nullptr;
    }
}

Role DormManager::getCurrentUserRole() const {
    if (currentAccount != nullptr) {
        return currentAccount->getRole();
    }
    return Role::STUDENT;
}

void DormManager::clearScreen() {
#if defined(_WIN32) || defined(_WIN64)
    std::system("cls");
#else
    std::cout << "\033[2J\033[H" << std::flush;
#endif
}

void DormManager::printHeaderInfo() {
    std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║  Phiên bản : 2.0.0                                                       ║\n";
    std::cout << "║  Ngày HT   : 05 / 11 / 2026                                              ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
}

void DormManager::pauseScreen() {
    std::cout << "\nNhấn Enter để tiếp tục...";
    std::string dummy;
    std::getline(std::cin, dummy);
}

std::string DormManager::inputString(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    if (!std::getline(std::cin, s)) {
        return "";
    }
    if (s.size() >= 3 &&
        static_cast<unsigned char>(s[0]) == 0xEF &&
        static_cast<unsigned char>(s[1]) == 0xBB &&
        static_cast<unsigned char>(s[2]) == 0xBF) {
        s.erase(0, 3);
    }
    while (!s.empty() && (s.back() == '\r' || s.back() == ' ' || s.back() == '\t')) {
        s.pop_back();
    }
    while (!s.empty() && (s.front() == ' ' || s.front() == '\t')) {
        s.erase(0, 1);
    }
    return s;
}

int DormManager::inputInt(const std::string& prompt) {
    while (true) {
        if (std::cin.eof()) return 0;
        std::string s = inputString(prompt);
        if (s.empty()) {
            if (std::cin.eof()) return 0;
            continue;
        }
        try {
            return std::stoi(s);
        } catch (const std::exception&) {
            if (std::cin.eof()) return 0;
            std::cout << "[Lỗi] Vui lòng nhập số nguyên hợp lệ!\n";
        }
    }
}

double DormManager::inputDouble(const std::string& prompt) {
    while (true) {
        if (std::cin.eof()) return 0.0;
        std::string s = inputString(prompt);
        if (s.empty()) {
            if (std::cin.eof()) return 0.0;
            continue;
        }
        try {
            return std::stod(s);
        } catch (const std::exception&) {
            if (std::cin.eof()) return 0.0;
            std::cout << "[Lỗi] Vui lòng nhập số thực hợp lệ!\n";
        }
    }
}

std::string DormManager::inputNonEmpty(const std::string& prompt) {
    while (true) {
        if (std::cin.eof()) return "";
        std::string s = inputString(prompt);
        if (!s.empty()) return s;
        std::cout << "[Lỗi] Trường này không được để trống! Vui lòng nhập lại.\n";
    }
}

Date DormManager::inputValidDate(const std::string& prompt, const Date& defaultDate) {
    while (true) {
        if (std::cin.eof()) return defaultDate;
        std::string s = inputString(prompt);
        if (s.empty()) {
            return defaultDate;
        }
        try {
            Date parsedDate;
            if (!Date::tryParse(s, parsedDate)) {
                throw std::invalid_argument("Ngày '" + s + "' không hợp lệ hoặc sai định dạng! Vui lòng nhập đúng dd/mm/yyyy (ví dụ: 05/11/2026).");
            }
            return parsedDate;
        } catch (const std::exception& e) {
            std::cout << "[Lỗi] " << e.what() << "\n";
        }
    }
}

std::string DormManager::inputValidPhone(const std::string& prompt) {
    while (true) {
        if (std::cin.eof()) return "0900000000";
        std::string s = inputString(prompt);
        try {
            if (s.length() != 10 || s[0] != '0') {
                throw std::invalid_argument("Số điện thoại phải gồm đúng 10 chữ số và bắt đầu bằng số 0 (ví dụ: 0912345678)!");
            }
            for (char c : s) {
                if (!std::isdigit(static_cast<unsigned char>(c))) {
                    throw std::invalid_argument("Số điện thoại chỉ được chứa các chữ số!");
                }
            }
            return s;
        } catch (const std::exception& e) {
            std::cout << "[Lỗi] " << e.what() << "\n";
        }
    }
}

std::string DormManager::inputValidEmail(const std::string& prompt) {
    while (true) {
        if (std::cin.eof()) return "student@dut.udn.vn";
        std::string s = inputString(prompt);
        try {
            size_t atPos = s.find('@');
            size_t dotPos = s.rfind('.');
            if (atPos == std::string::npos || dotPos == std::string::npos || atPos == 0 || dotPos <= atPos + 1 || dotPos >= s.length() - 1) {
                throw std::invalid_argument("Email không hợp lệ (phải chứa '@' và tên miền hợp lệ, ví dụ: sv@dut.udn.vn)!");
            }
            return s;
        } catch (const std::exception& e) {
            std::cout << "[Lỗi] " << e.what() << "\n";
        }
    }
}

std::string DormManager::inputValidClassName(const std::string& prompt) {
    while (true) {
        if (std::cin.eof()) return "22T_DT1";
        std::string s = inputString(prompt);
        try {
            std::string errorReason;
            if (!ClassValidator::isValidClassName(s, errorReason)) {
                throw std::invalid_argument("Tên lớp không đúng quy định: " + errorReason);
            }
            if (!ClassValidator::isClassRegistered(s)) {
                std::cout << "[Thông báo] Lớp '" << s << "' chưa có trong danh mục lớp chuẩn. Tự động đăng ký mới vào danh mục...\n";
                ClassValidator::registerClass(s);
            }
            return s;
        } catch (const std::exception& e) {
            std::cout << "[Lỗi] " << e.what() << "\n";
        }
    }
}

std::string DormManager::inputFacultyChoice() {
    Faculty::printMenu();
    while (true) {
        int choice = inputInt(">> Chọn Khoa (1 - 10): ");
        if (Faculty::isValidIndex(choice)) {
            std::string fac = Faculty::getByIndex(choice);
            std::cout << "-> Đã chọn Khoa: " << fac << "\n";
            return fac;
        }
        std::cout << "[Lỗi] Lựa chọn không hợp lệ! Vui lòng nhập số từ 1 đến 10.\n";
    }
}

Gender DormManager::inputGender(const std::string& prompt) {
    while (true) {
        if (std::cin.eof()) return Gender::MALE;
        std::string s = inputString(prompt);
        try {
            return EnumHelper::stringToGender(s);
        } catch (const std::invalid_argument& e) {
            std::cout << "[Lỗi] " << e.what() << "\n";
        } catch (...) {
            std::cout << "[Lỗi] Dữ liệu giới tính không hợp lệ! Vui lòng nhập lại.\n";
        }
    }
}

Role DormManager::inputRole(const std::string& prompt) {
    while (true) {
        if (std::cin.eof()) return Role::STUDENT;
        std::string s = inputString(prompt);
        try {
            return EnumHelper::stringToRole(s);
        } catch (const std::invalid_argument& e) {
            std::cout << "[Lỗi] " << e.what() << "\n";
        } catch (...) {
            std::cout << "[Lỗi] Dữ liệu vai trò không hợp lệ! Vui lòng nhập lại.\n";
        }
    }
}

Period DormManager::inputPeriod(const std::string& prompt) {
    while (true) {
        if (std::cin.eof()) return Period::ONE_MONTH;
        int m = inputInt(prompt);
        try {
            return EnumHelper::intToPeriod(m);
        } catch (const std::invalid_argument& e) {
            std::cout << "[Lỗi] " << e.what() << "\n";
        } catch (...) {
            std::cout << "[Lỗi] Kỳ hạn không hợp lệ! Vui lòng nhập lại.\n";
        }
    }
}

RoomStatus DormManager::inputRoomStatus(const std::string& prompt) {
    while (true) {
        if (std::cin.eof()) return RoomStatus::EMPTY;
        std::string s = inputString(prompt);
        try {
            return EnumHelper::stringToRoomStatus(s);
        } catch (const std::invalid_argument& e) {
            std::cout << "[Lỗi] " << e.what() << "\n";
        } catch (...) {
            std::cout << "[Lỗi] Trạng thái phòng không hợp lệ! Vui lòng nhập lại.\n";
        }
    }
}

void DormManager::runMainMenu() {
    bool running = true;
    while (running) {
        if (currentAccount == nullptr) {
            clearScreen();
        printHeaderInfo();
        std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
            std::cout << "║               HỆ THỐNG QUẢN LÝ KÝ TÚC XÁ SINH VIÊN (PBL2)                ║\n";
            std::cout << "╠══════════════════════════════════════════════════════════════════════════╣\n";
            std::cout << "║  [1] Đăng nhập hệ thống                                                  ║\n";
            std::cout << "║  [0] Thoát chương trình & Lưu dữ liệu                                    ║\n";
            std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
            int choice = inputInt(">> Lựa chọn của bạn: ");

            if (choice == 1) {
                showLoginScreen();
            } else if (choice == 0) {
                std::cout << "\nĐang lưu toàn bộ dữ liệu hệ thống...\n";
                saveAll();
                std::cout << "Cảm ơn bạn đã sử dụng chương trình! Tạm biệt.\n";
                running = false;
            } else {
                std::cout << "[Cảnh báo] Lựa chọn không hợp lệ!\n";
                pauseScreen();
            }
        } else {
            switch (currentAccount->getRole()) {
                case Role::ADMIN:
                    runAdminMenu();
                    break;
                case Role::STAFF:
                    runStaffMenu();
                    break;
                case Role::STUDENT:
                    runStudentMenu();
                    break;
            }
        }
    }
}

void DormManager::showLoginScreen() {
    clearScreen();
        printHeaderInfo();
    std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                          ĐĂNG NHẬP HỆ THỐNG                              ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
    std::string user = inputString("Tên đăng nhập: ");
    std::string pass = inputString("Mật khẩu: ");

    if (login(user, pass)) {
        std::cout << "[Thành công] Đăng nhập thành công với vai trò: "
                  << EnumHelper::roleToString(currentAccount->getRole()) << "\n";
    } else {
        std::cout << "[Thất bại] Sai tên đăng nhập hoặc mật khẩu!\n";
    }
    pauseScreen();
}

void DormManager::runStudentMenu() {
    std::string ownerId = currentAccount->getOwnerId();
    Student* s = studentManage->findById(ownerId);

    while (true) {
        clearScreen();
        printHeaderInfo();
        std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                CỔNG THÔNG TIN SINH VIÊN NỘI TRÚ KÝ TÚC XÁ                ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
        std::cout << "  >> Xin chào: " << (s ? s->getFullName() : currentAccount->getUsername())
                  << " | Mã SV: " << ownerId << "\n";
        std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║  [1] Xem thông tin hồ sơ cá nhân                                         ║\n";
        std::cout << "║  [2] Xem thông tin phòng ở hiện tại & bạn cùng phòng                     ║\n";
        std::cout << "║  [3] Xem hợp đồng thuê phòng của tôi                                     ║\n";
        std::cout << "║  [4] Xem phiếu báo tiền phòng & chi phí sinh hoạt tháng này              ║\n";
        std::cout << "║  [5] Đổi mật khẩu tài khoản                                              ║\n";
        std::cout << "║  [0] Đăng xuất tài khoản                                                 ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
        int choice = inputInt(">> Nhập lựa chọn: ");

        if (choice == 0) {
            logout();
            break;
        }

        switch (choice) {
            case 1:
                std::cout << "\n--- HỒ SƠ CÁ NHÂN CỦA BẠN ---\n";
                if (s != nullptr) s->print();
                else std::cout << "Chưa có thông tin hồ sơ cho tài khoản này.\n";
                pauseScreen();
                break;
            case 2: {
                std::cout << "\n--- THÔNG TIN PHÒNG Ở HIỆN TẠI ---\n";
                if (s != nullptr && s->getRent() != nullptr && s->getRent()->isActive()) {
                    Room* rm = s->getRent()->getRoom();
                    if (rm != nullptr) {
                        rm->print();
                        rm->printBrokenDevices();
                        std::cout << "\nDanh sách bạn cùng phòng:\n";
                        for (const Rent* r : rm->getActiveRent()) {
                            if (r != nullptr && r->getTenant() != nullptr) {
                                std::cout << " - " << r->getTenant()->getFullName()
                                          << " (" << r->getTenant()->getId() << ") | Lớp: "
                                          << r->getTenant()->getClass() << "\n";
                            }
                        }
                    }
                } else {
                    std::cout << "Bạn hiện tại chưa được xếp phòng hoặc hợp đồng đã kết thúc.\n";
                }
                pauseScreen();
                break;
            }
            case 3:
                std::cout << "\n--- HỢP ĐỒNG THUÊ PHÒNG CỦA BẠN ---\n";
                if (s != nullptr && s->getRent() != nullptr) {
                    s->getRent()->print();
                } else {
                    std::cout << "Bạn hiện tại không có hợp đồng thuê nào đang hiệu lực.\n";
                }
                pauseScreen();
                break;
            case 4: {
                std::cout << "\n--- PHIẾU BÁO TIỀN PHÒNG & SINH HOẠT THÁNG NÀY ---\n";
                if (s != nullptr && s->getRent() != nullptr && s->getRent()->isActive()) {
                    Room* rm = s->getRent()->getRoom();
                    if (rm != nullptr) {
                        double elec = inputDouble("Nhập chỉ số điện tiêu thụ của phòng tháng này (kWh): ");
                        double water = inputDouble("Nhập chỉ số nước tiêu thụ của phòng tháng này (m³): ");
                        roomManage->printRoomBill(rm->getId(), elec, water);
                    }
                } else {
                    std::cout << "Bạn hiện không ở trong phòng nào để xuất hóa đơn.\n";
                }
                pauseScreen();
                break;
            }
            case 5:
                handleChangePassword();
                break;
            default:
                std::cout << "[Cảnh báo] Lựa chọn không hợp lệ!\n";
                pauseScreen();
                break;
        }
    }
}

void DormManager::runStaffMenu() {
    std::string ownerId = currentAccount->getOwnerId();
    Staff* st = staffManager->findById(ownerId);

    while (true) {
        clearScreen();
        printHeaderInfo();
        std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                   BÀN LÀM VIỆC CÁN BỘ QUẢN LÝ KTX                        ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
        std::cout << "  >> Cán bộ: " << (st ? st->getFullName() : currentAccount->getUsername())
                  << " | Bộ phận: " << (st ? st->getDepartment() : "Ban Quản lý") << "\n";
        std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║  [1] Quản lý Danh sách Phòng ở                                           ║\n";
        std::cout << "║  [2] Quản lý Hồ sơ Sinh viên                                             ║\n";
        std::cout << "║  [3] Phân bổ & Xếp chỗ Phòng KTX (Xếp phòng, Chuyển phòng, Trả phòng)    ║\n";
        std::cout << "║  [4] Quản lý Hợp đồng Thuê KTX (Gia hạn, Cảnh báo hết hạn)               ║\n";
        std::cout << "║  [5] Quản lý Doanh thu & Hóa đơn Điện Nước                               ║\n";
        std::cout << "║  [6] Báo cáo Thống kê Tổng thể                                           ║\n";
        std::cout << "║  [7] Đổi mật khẩu cá nhân                                                ║\n";
        std::cout << "║  [0] Đăng xuất                                                           ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
        int choice = inputInt(">> Nhập lựa chọn: ");

        if (choice == 0) {
            logout();
            break;
        }

        switch (choice) {
            case 1: handleRoomManagement(); break;
            case 2: handleStudentManagement(); break;
            case 3: handleAllocationManagement(); break;
            case 4: handleContractManagement(); break;
            case 5: handleRevenueManagement(); break;
            case 6: handleReportManagement(); break;
            case 7: handleChangePassword(); break;
            default:
                std::cout << "[Cảnh báo] Lựa chọn không hợp lệ!\n";
                pauseScreen();
                break;
        }
    }
}

void DormManager::runAdminMenu() {
    while (true) {
        clearScreen();
        printHeaderInfo();
        std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║               TRUNG TÂM QUẢN TRỊ VIÊN HỆ THỐNG KTX (ADMIN)               ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
        std::cout << "  >> Quản trị viên: " << currentAccount->getUsername() << "\n";
        std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║  [1] Quản lý Danh sách Phòng ở                                           ║\n";
        std::cout << "║  [2] Quản lý Hồ sơ Sinh viên                                             ║\n";
        std::cout << "║  [3] Phân bổ & Xếp chỗ Phòng KTX (Xếp phòng, Chuyển phòng, Trả phòng)    ║\n";
        std::cout << "║  [4] Quản lý Hợp đồng Thuê KTX (Gia hạn, Cảnh báo hết hạn)               ║\n";
        std::cout << "║  [5] Quản lý Doanh thu & Hóa đơn Điện Nước                               ║\n";
        std::cout << "║  [6] Quản lý Hồ sơ Cán bộ KTX                                            ║\n";
        std::cout << "║  [7] Quản lý Hệ thống Tài khoản & Phân quyền                             ║\n";
        std::cout << "║  [8] Báo cáo Thống kê Tổng thể                                           ║\n";
        std::cout << "║  [9] Đổi mật khẩu cá nhân                                                ║\n";
        std::cout << "║  [0] Đăng xuất                                                           ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
        int choice = inputInt(">> Nhập lựa chọn: ");

        if (choice == 0) {
            logout();
            break;
        }

        switch (choice) {
            case 1: handleRoomManagement(); break;
            case 2: handleStudentManagement(); break;
            case 3: handleAllocationManagement(); break;
            case 4: handleContractManagement(); break;
            case 5: handleRevenueManagement(); break;
            case 6: {
                std::cout << "\n=== DANH SÁCH CÁN BỘ QUẢN LÝ KTX ===\n";
                TablePrinter::printStaffs(staffManager->getAll());
                pauseScreen();
                break;
            }
            case 7: handleAccountManagement(); break;
            case 8: handleReportManagement(); break;
            case 9: handleChangePassword(); break;
            default:
                std::cout << "[Cảnh báo] Lựa chọn không hợp lệ!\n";
                pauseScreen();
                break;
        }
    }
}

void DormManager::handleRoomManagement() {
    while (true) {
        clearScreen();
        printHeaderInfo();
        std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                          QUẢN LÝ PHÒNG Ở KTX                             ║\n";
        std::cout << "╠══════════════════════════════════════════════════════════════════════════╣\n";
        std::cout << "║  [1] Xem toàn bộ danh sách phòng (Dạng Bảng)                             ║\n";
        std::cout << "║  [2] Lọc các phòng còn chỗ trống (Dạng Bảng)                             ║\n";
        std::cout << "║  [3] Lọc phòng theo Tòa nhà (Dạng Bảng)                                  ║\n";
        std::cout << "║  [4] Lọc phòng theo Loại phòng (STANDARD / VIP)                          ║\n";
        std::cout << "║  [5] Xem chi tiết 1 phòng theo mã & thiết bị hỏng                        ║\n";
        std::cout << "║  [6] Báo hỏng thiết bị trong phòng                                       ║\n";
        std::cout << "║  [7] Xác nhận đã sửa chữa thiết bị                                       ║\n";
        std::cout << "║  [0] Quay lại Menu chính                                                 ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
        int c = inputInt(">> Lựa chọn: ");
        if (c == 0) break;

        if (c == 1) {
            std::cout << "\n=== TOÀN BỘ DANH SÁCH PHÒNG Ở KÝ TÚC XÁ ===\n";
            TablePrinter::printRooms(roomManage->getAll());
        } else if (c == 2) {
            auto list = roomManage->findAvailableRoom();
            std::cout << "\n=== DANH SÁCH PHÒNG CÒN CHỖ TRỐNG (" << list.size() << " phòng) ===\n";
            TablePrinter::printRooms(list);
        } else if (c == 3) {
            std::string bld = inputNonEmpty("Nhập tên tòa nhà (ví dụ: Khu A, Khu VIP): ");
            auto list = roomManage->findByBuilding(bld);
            std::cout << "\n=== KẾT QUẢ LỌC THEO TÒA '" << bld << "' (" << list.size() << " phòng) ===\n";
            TablePrinter::printRooms(list);
        } else if (c == 4) {
            std::string type = inputNonEmpty("Nhập loại phòng (STANDARD hoặc VIP): ");
            auto list = roomManage->findByType(type);
            std::cout << "\n=== KẾT QUẢ LỌC THEO LOẠI '" << type << "' (" << list.size() << " phòng) ===\n";
            TablePrinter::printRooms(list);
        } else if (c == 5) {
            std::string id = inputNonEmpty("Nhập mã phòng: ");
            Room* r = roomManage->findById(id);
            if (r != nullptr) {
                std::cout << "\n--- THÔNG TIN CHI TIẾT PHÒNG ---\n";
                r->print();
                TablePrinter::printBrokenDevices(r->getId(), r->getBrokenDevices());
            } else {
                std::cout << "[Lỗi] Không tìm thấy phòng có mã: " << id << "\n";
            }
        } else if (c == 6) {
            std::cout << "\n--- BÁO HỎNG THIẾT BỊ PHÒNG ---\n";
            std::string id = inputNonEmpty("Nhập mã phòng: ");
            Room* r = roomManage->findById(id);
            if (r == nullptr) {
                std::cout << "[Lỗi] Không tìm thấy phòng: " << id << "\n";
            } else {
                std::string dId = inputNonEmpty("Mã thiết bị (ví dụ: TB01): ");
                std::string dName = inputNonEmpty("Tên thiết bị (ví dụ: Quạt trần, Bàn học, Bóng đèn,...): ");
                std::string desc = inputNonEmpty("Mô tả tình trạng hư hỏng: ");
                Date d = inputValidDate("Ngày báo hỏng (dd/mm/yyyy, Enter để dùng 05/11/2026): ", Date(5, 11, 2026));
                r->addBrokenDevice(BrokenDevice(dId, dName, desc, d, false));
                roomManage->saveToFile();
                std::cout << "[Thành công] Đã ghi nhận thiết bị hỏng '" << dName << "' (" << dId << ") cho phòng " << id << "!\n";
            }
        } else if (c == 7) {
            std::cout << "\n--- XỬ LÝ / SỬA CHỮA THIẾT BỊ HỎNG ---\n";
            std::string id = inputNonEmpty("Nhập mã phòng: ");
            Room* r = roomManage->findById(id);
            if (r == nullptr) {
                std::cout << "[Lỗi] Không tìm thấy phòng: " << id << "\n";
            } else {
                TablePrinter::printBrokenDevices(r->getId(), r->getBrokenDevices());
                if (r->hasBrokenDevices()) {
                    std::string dId = inputNonEmpty("Nhập mã hoặc tên thiết bị đã sửa chữa xong: ");
                    if (r->fixBrokenDevice(dId)) {
                        roomManage->saveToFile();
                        std::cout << "[Thành công] Đã cập nhật thiết bị '" << dId << "' sang trạng thái ĐÃ SỬA XONG!\n";
                    } else {
                        std::cout << "[Lỗi] Không tìm thấy thiết bị: " << dId << "\n";
                    }
                }
            }
        }
        pauseScreen();
    }
}

void DormManager::handleStudentManagement() {
    while (true) {
        clearScreen();
        printHeaderInfo();
        std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                        QUẢN LÝ HỒ SƠ SINH VIÊN                           ║\n";
        std::cout << "╠══════════════════════════════════════════════════════════════════════════╣\n";
        std::cout << "║  [1] Xem danh sách tất cả sinh viên (Dạng Bảng)                          ║\n";
        std::cout << "║  [2] Tìm kiếm sinh viên theo Mã sinh viên                                ║\n";
        std::cout << "║  [3] Lọc sinh viên theo Lớp (Dạng Bảng)                                  ║\n";
        std::cout << "║  [4] Lọc sinh viên theo Khoa (Dạng Bảng)                                 ║\n";
        std::cout << "║  [5] Thêm sinh viên mới vào hệ thống                                     ║\n";
        std::cout << "║  [0] Quay lại Menu chính                                                 ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
        int c = inputInt(">> Lựa chọn: ");
        if (c == 0) break;

        if (c == 1) {
            std::cout << "\n=== DANH SÁCH TOÀN BỘ SINH VIÊN KÝ TÚC XÁ ===\n";
            TablePrinter::printStudents(studentManage->getAll());
        } else if (c == 2) {
            std::string id = inputNonEmpty("Nhập mã sinh viên: ");
            Student* s = studentManage->findById(id);
            if (s != nullptr) {
                TablePrinter::printStudents({s});
            } else {
                std::cout << "[Lỗi] Không tìm thấy sinh viên có mã: " << id << "\n";
            }
        } else if (c == 3) {
            std::string lop = inputNonEmpty("Nhập tên lớp: ");
            auto list = studentManage->findByClass(lop);
            std::cout << "\n=== DANH SÁCH SINH VIÊN LỚP '" << lop << "' (" << list.size() << " sinh viên) ===\n";
            TablePrinter::printStudents(list);
        } else if (c == 4) {
            std::cout << "\n--- CHỌN KHOA CẦN LỌC ---\n";
            std::string khoa = inputFacultyChoice();
            auto list = studentManage->findByFaculty(khoa);
            std::cout << "\n=== DANH SÁCH SINH VIÊN KHOA '" << khoa << "' (" << list.size() << " sinh viên) ===\n";
            TablePrinter::printStudents(list);
        } else if (c == 5) {
            std::cout << "\n--- NHẬP THÔNG TIN SINH VIÊN MỚI ---\n";
            std::string id = inputNonEmpty("Mã SV (ví dụ: 102220005): ");
            while (studentManage->findById(id) != nullptr) {
                std::cout << "[Lỗi] Mã sinh viên '" << id << "' đã tồn tại trong hệ thống! Vui lòng nhập mã khác.\n";
                id = inputNonEmpty("Mã SV: ");
            }

            std::string name = inputNonEmpty("Họ và tên: ");

            Gender gEnum = inputGender("Giới tính (1: Nam, 2: Nữ): ");
            bool g = (gEnum == Gender::MALE);

            int birthYear = 0;
            while (true) {
                try {
                    birthYear = inputInt("Năm sinh (1980 - 2012, ví dụ 2004): ");
                    if (birthYear < 1980 || birthYear > 2012) {
                        throw std::out_of_range("Năm sinh ngoài khoảng hợp lệ (1980-2012)! Vui lòng nhập lại.");
                    }
                    break;
                } catch (const std::exception& e) {
                    std::cout << "[Lỗi] " << e.what() << "\n";
                }
            }
            std::string birth = std::to_string(birthYear);

            std::string lop = inputValidClassName("Lớp sinh hoạt (ví dụ: 22T_DT1, 23KT): ");
            std::string khoaHoc = ClassValidator::extractCohort(lop);
            std::cout << "-> Tự động xác định Khóa: " << khoaHoc << "\n";

            std::cout << "\n--- CHỌN KHOA QUẢN LÝ TRỰC THUỘC ---\n";
            std::string khoa = inputFacultyChoice();

            std::string phone = inputValidPhone("Số điện thoại (10 số, bắt đầu bằng 0): ");
            std::string email = inputValidEmail("Email liên lạc: ");
            std::string address = inputNonEmpty("Quê quán / Địa chỉ thường trú: ");

            Student* newS = new Student(id, name, g, birth, phone, email, address, lop, khoa, khoaHoc);
            if (studentManage->add(newS)) {
                studentManage->saveToFile();
                std::cout << "[Thành công] Đã thêm sinh viên " << name << " (" << id << ") vào hệ thống!\n";
                accountManager->createAccount("ACC_" + id, id, "123456", Role::STUDENT, id);
                accountManager->saveToFile();
                std::cout << "[Thành công] Đã tự động tạo tài khoản đăng nhập (user: " << id << ", pass: 123456).\n";
            }
        }
        pauseScreen();
    }
}

void DormManager::handleAllocationManagement() {
    while (true) {
        clearScreen();
        printHeaderInfo();
        std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                    PHÂN BỔ & XẾP CHỖ PHÒNG KTX                           ║\n";
        std::cout << "╠══════════════════════════════════════════════════════════════════════════╣\n";
        std::cout << "║  [1] Xem sơ đồ phân bổ toàn bộ phòng KTX (Sức chứa & Sinh viên đang ở)  ║\n";
        std::cout << "║  [2] Xem danh sách phòng còn chỗ trống (Dạng Bảng)                       ║\n";
        std::cout << "║  [3] Đăng ký xếp chỗ phòng mới cho sinh viên                             ║\n";
        std::cout << "║  [4] Chuyển phòng cho sinh viên đang ở                                   ║\n";
        std::cout << "║  [5] Trả phòng & Giải phóng chỗ (Thanh lý hợp đồng)                      ║\n";
        std::cout << "║  [0] Quay lại Menu chính                                                 ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
        int c = inputInt(">> Lựa chọn: ");
        if (c == 0) break;

        if (c == 1) {
            std::cout << "\n=== SƠ ĐỒ PHÂN BỔ TOÀN BỘ PHÒNG KÝ TÚC XÁ ===\n";
            TablePrinter::printRoomAllocationMap(roomManage->getAll());
        } else if (c == 2) {
            auto list = roomManage->findAvailableRoom();
            std::cout << "\n=== DANH SÁCH PHÒNG CÒN CHỖ TRỐNG (" << list.size() << " phòng) ===\n";
            TablePrinter::printRooms(list);
        } else if (c == 3) {
            std::cout << "\n--- ĐĂNG KÝ XẾP CHỖ PHÒNG MỚI CHO SINH VIÊN ---\n";
            std::string rentId = inputNonEmpty("Mã hợp đồng mới (ví dụ: HD004): ");
            while (rentManager->findById(rentId) != nullptr) {
                std::cout << "[Lỗi] Mã hợp đồng '" << rentId << "' đã tồn tại! Vui lòng nhập mã khác.\n";
                rentId = inputNonEmpty("Mã hợp đồng mới: ");
            }

            std::string sId = inputNonEmpty("Mã sinh viên: ");
            Student* s = studentManage->findById(sId);
            if (s == nullptr) {
                std::cout << "[Lỗi] Không tìm thấy sinh viên có mã: " << sId << "\n";
                pauseScreen();
                continue;
            }

            if (s->getRent() != nullptr && s->getRent()->isActive()) {
                std::cout << "[Lỗi] Sinh viên " << sId << " (" << s->getFullName()
                          << ") hiện đang ở phòng " << (s->getRent()->getRoom() ? s->getRent()->getRoom()->getId() : "N/A")
                          << " với hợp đồng " << s->getRent()->getId() << "!\n";
                pauseScreen();
                continue;
            }

            std::string rId = inputNonEmpty("Mã phòng muốn xếp (ví dụ: P101, P201): ");
            Room* r = roomManage->findById(rId);
            if (r == nullptr) {
                std::cout << "[Lỗi] Không tìm thấy phòng có mã: " << rId << "\n";
                pauseScreen();
                continue;
            }

            if (r->isFull()) {
                std::cout << "[Lỗi] Phòng " << rId << " đã đủ " << r->getMaxStudents() << " người, không thể xếp thêm!\n";
                pauseScreen();
                continue;
            }

            if (r->getStatus() == RoomStatus::MAINTENANCE) {
                std::cout << "[Lỗi] Phòng " << rId << " đang trong thời gian bảo trì!\n";
                pauseScreen();
                continue;
            }

            if (!r->isEmpty()) {
                bool conflict = false;
                for (const Rent* rent : r->getActiveRent()) {
                    if (rent != nullptr && rent->getTenant() != nullptr) {
                        if (rent->getTenant()->getGender() != s->getGender()) {
                            std::cout << "[Lỗi] Phòng " << rId << " là phòng "
                                      << EnumHelper::genderToString(rent->getTenant()->getGender())
                                      << ", không thể xếp sinh viên " << EnumHelper::genderToString(s->getGender()) << " vào!\n";
                            pauseScreen();
                            conflict = true;
                            break;
                        }
                    }
                }
                if (conflict) continue;
            }

            Period period = inputPeriod("Thời hạn thuê (1, 3, 6, 12 tháng): ");
            Date startDate = inputValidDate("Ngày bắt đầu hợp đồng (dd/mm/yyyy, Enter để dùng 05/11/2026): ", Date(5, 11, 2026));

            Rent* newRent = rentManager->createRent(rentId, s, r, period, startDate, r->getBasePrice());
            if (newRent != nullptr) {
                rentManager->saveToFile();
                roomManage->saveToFile();
                studentManage->saveToFile();
            }
        } else if (c == 4) {
            std::cout << "\n--- CHUYỂN PHÒNG CHO SINH VIÊN ---\n";
            std::string sId = inputNonEmpty("Nhập mã sinh viên cần chuyển phòng: ");
            std::string newRoomId = inputNonEmpty("Nhập mã phòng mới muốn chuyển tới: ");
            if (rentManager->transferRoom(sId, newRoomId, *roomManage)) {
                rentManager->saveToFile();
                roomManage->saveToFile();
            }
        } else if (c == 5) {
            std::cout << "\n--- TRẢ PHÒNG & THANH LÝ HỢP ĐỒNG SỚM ---\n";
            std::string rentId = inputNonEmpty("Nhập mã hợp đồng cần thanh lý: ");
            Date actualEnd = inputValidDate("Ngày trả phòng (dd/mm/yyyy, Enter để dùng 05/11/2026): ", Date(5, 11, 2026));
            if (rentManager->terminateRent(rentId, actualEnd)) {
                rentManager->saveToFile();
                roomManage->saveToFile();
                studentManage->saveToFile();
            }
        }
        pauseScreen();
    }
}

void DormManager::handleContractManagement() {
    while (true) {
        clearScreen();
        printHeaderInfo();
        std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                       QUẢN LÝ HỢP ĐỒNG THUÊ KTX                          ║\n";
        std::cout << "╠══════════════════════════════════════════════════════════════════════════╣\n";
        std::cout << "║  [1] Xem toàn bộ danh sách hợp đồng (Dạng Bảng)                          ║\n";
        std::cout << "║  [2] Tìm kiếm hợp đồng theo mã hợp đồng                                  ║\n";
        std::cout << "║  [3] Tìm kiếm hợp đồng theo mã sinh viên                                 ║\n";
        std::cout << "║  [4] Tìm kiếm hợp đồng theo mã phòng                                     ║\n";
        std::cout << "║  [5] Gia hạn hợp đồng thuê phòng                                         ║\n";
        std::cout << "║  [6] Cảnh báo các hợp đồng sắp hết hạn (trong 30 ngày)                   ║\n";
        std::cout << "║  [7] Tự động quét và giải phóng các hợp đồng đã quá hạn                  ║\n";
        std::cout << "║  [0] Quay lại Menu chính                                                 ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
        int c = inputInt(">> Lựa chọn: ");
        if (c == 0) break;

        if (c == 1) {
            std::cout << "\n=== TOÀN BỘ DANH SÁCH HỢP ĐỒNG KÝ TÚC XÁ ===\n";
            TablePrinter::printRents(rentManager->getAll());
        } else if (c == 2) {
            std::string rentId = inputNonEmpty("Nhập mã hợp đồng: ");
            Rent* r = rentManager->findById(rentId);
            if (r != nullptr) {
                TablePrinter::printRents({r});
            } else {
                std::cout << "[Lỗi] Không tìm thấy hợp đồng có mã: " << rentId << "\n";
            }
        } else if (c == 3) {
            std::string sId = inputNonEmpty("Nhập mã sinh viên: ");
            auto list = rentManager->findByStudent(sId);
            std::cout << "\n=== DANH SÁCH HỢP ĐỒNG CỦA SINH VIÊN " << sId << " (" << list.size() << " hợp đồng) ===\n";
            TablePrinter::printRents(list);
        } else if (c == 4) {
            std::string rId = inputNonEmpty("Nhập mã phòng: ");
            auto list = rentManager->findByRoom(rId);
            std::cout << "\n=== DANH SÁCH HỢP ĐỒNG TẠI PHÒNG " << rId << " (" << list.size() << " hợp đồng) ===\n";
            TablePrinter::printRents(list);
        } else if (c == 5) {
            std::cout << "\n--- GIA HẠN HỢP ĐỒNG THUÊ PHÒNG ---\n";
            std::string rentId = inputNonEmpty("Nhập mã hợp đồng cần gia hạn: ");
            int addMonths = inputInt("Số tháng gia hạn thêm (ví dụ: 1, 3, 6, 12 tháng): ");
            if (rentManager->extendRent(rentId, addMonths)) {
                rentManager->saveToFile();
            }
        } else if (c == 6) {
            Date d = inputValidDate("Nhập mốc ngày hiện tại để kiểm tra (dd/mm/yyyy, Enter để dùng 05/11/2026): ", Date(5, 11, 2026));
            int days = inputInt("Số ngày cảnh báo trước (Enter hoặc <=0 để dùng 30 ngày): ");
            if (days <= 0) days = 30;
            auto expiring = rentManager->findExpiringRents(d, days);
            std::cout << "\n=== HỢP ĐỒNG SẮP HẾT HẠN (TRONG VÒNG " << days << " NGÀY TÍNH TỪ " << d.toString() << ") ===\n";
            TablePrinter::printRents(expiring);
        } else if (c == 7) {
            Date d = inputValidDate("Nhập mốc ngày kiểm tra quá hạn (dd/mm/yyyy, Enter để dùng 05/11/2026): ", Date(5, 11, 2026));
            int count = rentManager->checkAndExpireRents(d);
            std::cout << "Tổng số hợp đồng đã quá hạn được giải phóng: " << count << "\n";
            if (count > 0) {
                saveAll();
            }
        }
        pauseScreen();
    }
}

void DormManager::handleRevenueManagement() {
    while (true) {
        clearScreen();
        printHeaderInfo();
        std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                 QUẢN LÝ DOANH THU & HÓA ĐƠN TIỀN PHÒNG                   ║\n";
        std::cout << "╠══════════════════════════════════════════════════════════════════════════╣\n";
        std::cout << "║  [1] Báo cáo doanh thu toàn bộ KTX theo tháng (Bảng tổng hợp chi tiết)   ║\n";
        std::cout << "║  [2] Tính doanh thu & In phiếu tính tiền của một phòng cụ thể theo tháng ║\n";
        std::cout << "║  [3] Xuất phiếu báo tiền phòng & chi phí sinh hoạt chia theo sinh viên   ║\n";
        std::cout << "║  [0] Quay lại Menu chính                                                 ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
        int c = inputInt(">> Lựa chọn: ");
        if (c == 0) break;

        if (c == 1) {
            std::string my = inputNonEmpty("Nhập tháng/năm báo cáo (ví dụ: 11/2026): ");
            double defElec = inputDouble("Ước tính chỉ số điện trung bình mỗi phòng (kWh, mặc định 50): ");
            if (defElec < 0) defElec = 50.0;
            double defWater = inputDouble("Ước tính chỉ số nước trung bình mỗi phòng (m³, mặc định 5): ");
            if (defWater < 0) defWater = 5.0;
            roomManage->printMonthlyRevenueReport(my, defElec, defWater);
        } else if (c == 2) {
            std::string rId = inputNonEmpty("Nhập mã phòng cần tính doanh thu (ví dụ: P101, P201): ");
            Room* r = roomManage->findById(rId);
            if (r == nullptr) {
                std::cout << "[Lỗi] Không tìm thấy phòng: " << rId << "\n";
            } else {
                std::string my = inputNonEmpty("Nhập tháng/năm (ví dụ: 11/2026): ");
                double elec = inputDouble("Nhập chỉ số điện tiêu thụ của phòng trong tháng (kWh): ");
                double water = inputDouble("Nhập chỉ số nước tiêu thụ của phòng trong tháng (m³): ");
                roomManage->printSpecificRoomRevenue(rId, my, elec, water);
            }
        } else if (c == 3) {
            std::string rId = inputNonEmpty("Nhập mã phòng cần xuất phiếu thông báo (ví dụ: P101, P201): ");
            Room* r = roomManage->findById(rId);
            if (r == nullptr) {
                std::cout << "[Lỗi] Không tìm thấy phòng: " << rId << "\n";
            } else {
                double elec = inputDouble("Nhập số điện tiêu thụ trong tháng (kWh): ");
                double water = inputDouble("Nhập số nước tiêu thụ trong tháng (m³): ");
                roomManage->printRoomBill(rId, elec, water);
            }
        }
        pauseScreen();
    }
}

void DormManager::handleReportManagement() {
    while (true) {
        clearScreen();
        printHeaderInfo();
        std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                        BÁO CÁO THỐNG KÊ KÝ TÚC XÁ                        ║\n";
        std::cout << "╠══════════════════════════════════════════════════════════════════════════╣\n";
        std::cout << "║  [1] Báo cáo tỷ lệ lấp đầy phòng KTX                                     ║\n";
        std::cout << "║  [2] Báo cáo tổng doanh thu tiền phòng hàng tháng                        ║\n";
        std::cout << "║  [3] Xem bảng doanh thu tổng hợp toàn KTX theo tháng                     ║\n";
        std::cout << "║  [0] Quay lại Menu chính                                                 ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
        int c = inputInt(">> Lựa chọn: ");
        if (c == 0) break;

        if (c == 1) {
            roomManage->printOccupancyReport();
        } else if (c == 2) {
            TablePrinter::printDetailedMonthlyRevenueReport(rentManager->getAll(), roomManage->getAll());
        } else if (c == 3) {
            std::string my = inputNonEmpty("Nhập tháng/năm báo cáo (ví dụ: 11/2026): ");
            roomManage->printMonthlyRevenueReport(my);
        }
        pauseScreen();
    }
}

void DormManager::handleAccountManagement() {
    while (true) {
        clearScreen();
        printHeaderInfo();
        std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                     QUẢN LÝ TÀI KHOẢN NGƯỜI DÙNG                         ║\n";
        std::cout << "╠══════════════════════════════════════════════════════════════════════════╣\n";
        std::cout << "║  [1] Xem danh sách toàn bộ tài khoản (Dạng Bảng)                         ║\n";
        std::cout << "║  [2] Cấp tài khoản mới                                                   ║\n";
        std::cout << "║  [3] Khóa / Mở khóa tài khoản                                            ║\n";
        std::cout << "║  [0] Quay lại Menu chính                                                 ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
        int c = inputInt(">> Lựa chọn: ");
        if (c == 0) break;

        if (c == 1) {
            std::cout << "\n=== TẤT CẢ TÀI KHOẢN TRONG HỆ THỐNG ===\n";
            TablePrinter::printAccounts(accountManager->getAll());
        } else if (c == 2) {
            std::string accId = inputNonEmpty("Mã tài khoản (ví dụ: ACC006): ");
            while (accountManager->findById(accId) != nullptr) {
                std::cout << "[Lỗi] Mã tài khoản '" << accId << "' đã tồn tại! Vui lòng nhập mã khác.\n";
                accId = inputNonEmpty("Mã tài khoản: ");
            }

            std::string user = inputNonEmpty("Tên đăng nhập: ");
            while (accountManager->findByUsername(user) != nullptr) {
                std::cout << "[Lỗi] Tên đăng nhập '" << user << "' đã được sử dụng! Vui lòng nhập tên khác.\n";
                user = inputNonEmpty("Tên đăng nhập: ");
            }

            std::string pass = inputNonEmpty("Mật khẩu khởi tạo: ");
            Role r = inputRole("Vai trò (1: STUDENT, 2: STAFF, 3: ADMIN): ");
            
            std::string owner;
            if (r == Role::STUDENT) {
                owner = inputNonEmpty("Mã sinh viên liên kết (ví dụ: SV101): ");
                while (true) {
                    Student* s = studentManage->findById(owner);
                    if (s == nullptr) {
                        std::cout << "[Lỗi] Không tìm thấy sinh viên có mã '" << owner << "' trong hệ thống!\n";
                        owner = inputNonEmpty("Vui lòng nhập lại Mã sinh viên: ");
                        continue;
                    }
                    Account* existAcc = accountManager->findByOwnerId(owner);
                    if (existAcc != nullptr) {
                        std::cout << "[Lỗi] Sinh viên '" << s->getFullName() << "' (" << owner
                                  << ") đã có tài khoản liên kết '" << existAcc->getUsername() << "'!\n";
                        owner = inputNonEmpty("Vui lòng nhập Mã sinh viên khác: ");
                        continue;
                    }
                    std::cout << "  -> Xác nhận liên kết thành công với sinh viên: " << s->getFullName()
                              << " (Lớp: " << s->getClass() << ")\n";
                    break;
                }
            } else if (r == Role::STAFF) {
                owner = inputNonEmpty("Mã cán bộ liên kết (ví dụ: CB001): ");
                while (true) {
                    Staff* st = staffManager->findById(owner);
                    if (st == nullptr) {
                        std::cout << "[Lỗi] Không tìm thấy cán bộ có mã '" << owner << "' trong hệ thống!\n";
                        owner = inputNonEmpty("Vui lòng nhập lại Mã cán bộ: ");
                        continue;
                    }
                    Account* existAcc = accountManager->findByOwnerId(owner);
                    if (existAcc != nullptr) {
                        std::cout << "[Lỗi] Cán bộ '" << st->getFullName() << "' (" << owner
                                  << ") đã có tài khoản liên kết '" << existAcc->getUsername() << "'!\n";
                        owner = inputNonEmpty("Vui lòng nhập Mã cán bộ khác: ");
                        continue;
                    }
                    std::cout << "  -> Xác nhận liên kết thành công với cán bộ: " << st->getFullName()
                              << " (Bộ phận: " << st->getDepartment() << ")\n";
                    break;
                }
            } else {
                std::string inputOwner = inputString("Mã liên kết quản trị (nhấn Enter để mặc định 'ADMIN'): ");
                if (inputOwner.empty() || inputOwner == "ADMIN") {
                    owner = "ADMIN";
                    std::cout << "  -> Mặc định mã liên kết Quản trị viên: ADMIN\n";
                } else {
                    Staff* st = staffManager->findById(inputOwner);
                    if (st != nullptr) {
                        owner = inputOwner;
                        std::cout << "  -> Gắn quyền Admin cho cán bộ: " << st->getFullName()
                                  << " (" << st->getDepartment() << ")\n";
                    } else {
                        owner = inputOwner;
                        std::cout << "  -> Thiết lập mã liên kết quản trị: " << owner << "\n";
                    }
                }
            }

            if (accountManager->createAccount(accId, user, pass, r, owner)) {
                accountManager->saveToFile();
            }
        } else if (c == 3) {
            std::string user = inputNonEmpty("Nhập tên đăng nhập cần đổi trạng thái: ");
            Account* acc = accountManager->findByUsername(user);
            if (acc == nullptr) {
                std::cout << "[Lỗi] Không tìm thấy tài khoản: " << user << "\n";
            } else {
                acc->setStatus(!acc->getStatus());
                accountManager->saveToFile();
                std::cout << "[Thành công] Tài khoản '" << user << "' hiện đã "
                          << (acc->getStatus() ? "KÍCH HOẠT" : "BỊ KHÓA") << ".\n";
            }
        }
        pauseScreen();
    }
}

void DormManager::handleChangePassword() {
    clearScreen();
        printHeaderInfo();
        std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                          ĐỔI MẬT KHẨU TÀI KHOẢN                          ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
    std::string oldPass = inputString("Nhập mật khẩu hiện tại: ");
    if (!currentAccount->verifyPassword(oldPass)) {
        std::cout << "[Lỗi] Mật khẩu hiện tại không chính xác!\n";
    } else {
        std::string newPass = inputString("Nhập mật khẩu mới: ");
        std::string confirm = inputString("Xác nhận mật khẩu mới: ");
        if (newPass != confirm) {
            std::cout << "[Lỗi] Mật khẩu xác nhận không trùng khớp!\n";
        } else {
            currentAccount->changePassword(newPass);
            accountManager->saveToFile();
            std::cout << "[Thành công] Đổi mật khẩu thành công!\n";
        }
    }
    pauseScreen();
}
