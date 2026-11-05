# 📑 TỔNG HỢP CHỨC NĂNG CÁC HÀM TRONG DỰ ÁN KTX-MANAGEMENT-SYSTEM
> **Tài liệu tham khảo nhanh phục vụ học tập và bảo vệ đồ án PBL2**  
> *Được biên soạn ngắn gọn, súc tích theo cấu trúc 8 module hiện đại.*

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
| `tryParse(str, outDate)` | **Bẫy lỗi:** Phân tích cú pháp chuỗi ngày tháng, trả về `true` nếu hợp lệ và gán vào `outDate`. |
| `addMonths(m)` | Cộng thêm `m` tháng vào ngày hiện tại (dùng để tính ngày hết hạn hợp đồng). |
| `toDays()` | Đổi ngày tháng năm thành tổng số ngày tuyệt đối tính từ năm 1900. |
| `daysBetween(other)` | Tính chính xác khoảng cách số ngày giữa 2 mốc thời gian (`other - this`). |
| `operator<, <=, >, ==` | Nạp chồng toán tử so sánh thứ tự thời gian trước/sau giữa hai ngày. |

### 3. Lớp Chuẩn hóa Khoa `Faculty` (Đồng bộ 10 Khoa ĐH Bách Khoa)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `getList()` | Trả về danh sách chuẩn gồm 10 khoa đại học đào tạo tại trường. |
| `printMenu()` | Hiển thị menu chọn 1..10 tên khoa chuẩn có đánh số. |
| `getByIndex(index)` | Lấy tên khoa chuẩn tương ứng với chỉ số người dùng chọn (1 đến 10). |
| `isValidIndex(index)` | Kiểm tra lựa chọn khoa có nằm trong khoảng 1..10 hay không. |
| `normalizeFaculty(input)` | Tự động chuẩn hóa các viết tắt (ví dụ: `CNTT` -> `Cong nghe Thong tin`). |

### 4. Lớp Rà soát Lớp & Tính Khóa `ClassValidator`
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `extractCohort(className)` | **Tự động trích xuất Khóa:** Lấy 2 chữ số đầu của lớp để tạo khóa (ví dụ: `22T_DT1` -> `K22`, `21D_DT2` -> `K21`). |
| `isValidClassName(name, errorReason)` | **Bẫy lỗi lớp:** Kiểm tra quy tắc đặt tên lớp (2 số đầu khóa 18-26, độ dài 4-15 ký tự, chữ/số/_). |
| `loadClasses(filePath)` | Đọc danh mục lớp đã đăng ký từ file `data/classes.json`. |
| `isClassRegistered(name)` | Kiểm tra xem lớp học đã có trong danh mục chuẩn của trường chưa. |
| `registerClass(name)` | Tự động lưu lớp mới hợp lệ vào file `data/classes.json`. |

### 5. Các kiểu liệt kê & Hàm chuyển đổi chuỗi (`namespace EnumHelper`)
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
| `getClass(), getFaculty(), getCohort()` | Lấy tên lớp sinh hoạt, khoa và khóa học tự động tính toán. |
| `setClass(className)` | Cập nhật lớp và **tự động tính lại Khóa** thông qua `ClassValidator::extractCohort`. |
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

## 🏢 MODULE 3: CÂY PHÒNG Ở & THIẾT BỊ HỎNG (`include/Room.h` & `src/Room.cpp`)

### 1. Lớp `BrokenDevice` (Quản lý thiết bị hư hỏng trong phòng)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `getDeviceId(), getDeviceName()` | Lấy mã định danh và tên thiết bị báo hỏng (ví dụ: `TB01`, Quạt trần). |
| `getDescription()` | Lấy mô tả chi tiết tình trạng hư hỏng (kêu to, chập cháy, gãy chân,...). |
| `getReportDate()` | Lấy ngày sinh viên hoặc cán bộ ghi nhận báo hỏng. |
| `isFixed() / setFixed(bool)` | Kiểm tra và cập nhật trạng thái sửa chữa thiết bị (`true`: Đã sửa xong). |
| `print()` | In thông tin chi tiết thiết bị hỏng ra màn hình. |
| `toJson() / fromJson()` | Đọc và ghi dữ liệu thiết bị hỏng với JSON. |

