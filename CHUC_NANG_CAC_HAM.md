# 📑 TỔNG HỢP CHỨC NĂNG CÁC HÀM TRONG DỰ ÁN KTX-MANAGEMENT-SYSTEM
> **Tài liệu tham khảo nhanh phục vụ học tập và bảo vệ đồ án PBL2**  
> *Được biên soạn ngắn gọn, súc tích theo cấu trúc 7 module đã gộp gọn gàng.*

---

## 📌 MODULE 1: NỀN TẢNG DÙNG CHUNG (`include/Common.h` & `src/Common.cpp`)

### 1. Lớp Interface `IEntity` (Quy chuẩn chung cho mọi thực thể)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `getId()` | Trả về mã định danh duy nhất của thực thể (mã phòng, mã SV, mã hợp đồng,...). |
| `print()` | In thông tin chi tiết của đối tượng ra màn hình Console định dạng đẹp. |
| `toJson()` | Chuyển đổi đối tượng C++ thành cấu trúc dữ liệu JSON (`nlohmann::json`). |
| `serialize()` | Chuyển đổi đối tượng thành chuỗi JSON String có thụt dòng (indent 2) để lưu file. |

### 2. Lớp Xử lý Ngày tháng `Date`
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `Date(d, m, y)` | Khởi tạo ngày tháng năm; tự động sửa về ngày hợp lệ nếu dữ liệu sai. |
| `isValid()` | Kiểm tra ngày tháng có hợp lệ không (xét năm nhuận, số ngày trong tháng 1-12). |
| `toString()` | Xuất chuỗi định dạng chuẩn `dd/mm/yyyy` (ví dụ: `01/09/2026`). |
| `fromString(str)` | Chuyển chuỗi `dd/mm/yyyy` ngược lại thành đối tượng `Date`. |
| `addMonths(m)` | Cộng thêm `m` tháng vào ngày hiện tại (dùng để tính ngày hết hạn hợp đồng). |
| `toDays()` | Đổi ngày tháng năm thành tổng số ngày tuyệt đối tính từ năm 1900. |
| `daysBetween(other)` | Tính chính xác khoảng cách số ngày giữa 2 mốc thời gian (`other - this`). |
| `operator<, <=, >, ==` | Nạp chồng toán tử so sánh thứ tự thời gian trước/sau giữa hai ngày. |

### 3. Các kiểu liệt kê & Hàm chuyển đổi chuỗi (`namespace EnumHelper`)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `genderToString / stringToGender` | Chuyển đổi qua lại giữa `Gender` (MALE, FEMALE) và chuỗi tiếng Việt. |
| `roleToString / stringToRole` | Chuyển đổi qua lại giữa vai trò `Role` (STUDENT, STAFF, ADMIN) và chuỗi. |
| `roomStatusToString / stringToRoomStatus` | Chuyển đổi trạng thái phòng (EMPTY, OCCUPIED, FULL, MAINTENANCE). |
| `rentStatusToString / stringToRentStatus` | Chuyển đổi trạng thái hợp đồng (ACTIVE, EXPIRED, TERMINATED). |
| `periodToMonths / intToPeriod` | Chuyển đổi giữa kỳ hạn hợp đồng `Period` và số tháng nguyên (1, 3, 6, 12). |

---

## 👥 MODULE 2: CÂY CON NGƯỜI (`include/Human.h` & `src/Human.cpp`)

### 1. Lớp cơ sở trừu tượng `Human`
*Lưu trữ thông tin cá nhân: Họ tên, giới tính, ngày sinh, số điện thoại, email, địa chỉ.*
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `getFullName() / setFullName()` | Lấy hoặc cập nhật họ và tên. |
| `getPhone(), getEmail(),...` | Các Getter / Setter lấy và sửa thông tin liên lạc, quê quán. |
| `getId()` *(thuần ảo = 0)* | Bắt buộc lớp con (`Student`, `Staff`) phải tự định nghĩa mã cá nhân. |

