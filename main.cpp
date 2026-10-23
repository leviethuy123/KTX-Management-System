#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include "DormManager.h"

int main() {
#ifdef _WIN32
    // Thiết lập bảng mã UTF-8 cho Windows Console để hiển thị tiếng Việt có dấu
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    // Khởi chạy hệ thống điều phối trung tâm Ký túc xá (Facade Pattern)
    DormManager app;
    app.runMainMenu();

    return 0;
}
