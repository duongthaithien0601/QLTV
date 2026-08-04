#pragma once
#include "cautruc.h"
#include "dausach.h"

// ======================= DỮ LIỆU THỐNG KÊ =======================
struct ThongKeQuaHan {
    DocGia* DuLieuDocGia;
    MuonTraNode* PhieuMuon;
    const DauSach* DuLieuSach;
    int SoNgayTre;
};

// ======================= TOP 10 SÁCH MƯỢN NHIỀU NHẤT =======================
// Chèn địa chỉ một đầu sách vào danh sách Top 10 theo số lượt mượn giảm dần
inline void ChenDauSachVaoTop10(DauSach* DanhSachKetQua[], int& SoLuongKetQua, DauSach* DauSachCanChen) {
    int ViTriChen = 0;
    while (ViTriChen < SoLuongKetQua && DanhSachKetQua[ViTriChen]->SoLuotMuon >= DauSachCanChen->SoLuotMuon){
        ViTriChen++;
    }
    if (ViTriChen >= 10){
        return;
    }
    int ViTriCuoi = SoLuongKetQua;
    if (ViTriCuoi >= 10){
        ViTriCuoi = 9;
    }
    for (int i = ViTriCuoi; i > ViTriChen; i--){
        DanhSachKetQua[i] = DanhSachKetQua[i - 1];
    }
    DanhSachKetQua[ViTriChen] = DauSachCanChen;
    if (SoLuongKetQua < 10) {
        SoLuongKetQua++;
    }
}
// Lập Top 10 bằng cách duy trì trực tiếp một mảng tối đa 10 địa chỉ đầu sách
inline void ThongKeTop10TheoLuotMuon(const DanhSachDauSach& DanhSachDauSach, DauSach* DanhSachKetQua[], int& SoLuongKetQua) {
    SoLuongKetQua = 0;
    for (int i = 0; i < DanhSachDauSach.SoLuong; i++) {
        DauSach* DuLieuSach = DanhSachDauSach.Nodes[i];
        if (DuLieuSach->SoLuotMuon > 0) {
            ChenDauSachVaoTop10(DanhSachKetQua, SoLuongKetQua, DuLieuSach);
        }
    }
}

// =================== THỐNG KÊ ĐỘC GIẢ QUÁ HẠN ===================
// Đếm chính xác số phiếu đang mượn đã quá hạn tại ngày kiểm tra
inline int DemSoPhieuQuaHan(DocGiaNode* Root, const NgayThangNam& NgayKiemTra) {
    if (Root == NULL) {
        return 0;
    }
    int SoLuongDem = DemSoPhieuQuaHan(Root->Left, NgayKiemTra) + DemSoPhieuQuaHan(Root->Right, NgayKiemTra);
    for (
        const MuonTraNode* ConTroHienTai = Root->ThongTin.MuonTraHead;
        ConTroHienTai != NULL;
        ConTroHienTai = ConTroHienTai->Next
        ){
        if (ConTroHienTai->TrangThai == 0 && TinhSoNgayTre(ConTroHienTai->NgayMuon, NgayKiemTra) > 0){
            SoLuongDem++;
        }
    }
    return SoLuongDem;
}
// Chèn một dòng quá hạn vào mảng theo số ngày trễ giảm dần
inline void ChenThongKeQuaHanTheoThuTu(
    ThongKeQuaHan DanhSachQuaHan[],
    int& SoLuongKetQua,
    int SoPhanTuToiDa,
    DocGia* DuLieuDocGia,
    MuonTraNode* PhieuMuon,
    const DauSach* DuLieuSach,
    int SoNgayTre
) {
    if (SoLuongKetQua >= SoPhanTuToiDa) {
        return;
    }
    ThongKeQuaHan DongCanChen;
    DongCanChen.DuLieuDocGia = DuLieuDocGia;
    DongCanChen.PhieuMuon = PhieuMuon;
    DongCanChen.DuLieuSach = DuLieuSach;
    DongCanChen.SoNgayTre = SoNgayTre;
    int ViTriChen = SoLuongKetQua;
    while (ViTriChen > 0 && DongCanChen.SoNgayTre > DanhSachQuaHan[ViTriChen - 1].SoNgayTre) {
        DanhSachQuaHan[ViTriChen] = DanhSachQuaHan[ViTriChen - 1];
        ViTriChen--;
    }
    DanhSachQuaHan[ViTriChen] = DongCanChen;
    SoLuongKetQua++;
}
// Duyệt cây và chèn các phiếu quá hạn vào mảng kết quả
inline void DuyetCayThongKeQuaHan(
    DocGiaNode* Root,
    const DanhSachDauSach& DanhSachDauSach,
    const NgayThangNam& NgayKiemTra,
    ThongKeQuaHan DanhSachQuaHan[],
    int& SoLuongKetQua,
    int SoPhanTuToiDa
) {
    if (Root == NULL || SoLuongKetQua >= SoPhanTuToiDa) {
        return;
    }
    DuyetCayThongKeQuaHan(Root->Left, DanhSachDauSach, NgayKiemTra, DanhSachQuaHan, SoLuongKetQua, SoPhanTuToiDa);
    DocGia* DuLieuDocGia = &Root->ThongTin;
    for (
        MuonTraNode* PhieuMuon = DuLieuDocGia->MuonTraHead;
        PhieuMuon != NULL;
        PhieuMuon = PhieuMuon->Next
        ) {
        // Chỉ xét các phiếu đang mượn
        if (PhieuMuon->TrangThai != 0) {
            continue;
        }
        int SoNgayTre = TinhSoNgayTre(PhieuMuon->NgayMuon, NgayKiemTra);
        if (SoNgayTre <= 0) {
            continue;
        }
        char ISBNCanXuLy[15];
        LayISBNTuMaSach(PhieuMuon->MaSach, ISBNCanXuLy, 15);
        const DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy);
        ChenThongKeQuaHanTheoThuTu(DanhSachQuaHan, SoLuongKetQua, SoPhanTuToiDa, DuLieuDocGia, PhieuMuon, DuLieuSach, SoNgayTre);
    }
    DuyetCayThongKeQuaHan(Root->Right, DanhSachDauSach, NgayKiemTra, DanhSachQuaHan, SoLuongKetQua, SoPhanTuToiDa);
}
// Lập danh sách quá hạn theo số ngày trễ giảm dần bằng thuật toán chèn có thứ tự
inline void LapDanhSachQuaHan(
    DocGiaNode* Root,
    const DanhSachDauSach& DanhSachDauSach,
    const NgayThangNam& NgayKiemTra,
    ThongKeQuaHan DanhSachQuaHan[],
    int& SoLuongKetQua,
    int SoPhanTuToiDa
) {
    SoLuongKetQua = 0;
    DuyetCayThongKeQuaHan(Root, DanhSachDauSach, NgayKiemTra, DanhSachQuaHan, SoLuongKetQua, SoPhanTuToiDa);
}