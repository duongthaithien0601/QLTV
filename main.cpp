#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "cautruc.h"
#include "dsdms.h"
#include "dsdocgia.h"
#include "dsdausach.h"
#include "dsmuontra.h"
#include "luutru.h"
#include "menu_tui.h"

int main() {
    // 1. Tối ưu hóa tốc độ nhập/xuất (I/O)
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    // 2. Khởi tạo cấu trúc dữ liệu chính
    DanhSachDauSach DanhSachDauSach;
    DocGiaNode* Root = NULL;
    // 3. Nạp dữ liệu từ File
    if (!DocToanBoDuLieu(DanhSachDauSach, Root)) {
        std::cout << "Tai du lieu that bai. Chuong trinh se khoi dong voi CSDL rong.\n";
        DanhSachDauSach.SoLuong = 0;
        Root = NULL;
    }
    // 4. Chạy giao diện Menu chính
    menutui::MenuChinhTUI(DanhSachDauSach, Root);
    // 5. Lưu dữ liệu trước khi thoát
    if (LuuToanBoDuLieu(DanhSachDauSach, Root)) {
        std::cout << "Da luu du lieu. Tam biet!\n";
    }
    else {
        std::cout << "Luu du lieu that bai!\n";
    }
    // 6. Dọn dẹp bộ nhớ (Clean up)
    GiaiPhongDanhSachDauSach(DanhSachDauSach);
    GiaiPhongCayDocGia(Root);
    return 0;
}