### 2. Lớp cơ sở trừu tượng `Room`
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `getId()` | Trả về mã phòng `roomId` (ví dụ: `P101`, `P201`). |
| `getBasePrice() / setBasePrice()` | Quản lý giá thuê phòng cơ bản (được đưa lên lớp cha `Room`). |
| `getEquipmentList()` *(thuần ảo)* | **Đa hình:** Trả về danh mục trang thiết bị mặc định của loại phòng. |
| `addRent(Rent*)` | Thêm hợp đồng sinh viên vào phòng; tự động đổi trạng thái sang `OCCUPIED` hoặc `FULL`. |
| `removeRent(Rent*)` | Gỡ hợp đồng khi sinh viên chuyển/trả phòng; tự động đổi trạng thái về `EMPTY` hoặc `OCCUPIED`. |
| `isEmpty() / isFull()` | Kiểm tra phòng có đang trống hoàn toàn hay đã đủ người. |
| `addBrokenDevice(BrokenDevice)` | Báo hỏng và thêm thiết bị hỏng vào danh sách `brokenDevices` của phòng. |
| `fixBrokenDevice(id)` | Đánh dấu thiết bị đã được sửa chữa hoàn tất. |
| `countUnfixedDevices()` | Đếm số lượng thiết bị chưa được sửa chữa trong phòng. |
| `printBrokenDevices()` | In danh sách toàn bộ thiết bị báo hỏng của phòng. |
| `calculateTotalFee()` *(thuần ảo)* | Phương thức đa hình tính tổng tiền phòng (được lớp con ghi đè). |

### 3. Lớp `StandardRoom` (Phòng Tiêu chuẩn) & `VipRoom` (Phòng VIP)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `StandardRoom::calculateTotalFee()` | **Đa hình:** $\text{Giá gốc (400k)} + (\text{Điện} \times 3.000) + (\text{Nước} \times 10.000)$. |
| `VipRoom::calculateTotalFee()` | **Đa hình:** $\text{Giá gốc (1.2tr)} + \text{Phí VIP (250k)} + (\text{Điện} \times 3.000) + (\text{Nước} \times 10.000)$. |
| `VipRoom::getServiceFee()` | Lấy mức phí dịch vụ VIP riêng biệt (250.000 VNĐ). |

---

## 📜 MODULE 4: HỢP ĐỒNG THUÊ KTX (`include/Rent.h` & `src/Rent.cpp`)

| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `getId()` | Trả về mã hợp đồng `rentId` (ví dụ: `HD001`). |
| `getExpectedEndDate()` | Tính ngày kết thúc dự kiến = Ngày bắt đầu + Số tháng thuê (`startDate.addMonths()`). |
| `terminateEarly(actualDate)` | Chấm dứt hợp đồng sớm trước hạn, đổi trạng thái sang `TERMINATED`. |
| `isActive()` | Kiểm tra hợp đồng có đang hiệu lực không (`status == RentStatus::ACTIVE`). |
| `calculateTotalAmount()` | Tính tổng tiền thuê cả kỳ = `rentalPeriod * rentFee`. |

---

## 📊 MODULE 5: IN DỮ LIỆU DẠNG BẢNG ASCII (`include/TablePrinter.h` & `src/TablePrinter.cpp`)

| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `printStudents(list)` | In danh sách sinh viên dạng bảng kẻ khung ASCII trực quan, rõ ràng. |
| `printRooms(list)` | In danh sách phòng ở (sức chứa, số người ở, giá, tình trạng thiết bị hỏng). |
| `printRents(list)` | In danh sách hợp đồng thuê phòng (ngày bắt đầu, ngày hết hạn, tiền phòng). |
| `printStaffs(list)` | In danh sách cán bộ quản lý KTX (phòng ban, chức vụ, SĐT). |
| `printAccounts(list)` | In danh mục tài khoản người dùng và vai trò phân quyền. |
| `printRoomAllocationMap(list)` | In **Sơ đồ phân bổ phòng KTX** hiển thị tên các sinh viên đang ở trong từng phòng. |
| `printBrokenDevices(rId, list)` | In bảng chi tiết các thiết bị báo hỏng và trạng thái xử lý trong phòng. |
| `printRevenueTable(rows, m/y)` | In **Bảng tổng hợp doanh thu toàn KTX theo tháng** (tiền phòng, dịch vụ, điện, nước). |
| `printSingleRoomRevenue(row, m/y)` | In **Phiếu chi tiết tính doanh thu của từng phòng cụ thể** trong tháng. |

---

## 🔐 MODULE 6: TÀI KHOẢN NGƯỜI DÙNG (`include/Account.h` & `src/Account.cpp`)

| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `getId()` | Trả về mã tài khoản `accountId` (ví dụ: `ACC001`). |
| `hashPassword(password)` | Thuật toán băm mật khẩu DJB2 sang chuỗi Hex để lưu trữ an toàn. |
| `verifyPassword(password)` | Kiểm tra mật khẩu người dùng nhập vào có khớp với mã băm hay không. |
| `changePassword(newPass)` | Đổi mật khẩu mới và tự động băm lại mật khẩu. |
| `getRole()` | Lấy quyền hạn của tài khoản (`STUDENT`, `STAFF`, hoặc `ADMIN`). |

---

## ⚙️ MODULE 7: TẦNG QUẢN LÝ REPOSITORY (`include/manager/` & `src/manager/`)

### 1. Khuôn mẫu Generic `Manager<T>`
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `add(T* item)` | Thêm đối tượng mới; tự động kiểm tra trùng mã ID. |
| `remove(id)` | Xóa đối tượng: giải phóng vùng nhớ `delete *it` rồi xóa khỏi `vector`. |
| `findById(id)` | Tìm kiếm và trả về con trỏ `T*` của đối tượng có mã ID tương ứng. |
| `getAll()` | Trả về tham chiếu hằng đến danh sách toàn bộ đối tượng `const vector<T*>&`. |
| `clear()` | Giải phóng toàn bộ con trỏ trong vector để chống rò rỉ bộ nhớ (Memory Leak). |

### 2. Các Manager nghiệp vụ chuyên biệt
| Manager | Các hàm nghiệp vụ đặc trưng |
| :--- | :--- |
| **`StudentManager`** | `findByClass()` (lọc theo lớp), `findByFaculty()` (lọc theo khoa). |
| **`RoomManager`** | `findAvailableRoom()` (tìm phòng trống), `printOccupancyReport()` (báo cáo lấp đầy), `printRoomBill()` (phiếu báo tiền phòng điện nước chia theo người), `calculateRoomMonthlyRevenue()` (tính doanh thu 1 phòng cụ thể trong tháng), `calculateAllRoomsMonthlyRevenue()` (tính doanh thu toàn bộ phòng), `printMonthlyRevenueReport()` (in bảng doanh thu toàn KTX theo tháng), `printSpecificRoomRevenue()` (in phiếu chi tiết doanh thu 1 phòng). |
| **`RentManager`** | `createRent()` (xếp phòng an toàn, kiểm tra giới tính/sức chứa), `terminateRent()` (thanh lý hợp đồng), `extendRent()` (gia hạn thêm tháng), `transferRoom()` (chuyển phòng cho sinh viên), `findExpiringRents()` (cảnh báo 30 ngày hết hạn), `checkAndExpireRents()` (tự động giải phóng phòng quá hạn), `calculateTotalMonthlyRevenue()`. |
| **`StaffManager`** | `findByDepartment()` (lọc cán bộ theo phòng ban). |
| **`AccountManager`** | `authenticate()` (xác thực đăng nhập), `findByUsername()`, `createAccount()` (cấp tài khoản mới). |

---

