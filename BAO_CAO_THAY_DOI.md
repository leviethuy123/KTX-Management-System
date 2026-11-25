# 📋 BÁO CÁO THAY ĐỔI & TIẾN ĐỘ DỰ ÁN

> **Dự án:** Quản lý Ký túc xá Sinh viên (Hệ thống PBL2 - C++17 OOP)  
> **Thời điểm cập nhật:** 22/11/2026 (ICT)  
> **Trạng thái:** Hoàn thành toàn diện các yêu cầu nâng cấp & Kiểm thử 100% đạt chuẩn

---

## 🎯 1. TỔNG QUAN CÁC MỤC TIÊU NÂNG CẤP HỆ THỐNG (PHIÊN 24/11/2026)

Trong đợt nâng cấp toàn diện này, hệ thống đã hoàn thiện các yêu cầu kỹ thuật cốt lõi của đề tài PBL2: thay thế cấu trúc dữ liệu thư viện có sẵn bằng cấu trúc mảng động tự định nghĩa, mở rộng tính Đa hình (Polymorphism) trong lập trình hướng đối tượng, tối ưu hóa giao diện tương tác người dùng và bổ sung các chức năng nghiệp vụ quản lý phòng ở chuyên sâu:

1. **Tự thiết kế & Triển khai cấu trúc dữ liệu `Vector<T>` thay thế hoàn toàn `std::vector`:**
   - Thay vì sử dụng thư viện STL `std::vector`, đồ án đã tự xây dựng class mẫu (class template) `Vector<T>` quản lý mảng động với cấp phát bộ nhớ động (`new[]`, `delete[]`), cơ chế tự động nhân đôi sức chứa (`reserve`, `capacity`), các phương thức thêm/xóa/truy cập (`push_back`, `pop_back`, `erase`, `clear`, `operator[]`, `at`), bẫy ngoại lệ `std::out_of_range`, hỗ trợ cơ chế dịch chuyển `std::move` và con trỏ duyệt `begin()`, `end()` tương thích hoàn hảo với vòng lặp `range-for` và thuật toán sắp xếp `std::sort`.
   - Lược bỏ hàm tạo `initializer_list` dư thừa theo đúng yêu cầu đề tài, đảm bảo tính tinh gọn, an toàn và tối ưu bộ nhớ.
   - Chuyển đổi toàn bộ các lớp quản lý (`Manager<T>`, `RoomManager`, `StudentManager`, `StaffManager`, `RentManager`, `AccountManager`, `DormManager`) sang sử dụng `Vector<T>`.

2. **Gộp module `TablePrinter` vào `Common.h` và `Common.cpp`:**
   - Xóa bỏ 2 tệp riêng biệt `include/TablePrinter.h` và `src/TablePrinter.cpp`.
   - Khai báo lớp `TablePrinter` và namespace `StringUtils` được tích hợp vào `include/Common.h`.
   - Toàn bộ phần cài đặt của `TablePrinter` và `StringUtils` được chuyển vào `src/Common.cpp`.
   - Không phụ thuộc vào `Makefile`, dự án có thể biên dịch trực tiếp và nhất quán bằng lệnh `g++`.

3. **Mở rộng tính Đa hình (Polymorphism) với Quy định riêng của từng phòng:**
   - Bổ sung phương thức thuần ảo `virtual std::string getRegulations() const = 0;` trong lớp trừu tượng `Room`.
   - Cài đặt ghi đè (`override`) độc lập tại 2 lớp dẫn xuất:
     - **StandardRoom:** *"Đóng cửa 23:00 - 05:00 | Không nấu ăn trong phòng | Trực nhật dọn vệ sinh hàng tuần | Giữ trật tự sau 22:00"*.
     - **VipRoom:** *"Ra vào tự do 24/7 (thẻ từ) | Cho phép khu bếp riêng & thiết bị mini | Dịch vụ dọn phòng 2 lần/tuần | Bảo quản nội thất cao cấp"*.
   - Quy định riêng được tự động xuất ra khi xem thông tin chi tiết phòng (`Room::print()`).

4. **Thêm chức năng Tạo phòng mới (`handleCreateRoom`):**
   - Bổ sung lựa chọn `[8] Thêm phòng mới vào hệ thống` trong Menu Quản lý phòng ở của Cán bộ / Admin.
   - Cung cấp phương thức nghiệp vụ `createRoom(...)` trong `RoomManager`.
   - Bẫy lỗi trùng lặp mã phòng, nhập loại phòng (Standard / VIP), tòa nhà, tầng, sức chứa, đơn giá cơ bản và phí dịch vụ phát sinh; tự động lưu trực tiếp vào cơ sở dữ liệu `data/rooms.json`.

