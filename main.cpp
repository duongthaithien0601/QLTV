#include <iostream>
#include "cautruc.h"
#include "dausach.h"
#include "docgia.h"
#include "muontra.h"
#include "thongke.h"
#include "luutru.h"
#include "giaodien.h"



int main() {
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
    bool LuuThanhCong = LuuToanBoDuLieu(DanhSachDauSach, Root);
    std::string ThongBaoLuu;
    if (LuuThanhCong) {
        ThongBaoLuu = "Da luu du lieu. Tam biet!";
    }
    else {
        ThongBaoLuu = "Luu du lieu that bai!";
    }
    const int XKhung = 2;
    const int ChieuRongKhung = 118;
    const int YChanTrang = 12;
    int XThongBao = XKhung + ChieuRongKhung - 2 - static_cast<int>(ThongBaoLuu.length());
    tui::DiChuyenConTro(XThongBao, YChanTrang);
    if (LuuThanhCong) {
        tui::DatMau(tui::MauThanhCong);
    }
    else {
        tui::DatMau(tui::MauCanhBao);
    }
    std::cout << ThongBaoLuu;
    tui::DatLaiMau();
    std::cout.flush();
    // Giải phóng toàn bộ vùng nhớ động
    GiaiPhongDanhSachDauSach(DanhSachDauSach);
    GiaiPhongCayDocGia(Root);
    // Đưa con trỏ xuống dưới khung
    tui::DiChuyenConTro(1, 14);
    return 0;
}