### 2. Lớp `Student` (Kế thừa `Human`)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `getId()` | Trả về mã sinh viên `studentId` (ví dụ: `SV101`). |
| `getClass(), getFaculty(), getCohort()` | Lấy tên lớp sinh hoạt, khoa và khóa học của sinh viên. |
| `getRent() / setRent()` | Lấy hoặc liên kết con trỏ hợp đồng thuê phòng hiện tại của sinh viên. |
| `print()` | In thông tin cá nhân, mã sinh viên, lớp, khoa và thông tin liên hệ. |
| `toJson() / fromJson()` | Đóng gói thông tin sinh viên sang JSON và trích xuất từ JSON vào đối tượng. |

### 3. Lớp `Staff` (Kế thừa `Human`)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `getId()` | Trả về mã cán bộ `employeeId` (ví dụ: `CB001`). |
| `getDepartment(), getPosition()` | Lấy thông tin phòng ban công tác và chức vụ của cán bộ quản lý KTX. |
| `print()` | In thông tin hồ sơ cán bộ quản lý KTX. |
| `toJson() / fromJson()` | Đóng gói và đọc thông tin cán bộ từ định dạng JSON. |

---

## 🏢 MODULE 3: CÂY PHÒNG Ở ĐA HÌNH (`include/Room.h` & `src/Room.cpp`)

### 1. Lớp cơ sở trừu tượng `Room`
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `getId()` | Trả về mã phòng `roomId` (ví dụ: `P101`, `P201`). |
| `addRent(Rent*)` | Thêm hợp đồng sinh viên vào phòng; tự động đổi trạng thái sang `OCCUPIED` hoặc `FULL`. |
| `removeRent(Rent*)` | Gỡ hợp đồng khi sinh viên trả phòng; tự động đổi trạng thái về `EMPTY` hoặc `OCCUPIED`. |
| `isEmpty()` | Kiểm tra phòng có đang trống hoàn toàn hay không (`activeRent.empty()`). |
| `isFull()` | Kiểm tra phòng đã đủ người chưa (`activeRent.size() >= maxStudents`). |
| `calculateTotalFee()` *(thuần ảo)* | Phương thức đa hình tính tổng tiền phòng (được lớp con ghi đè). |
| `getRoomType()` *(thuần ảo)* | Trả về loại phòng (`STANDARD` hoặc `VIP`). |

### 2. Lớp `StandardRoom` (Phòng Tiêu chuẩn - Kế thừa `Room`)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `calculateTotalFee(elec, water)` | **Đa hình:** $\text{Giá gốc (400k)} + (\text{Điện} \times 3.000) + (\text{Nước} \times 10.000)$. |
| `getAmenities()` | Trả về danh sách tiện ích cơ bản (quạt, bàn học, ghế). |
| `toJson() / fromJson()` | Đọc và ghi dữ liệu phòng thường cùng trang thiết bị kèm theo. |

### 3. Lớp `VipRoom` (Phòng VIP dịch vụ cao cấp - Kế thừa `Room`)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `calculateTotalFee(elec, water)` | **Đa hình:** $\text{Giá gốc (1.2tr)} + \text{Phí VIP (250k)} + (\text{Điện} \times 3.000) + (\text{Nước} \times 10.000)$. |
| `getAmenities()` | Trả về danh sách tiện ích VIP (điều hòa, bình nóng lạnh, wifi, dọn phòng). |
| `getServiceFee()` | Lấy mức phí dịch vụ cộng thêm của phòng VIP. |

---

## 📜 MODULE 4: HỢP ĐỒNG THUÊ KTX (`include/Rent.h` & `src/Rent.cpp`)

| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `getId()` | Trả về mã hợp đồng `rentId` (ví dụ: `HD001`). |
| `getExpectedEndDate()` | Tính ngày kết thúc dự kiến = Ngày bắt đầu + Số tháng thuê (`startDate.addMonths()`). |
| `terminateEarly(actualDate)` | Chấm dứt hợp đồng sớm trước hạn, đổi trạng thái sang `TERMINATED`. |
| `isActive()` | Kiểm tra hợp đồng có đang hiệu lực không (`status == RentStatus::ACTIVE`). |
| `calculateTotalAmount()` | Tính tổng tiền thuê cả kỳ = `rentalPeriod * rentFee`. |
| `toJson() / fromJson()` | Lưu trữ thông tin hợp đồng liên kết giữa `studentId` và `roomId`. |

---

## 🔐 MODULE 5: TÀI KHOẢN NGƯỜI DÙNG (`include/Account.h` & `src/Account.cpp`)

| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `getId()` | Trả về mã tài khoản `accountId` (ví dụ: `ACC001`). |
| `hashPassword(password)` | Thuật toán băm mật khẩu DJB2 sang chuỗi Hex để lưu trữ an toàn. |
| `verifyPassword(password)` | Kiểm tra mật khẩu người dùng nhập vào có khớp với mã băm hay không. |
| `changePassword(newPass)` | Đổi mật khẩu mới và tự động băm lại mật khẩu. |
| `getRole()` | Lấy quyền hạn của tài khoản (`STUDENT`, `STAFF`, hoặc `ADMIN`). |

---

## ⚙️ MODULE 6: TẦNG QUẢN LÝ REPOSITORY (`include/Managers.h` & `src/Managers.cpp`)

### 1. Khuôn mẫu Generic `Manager<T>`
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `add(T* item)` | Thêm đối tượng mới; tự động kiểm tra trùng mã ID. |
| `remove(id)` | Xóa đối tượng: giải phóng vùng nhớ `delete *it` rồi xóa khỏi `vector`. |
| `findById(id)` | Tìm kiếm và trả về con trỏ `T*` của đối tượng có mã ID tương ứng. |
| `printAll()` | Duyệt toàn bộ danh sách `vector<T*>` và in ra màn hình. |
| `clear()` | Giải phóng toàn bộ con trỏ trong vector để chống rò rỉ bộ nhớ (Memory Leak). |

### 2. Các Manager nghiệp vụ chuyên biệt
| Manager | Các hàm nghiệp vụ đặc trưng |
| :--- | :--- |
| **`StudentManager`** | `findByClass()` (lọc theo lớp), `findByFaculty()` (lọc theo khoa). |
| **`RoomManager`** | `findAvailableRoom()` (tìm phòng còn chỗ), `printOccupancyReport()` (báo cáo lấp đầy KTX), `printRoomBill()` (tính tiền phòng & chia đều tiền điện nước). |
| **`StaffManager`** | `findByDepartment()` (lọc cán bộ theo phòng ban). |
| **`RentManager`** | `createRent()` (xếp phòng an toàn), `terminateRent()` (thanh lý hợp đồng), `findExpiringRents()` (báo trước 30 ngày hết hạn), `checkAndExpireRents()` (tự động giải phóng phòng quá hạn), `calculateTotalMonthlyRevenue()` (tổng doanh thu tháng). |
| **`AccountManager`** | `authenticate()` (xác thực đăng nhập), `findByUsername()`, `createAccount()` (cấp tài khoản mới). |

---

## 🎮 MODULE 7: ĐIỀU PHỐI MẶT TIỀN FACADE (`include/DormManager.h` & `src/DormManager.cpp`)

| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `loadAll() / saveAll()` | Nạp và lưu đồng thời toàn bộ dữ liệu của tất cả 5 Manager xuống file JSON. |
| `login(user, pass)` | Xác thực đăng nhập và gán phiên làm việc vào con trỏ `currentAccount`. |
| `logout()` | Hủy phiên làm việc hiện tại, đưa hệ thống về màn hình chờ. |
| `getCurrentUserRole()` | Lấy quyền hạn của người đang đăng nhập (`ADMIN`, `STAFF`, `STUDENT`). |
| `runMainMenu()` | Vòng lặp điều hướng chính, tự động mở Menu tương ứng theo quyền hạn. |
| `runAdminMenu()` | Bàn làm việc toàn quyền của Quản trị viên hệ thống. |
| `runStaffMenu()` | Bàn làm việc của Cán bộ KTX (xếp phòng, quản lý SV, hóa đơn điện nước). |
| `runStudentMenu()` | Cổng thông tin của Sinh viên (hồ sơ, bạn cùng phòng, xem hóa đơn tháng). |
| `inputString()` | Xử lý nhập chuỗi an toàn: tự động cắt bỏ ký tự `\r` và mã UTF-8 BOM chống trôi lệnh trên Windows. |

---

## 🚀 ĐIỂM KHỞI CHẠY CHƯƠNG TRÌNH (`main.cpp`)

Chỉ gồm 16 dòng code thanh lịch theo chuẩn Facade Pattern:
```cpp
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    DormManager app;
    app.runMainMenu();

    return 0;
}
```