5. **Tối ưu hóa giao diện nhập liệu & Trực quan hóa thông tin trước khi nhập (Hạn chế tối đa lỗi người dùng):**
   - **Nhập giới tính:** Hiển thị card lựa chọn trực quan `[1] Nam | [2] Nữ` trước khi nhập (`inputGender`).
   - **Xếp phòng mới:**
     - Tự động hiển thị danh sách các sinh viên đang chờ xếp phòng trước khi yêu cầu nhập mã sinh viên.
     - Sau khi chọn sinh viên, hệ thống tự động lọc và hiển thị danh sách các phòng còn chỗ trống **phù hợp theo giới tính** của sinh viên đó trước khi yêu cầu nhập mã phòng.

6. **Lược bỏ các hiển thị tự động dư thừa khi thêm sinh viên:**
   - Gỡ bỏ 2 thông báo không cần thiết: `-> Tự động xác định Khóa...` và `-> Tự động xác định Khoa...` trong giao diện thêm mới sinh viên, giữ màn hình nhập liệu ngắn gọn, sạch sẽ.

7. **Bổ sung tính năng Lọc đa tiêu chí trong Phân bổ & Xếp phòng (`handleFilterAllocation`):**
   - Bổ sung lựa chọn `[6] Tìm kiếm & Lọc phòng xếp chỗ theo tiêu chí` trong Menu Phân bổ phòng, hỗ trợ:
     - Lọc các phòng còn chỗ trống phù hợp cho Nam.
     - Lọc các phòng còn chỗ trống phù hợp cho Nữ.
     - Lọc phòng còn chỗ theo Loại phòng (Standard / VIP).
     - Lọc phòng còn chỗ theo Tòa nhà (Khu A, Khu B, Khu VIP,...).
     - Lọc danh sách sinh viên chưa có phòng theo từng Khoa trong 8 Khoa chuẩn của Trường Đại học Bách Khoa.

8. **Chuẩn hóa & Sinh bộ dữ liệu mẫu lớn (`data/*.json`):**
   - Mở rộng bộ dữ liệu JSON mô phỏng hoạt động thực tế đầy đủ: 20 phòng (10 Standard, 10 VIP), 50 sinh viên, 10 cán bộ quản lý & kế toán, 46 hợp đồng thuê phòng (đa dạng trạng thái ACTIVE, EXPIRED, TERMINATED) và 62 tài khoản đăng nhập phân quyền (Admin, Staff, Student).

---

## 📊 2. BẢNG THỐNG KÊ CHI TIẾT CÁC TỆP TIN THAY ĐỔI

