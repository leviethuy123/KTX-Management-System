#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
#include "common/Enums.h"
#include "common/Date.h"
#include "model/Student.h"
#include "model/Staff.h"
#include "model/StandardRoom.h"
#include "model/VipRoom.h"
#include "model/Rent.h"

using json = nlohmann::json;

int main() {
    std::cout << "=========================================================" << std::endl;
    std::cout << "  HE THONG QUAN LY KTX - LUU TRU DU LIEU BANG JSON       " << std::endl;
    std::cout << "=========================================================\n" << std::endl;

    // 1. Thử nghiệm Đọc dữ liệu từ file JSON (data/students.json)
    std::cout << "--- 1. DOC DANH SACH SINH VIEN TU data/students.json ---" << std::endl;
    std::vector<Student> studentList;
    {
        std::ifstream file("data/students.json");
        if (file.is_open()) {
            json jArr;
            file >> jArr;
            for (const auto& item : jArr) {
                studentList.push_back(Student::fromJson(item));
            }
            std::cout << "-> Da doc thanh cong " << studentList.size() << " sinh vien tu file JSON:" << std::endl;
            for (const auto& sv : studentList) {
                sv.print();
            }
        } else {
            std::cout << "-> Khong mo duoc file data/students.json!" << std::endl;
        }
    }

    // 2. Thử nghiệm Đọc danh sách phòng từ file JSON (data/rooms.json) với tính Đa hình
    std::cout << "\n--- 2. DOC DANH SACH PHONG TU data/rooms.json (DA HINH) ---" << std::endl;
    std::vector<Room*> roomList;
    {
        std::ifstream file("data/rooms.json");
        if (file.is_open()) {
            json jArr;
            file >> jArr;
            for (const auto& item : jArr) {
                std::string type = item.value("roomType", "STANDARD");
                if (type == "VIP") {
                    roomList.push_back(new VipRoom(VipRoom::fromJson(item)));
                } else {
                    roomList.push_back(new StandardRoom(StandardRoom::fromJson(item)));
                }
            }
            std::cout << "-> Da doc thanh cong " << roomList.size() << " phong tu file JSON:" << std::endl;
            for (Room* r : roomList) {
                r->print();
                std::cout << "  --> Phi du kien (50 kWh dien, 3 m3 nuoc): "
                          << static_cast<long long>(r->calculateTotalFee(50, 3)) << " VND/thang" << std::endl;
            }
        }
    }

    // 3. Thử nghiệm Đọc hợp đồng từ data/rents.json
    std::cout << "\n--- 3. DOC HOP DONG THUE TU data/rents.json ---" << std::endl;
    std::vector<Rent> rentList;
    {
        std::ifstream file("data/rents.json");
        if (file.is_open()) {
            json jArr;
            file >> jArr;
            for (const auto& item : jArr) {
                std::string sId = item.value("studentId", "");
                std::string rId = item.value("roomId", "");

                Student* foundStudent = nullptr;
                for (auto& s : studentList) {
                    if (s.getId() == sId) {
                        foundStudent = &s;
                        break;
                    }
                }

                Room* foundRoom = nullptr;
                for (auto* r : roomList) {
                    if (r->getId() == rId) {
                        foundRoom = r;
                        break;
                    }
                }

                Rent rObj = Rent::fromJson(item, foundStudent, foundRoom);
                rentList.push_back(rObj);
            }

            std::cout << "-> Da doc thanh cong " << rentList.size() << " hop dong tu file JSON:" << std::endl;
            for (auto& hd : rentList) {
                hd.print();
                std::cout << "  --> Tong gia tri: " << static_cast<long long>(hd.calculateTotalAmount())
                          << " VND | Han chot: " << hd.getExpectedEndDate().toString() << std::endl;
                
                // Gan vao phong
                if (hd.getRoom() != nullptr) {
                    hd.getRoom()->addRent(&hd);
                }
            }
        }
    }

    std::cout << "\nTrang thai phong sau khi nap hop dong tu JSON:" << std::endl;
    for (Room* r : roomList) {
        r->print();
    }

    // 4. Thử nghiệm Thêm sinh viên mới & Xuất ngược lại ra JSON
    std::cout << "\n--- 4. GHI DU LIEU MOI NGUOC LAI FILE JSON ---" << std::endl;
    Student svMoi("SV104", "Vu Thi Huong", false, "2005", "0945678901",
                  "huong.vt@sv.dut.udn.vn", "Quang Tri", "23T_DT1", "CNTT", "K23");
    studentList.push_back(svMoi);
    std::cout << "-> Da them sinh vien moi: " << svMoi.getFullName() << std::endl;

    // Xuất ra chuỗi JSON
    json exportArray = json::array();
    for (const auto& s : studentList) {
        exportArray.push_back(s.toJson());
    }

    std::cout << "\nDu lieu JSON xuat ra (Mau 1 ban ghi):" << std::endl;
    std::cout << svMoi.serialize() << std::endl;

    // Ghi đè vào data/students.json
    {
        std::ofstream outFile("data/students.json");
        if (outFile.is_open()) {
            outFile << exportArray.dump(2);
            std::cout << "-> Da luu thanh cong toan bo " << studentList.size()
                      << " sinh vien vao data/students.json!" << std::endl;
        }
    }

    // Dọn dẹp con trỏ
    for (Room* r : roomList) {
        delete r;
    }
    roomList.clear();

    std::cout << "\n=========================================================" << std::endl;
    std::cout << "  CHUYEN DOI HOAN TOAN SANG JSON THANH CONG 100%!       " << std::endl;
    std::cout << "=========================================================" << std::endl;

    return 0;
}
