#include <iostream>
#include "cautruc.h"
#include "dausach.h"
#include "docgia.h"
#include "muontra.h"
#include "thongke.h"
#include "luutru.h"
#include "giaodien.h"

int main() {
    // Tối ưu tốc độ nhập xuất
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    // Khởi tạo dữ liệu chính của chương trình
    DanhSachDauSach DanhSachDauSach;
    DocGiaNode* Root = NULL;
    // Đọc dữ liệu đã lưu trước khi mở menu
    if (!DocToanBoDuLieu(DanhSachDauSach, Root)) {
        std::cout << "Tai du lieu that bai. Chuong trinh se khoi dong voi CSDL rong.\n";
        DanhSachDauSach.SoLuong = 0;
        Root = NULL;
    }
    // Chạy giao diện chính
    giaodien::MenuChinhTUI(DanhSachDauSach, Root);
    // Lưu dữ liệu trước khi kết thúc chương trình
    if (LuuToanBoDuLieu(DanhSachDauSach, Root)) {
        std::cout << "Da luu du lieu. Tam biet!\n";
    }
    else {
        std::cout << "Luu du lieu that bai!\n";
    }
    // Giải phóng toàn bộ vùng nhớ động
    GiaiPhongDanhSachDauSach(DanhSachDauSach);
    GiaiPhongCayDocGia(Root);
    return 0;
}