| STT | Tệp tin (File Path) | Loại thay đổi | Trọng tâm kỹ thuật |
| :---: | :--- | :---: | :--- |
| **1** | `include/Common.h` | Cập nhật lớn | Cài đặt class template `Vector<T>` tự định nghĩa; Tích hợp khai báo `class TablePrinter` và `namespace StringUtils`; Đồng bộ 8 Khoa chuẩn `Faculty`. |
| **2** | `src/Common.cpp` | Cập nhật lớn | Chuyển toàn bộ cài đặt `TablePrinter` và `StringUtils` sang; Cài đặt xử lý hiển thị bảng định dạng UTF-8, format tiền tệ và in sơ đồ phòng. |
| **3** | `include/TablePrinter.h` | Đã xóa | Xóa file riêng lẻ theo yêu cầu gộp mã nguồn vào `Common.h`. |
| **4** | `src/TablePrinter.cpp` | Đã xóa | Xóa file riêng lẻ theo yêu cầu gộp mã nguồn vào `Common.cpp`. |
| **5** | `include/Room.h` | Cập nhật | Khai báo phương thức thuần ảo `virtual std::string getRegulations() const = 0;` trong `Room`; ghi đè `override` trong `StandardRoom` và `VipRoom`. |
| **6** | `src/Room.cpp` | Cập nhật | Cài đặt `StandardRoom::getRegulations()` và `VipRoom::getRegulations()`; xuất quy định riêng trong `Room::print()`. |
| **7** | `include/manager/RoomManager.h` | Cập nhật | Đổi toàn bộ sang `Vector<Room*>`; khai báo `createRoom(...)` và `findAvailableRoomByGender(...)`. |
| **8** | `src/manager/RoomManager.cpp` | Cập nhật | Cài đặt `createRoom(...)` và `findAvailableRoomByGender(...)`; chuyển đổi các thao tác nội bộ sang `Vector<T>`. |
| **9** | `include/manager/DormManager.h` | Cập nhật | Khai báo các hàm điều khiển mới: `handleCreateRoom()`, `handleFilterAllocation()`, `getUnassignedStudents()`. |
| **10** | `src/manager/DormManager.cpp` | Cập nhật lớn | Cài đặt menu Tạo phòng [8]; Menu Lọc phân bổ [6]; Giao diện chọn giới tính card; Hiển thị danh sách trước khi nhập mã SV và mã phòng; Lược bỏ in thừa Khóa/Khoa. |
| **11** | `include/manager/Manager.h` | Cập nhật | Chuyển mảng chứa nội bộ từ `std::vector<T*>` sang `Vector<T*>`. |
| **12** | `include/manager/StudentManager.h` | Cập nhật | Chuyển kiểu dữ liệu trả về sang `Vector<Student*>`. |
| **13** | `src/manager/StudentManager.cpp` | Cập nhật | Đồng bộ phương thức lọc và nạp dữ liệu với `Vector<Student*>`. |
| **14** | `include/manager/StaffManager.h` | Cập nhật | Chuyển kiểu dữ liệu trả về sang `Vector<Staff*>`. |
| **15** | `src/manager/StaffManager.cpp` | Cập nhật | Đồng bộ phương thức nạp và quản lý với `Vector<Staff*>`. |
| **16** | `include/manager/RentManager.h` | Cập nhật | Chuyển danh sách hợp đồng sang `Vector<Rent*>`. |
| **17** | `src/manager/RentManager.cpp` | Cập nhật | Đồng bộ nghiệp vụ tạo, chuyển, gia hạn và thanh lý hợp đồng với `Vector<Rent*>`. |
| **18** | `include/manager/AccountManager.h` | Cập nhật | Bỏ `include <vector>`, sử dụng `Vector<Account*>`. |
| **19** | `data/rooms.json` | Cập nhật dữ liệu | Lưu trữ 20 phòng (10 Standard, 10 VIP) với đầy đủ thông tin thiết bị hỏng thực tế. |
| **20** | `data/students.json` | Cập nhật dữ liệu | Lưu trữ 50 sinh viên chuẩn hóa thông tin cá nhân, lớp học và khoa. |
| **21** | `data/staffs.json` | Cập nhật dữ liệu | Lưu trữ 10 cán bộ phân công các bộ phận KTX. |
| **22** | `data/rents.json` | Cập nhật dữ liệu | Lưu trữ 46 hợp đồng thuê với đa dạng trạng thái. |
| **23** | `data/accounts.json` | Cập nhật dữ liệu | Lưu trữ 62 tài khoản người dùng tương ứng cho toàn bộ cán bộ và sinh viên. |

---

## 🔍 3. CHI TIẾT CÁC ĐIỂM SÁNG KỸ THUẬT & KIẾN TRÚC OOP

### 🌟 3.1. Cấu trúc dữ liệu tự định nghĩa `Vector<T>`
```cpp
template <typename T>
class Vector {
private:
    T* arr = nullptr;           // Mảng động lưu trữ dữ liệu
    std::size_t n = 0;          // Số lượng phần tử thực tế
    std::size_t cap = 0;        // Sức chứa bộ nhớ hiện tại
public:
    Vector() = default;
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;
    Vector& operator=(Vector other) noexcept;
    ~Vector() { delete[] arr; }

    std::size_t size() const { return n; }
    std::size_t capacity() const { return cap; }
    bool empty() const { return n == 0; }

    T& operator[](std::size_t i) { return arr[i]; }
    const T& operator[](std::size_t i) const { return arr[i]; }
    T& at(std::size_t i);

    void reserve(std::size_t newCap);
    void push_back(T x);
    void pop_back();
    void clear() { n = 0; }
    void erase(std::size_t i);
    void swap(Vector& other) noexcept;

    T* begin() { return arr; }
    T* end() { return arr + n; }
    const T* begin() const { return arr; }
    const T* end() const { return arr + n; }
};
```
- **Ưu điểm thiết kế:**
  - Tự quản lý vòng đời tài nguyên bộ nhớ động tuân thủ quy tắc **Rule of Five / Copy-and-Swap idiom**.
  - Đảm bảo exception safety và tránh memory leak.
  - Cung cấp `begin()` và `end()` cho phép sử dụng cú pháp C++ hiện đại `for (T* item : list)` và gọi các hàm thuật toán chuẩn `std::sort`.