## 🎮 MODULE 8: ĐIỀU PHỐI MẶT TIỀN FACADE (`include/manager/DormManager.h` & `src/manager/DormManager.cpp`)

| Nhóm hàm | Tên hàm | Chức năng chi tiết |
| :--- | :--- | :--- |
| **Hệ thống** | `loadAll() / saveAll()` | Nạp và lưu đồng bộ dữ liệu của tất cả 5 Manager xuống JSON. |
| | `login() / logout()` | Đăng nhập phân quyền và đăng xuất an toàn. |
| | `runMainMenu()` | Vòng lặp điều hướng theo 3 quyền hạn (`ADMIN`, `STAFF`, `STUDENT`). |
| **Tách biệt nghiệp vụ** | `handleAllocationManagement()` | **Tách riêng Phân bổ & Xếp chỗ:** Sơ đồ phân bổ phòng, xếp phòng mới, chuyển phòng, trả phòng. |
| | `handleContractManagement()` | **Tách riêng Quản lý Hợp đồng:** Xem bảng HĐ, tìm kiếm theo mã SV/phòng/HĐ, gia hạn HĐ, cảnh báo hết hạn, giải phóng HĐ quá hạn. |
| | `handleRevenueManagement()` | **Quản lý Doanh thu từng tháng:** Bảng doanh thu toàn KTX, tính doanh thu chi tiết từng phòng cụ thể, xuất hóa đơn điện nước. |
| | `handleRoomManagement()` | Quản lý danh sách phòng dạng bảng, lọc tòa/loại phòng, quản lý báo hỏng và sửa chữa thiết bị. |
| | `handleStudentManagement()` | Quản lý danh sách sinh viên dạng bảng, lọc lớp/khoa, thêm SV mới bẫy lỗi trực tiếp. |
| **Giao diện & Bẫy lỗi** | `clearScreen()` | Xóa màn hình console (`cls`/ANSI) mỗi khi chuyển menu hoặc sang lựa chọn mới. |
| | `inputGender(prompt)` | Bẫy lỗi `try-catch` chuyển đổi giới tính (`Nam`/`Nu`/`1`/`2`), nếu sai ném `invalid_argument`, báo lỗi và yêu cầu nhập lại, không ngầm định. |
| | `inputRole(prompt)` | Bẫy lỗi `try-catch` chuyển đổi vai trò (`STUDENT`/`STAFF`/`ADMIN`), nếu sai báo lỗi và yêu cầu nhập lại, không ngầm định. |
| | `inputPeriod(prompt)` | Bẫy lỗi `try-catch` kỳ hạn thuê (chỉ chấp nhận `1, 3, 6, 12` tháng), nếu sai ném `invalid_argument` và yêu cầu nhập lại. |
| | `inputRoomStatus(prompt)`| Bẫy lỗi `try-catch` trạng thái phòng (`EMPTY`, `OCCUPIED`, `FULL`, `MAINTENANCE`), báo lỗi và yêu cầu nhập lại. |
| | `inputValidDate(prompt)` | Bắt buộc nhập đúng định dạng `dd/mm/yyyy`, kiểm tra ngày hợp lệ (1-31, 1-12, năm nhuận). |
| | `inputValidPhone(prompt)` | Bắt buộc nhập đúng 10 chữ số và bắt đầu bằng số `0`. |
| | `inputValidEmail(prompt)` | Kiểm tra email bắt buộc chứa `@` và tên miền hợp lệ. |
| | `inputValidClassName(prompt)` | Bẫy lỗi 2 số đầu là khóa (18-26), tự động đăng ký vào `classes.json`. |
| | `inputFacultyChoice()` | Hiển thị menu đóng khung 1..10 khoa chuẩn đồng bộ, yêu cầu nhập số hợp lệ. |
| | `inputNonEmpty(prompt)` | Không cho phép bỏ trống dữ liệu bắt buộc. |

---

## 🚀 ĐIỂM KHỞI CHẠY CHƯƠNG TRÌNH (`main.cpp`)

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