---

### 🌟 3.2. Tính Đa hình (Polymorphism) trong Quy định phòng ở
- Lớp trừu tượng `Room` đặt nền tảng giao diện:
  ```cpp
  virtual std::string getRegulations() const = 0;
  ```
- Lớp con `StandardRoom` và `VipRoom` định nghĩa quy tắc sinh hoạt riêng biệt:
  - Phòng Standard nhấn mạnh quy định an toàn chung, giờ đóng cửa và phân công trực nhật dọn dẹp.
  - Phòng VIP tập trung vào tiện ích cao cấp, quyền ra vào tự do qua thẻ từ, tiêu chuẩn sử dụng thiết bị điện/bếp và dịch vụ dọn phòng.
- Trong hàm `Room::print()`, lời gọi `getRegulations()` được liên kết động tại thời điểm chạy (Dynamic Binding), thể hiện rõ tính đa hình hướng đối tượng.

---

### 🌟 3.3. Thuật toán phân bổ & Lọc phòng thông minh theo Giới tính
- Phương thức `RoomManager::findAvailableRoomByGender(Gender g)` kiểm tra tính toàn vẹn:
  1. Phòng phải còn chỗ (`!r->isFull()`) và không trong trạng thái bảo trì (`status != MAINTENANCE`).
  2. Nếu phòng hoàn toàn trống (`r->isEmpty()`): chấp nhận xếp cho giới tính `g`.
  3. Nếu phòng đã có người ở: quét toàn bộ danh sách hợp đồng active trong phòng, nếu tất cả sinh viên cùng phòng đều cùng giới tính `g` thì phòng đó mới đủ điều kiện cho sinh viên giới tính `g`.
- Nhờ vậy, hệ thống tuyệt đối không bao giờ để xảy ra xung đột giới tính nam/nữ trong cùng một phòng ở KTX.

---

## 🧪 4. KẾT QUẢ KIỂM THỬ THỰC TẾ

1. **Biên dịch dự án:**
   - Sử dụng trình biên dịch `g++` (chuẩn C++17):
     ```powershell
     g++ -std=c++17 -Wall -Wextra -Iinclude main.cpp src/*.cpp src/manager/*.cpp -o app.exe
     ```
   - **Kết quả:** Biên dịch thành công với **0 lỗi, 0 cảnh báo (Zero Warnings / Zero Errors)**.

2. **Kiểm thử tự động các tính năng:**
   - **Test 1 (Tạo phòng mới):** Tạo thành công phòng `P301`, lưu tức thì vào `data/rooms.json`, tra cứu chi tiết hiển thị đúng đầy đủ thông số.
   - **Test 2 (Quy định phòng - Đa hình):** Kiểm tra hiển thị quy định riêng của phòng Standard (`P101`) và phòng VIP (`V101`) thành công 100%.
   - **Test 3 & 4a (Thêm sinh viên):** Giao diện hiển thị card chọn giới tính `[1] Nam | [2] Nữ`, không còn xuất hiện các dòng in thừa tự động xác định Khóa/Khoa.
   - **Test 4b (Phân bổ phòng trực quan):** Hiển thị danh sách sinh viên đang chờ phòng trước khi nhập mã sinh viên; sau khi chọn sinh viên, hiển thị danh sách phòng trống phù hợp giới tính trước khi nhập mã phòng.
   - **Test 5 (Bộ lọc phân bổ):** Lọc phòng trống cho Nam, cho Nữ, theo Loại VIP/Standard, theo Tòa nhà và lọc sinh viên chưa xếp phòng theo Khoa đều cho kết quả chính xác 100%.

3. **Tính toàn vẹn dữ liệu:**
   - Toàn bộ các file trong thư mục `data/` được đồng bộ, bảo đảm tính nhất quán giữa Sinh viên, Cán bộ, Hợp đồng, Phòng và Tài khoản người dùng.

---

> [!IMPORTANT]
