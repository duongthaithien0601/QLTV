#pragma once
#include "cautruc.h"
#include "dausach.h"
#include "docgia.h"

// ======================= DỮ LIỆU THỐNG KÊ =======================
struct ThongKeQuaHan {
    DocGia* DuLieuDocGia;
    MuonTraNode* PhieuMuon;
    const DauSach* DuLieuSach;
    int SoNgayTre;

    ThongKeQuaHan() {
        DuLieuDocGia = NULL;
        PhieuMuon = NULL;
        DuLieuSach = NULL;
        SoNgayTre = 0;
    }
};

// ======================= TOP 10 SÁCH MƯỢN NHIỀU NHẤT =======================
// Kiểm tra đầu sách thứ nhất có đứng trước đầu sách thứ hai trong danh sách Top 10 hay không
inline bool DauSachDungTruocTrongTop10(const DauSach* DauSachThuNhat, const DauSach* DauSachThuHai){
    if (DauSachThuNhat->SoLuotMuon > DauSachThuHai->SoLuotMuon){
        return true;
    }
    if (DauSachThuNhat->SoLuotMuon < DauSachThuHai->SoLuotMuon){
        return false;
    }
    int KetQuaSoSanhTen = std::strcmp(DauSachThuNhat->TenSach, DauSachThuHai->TenSach);
    if (KetQuaSoSanhTen < 0) {
        return true;
    }
    if (KetQuaSoSanhTen > 0) {
        return false;
    }
    return std::strcmp(DauSachThuNhat->ISBN, DauSachThuHai->ISBN) < 0;
}
// Chèn địa chỉ một đầu sách vào danh sách Top 10 và chỉ giữ lại tối đa 10 phần tử
inline void ChenDauSachVaoTop10(DauSach* DanhSachKetQua[], int& SoLuongKetQua, DauSach* DauSachCanChen){
    if (DanhSachKetQua == NULL || DauSachCanChen == NULL || DauSachCanChen->SoLuotMuon <= 0){
        return;
    }
    int ViTriChen = 0;
    while (ViTriChen < SoLuongKetQua && !DauSachDungTruocTrongTop10(DauSachCanChen, DanhSachKetQua[ViTriChen])){
        ViTriChen++;
    }
    // Nếu mảng đã đủ 10 phần tử và đầu sách mới (không nằm trong Top 10 thì không chèn)
    if (ViTriChen >= 10) {
        return;
    }
    int ViTriCuoi = SoLuongKetQua;
    if (ViTriCuoi >= 10) {
        ViTriCuoi = 9;
    }
    for (int i = ViTriCuoi; i > ViTriChen; i--) {
        DanhSachKetQua[i] = DanhSachKetQua[i - 1];
    }
    DanhSachKetQua[ViTriChen] = DauSachCanChen;
    if (SoLuongKetQua < 10) {
        SoLuongKetQua++;
    }
}
// Lập Top 10 bằng cách duy trì trực tiếp một mảng tối đa 10 địa chỉ đầu sách
inline void ThongKeTop10TheoLuotMuon(const DanhSachDauSach& DanhSachDauSach, DauSach* DanhSachKetQua[], int& SoLuongKetQua){
    SoLuongKetQua = 0;
    for (int i = 0; i < DanhSachDauSach.SoLuong;  i++){
        DauSach* DuLieuSach = DanhSachDauSach.Nodes[i];
        if (DuLieuSach != NULL && DuLieuSach->SoLuotMuon > 0){
            ChenDauSachVaoTop10(DanhSachKetQua, SoLuongKetQua, DuLieuSach);
        }
    }
}

// =================== THỐNG KÊ ĐỘC GIẢ QUÁ HẠN ===================
// Đếm chính xác số phiếu đang mượn đã quá hạn
inline int DemSoPhieuQuaHan(DocGiaNode* Root, const NgayThangNam& NgayHienTai){
    if (Root == NULL) {
        return 0;
    }
    int SoLuongDem = DemSoPhieuQuaHan(Root->Left, NgayHienTai) + DemSoPhieuQuaHan(Root->Right, NgayHienTai);
    for (MuonTraNode* ConTroHienTai = Root->ThongTin.MuonTraHead; ConTroHienTai != NULL; ConTroHienTai = ConTroHienTai->Next){
        if (ConTroHienTai->TrangThai == 0 && TinhSoNgayTre(ConTroHienTai->NgayMuon, NgayHienTai) > 0){
            SoLuongDem++;
        }
    }
    return SoLuongDem;
}
// Kiểm tra dòng quá hạn thứ nhất có đứng trước dòng quá hạn thứ hai hay không
inline bool ThongKeQuaHanDungTruoc(const ThongKeQuaHan& DongThuNhat, const ThongKeQuaHan& DongThuHai){
    // Ưu tiên số ngày trễ giảm dần
    if (DongThuNhat.SoNgayTre > DongThuHai.SoNgayTre){
        return true;
    }
    if (DongThuNhat.SoNgayTre < DongThuHai.SoNgayTre){
        return false;
    }
    // Nếu cùng số ngày trễ thì mã thẻ tăng dần
    if (
        DongThuNhat.DuLieuDocGia->MaThe < DongThuHai.DuLieuDocGia->MaThe){
        return true;
    }
    if (DongThuNhat.DuLieuDocGia->MaThe > DongThuHai.DuLieuDocGia->MaThe){
        return false;
    }
    // Nếu cùng mã thẻ thì mã sách tăng dần
    return std::strcmp(DongThuNhat.PhieuMuon->MaSach, DongThuHai.PhieuMuon->MaSach) < 0;
}
// Chèn một dòng quá hạn vào đúng vị trí trong mảng kết quả
inline void ChenThongKeQuaHanTheoThuTu(
    ThongKeQuaHan DanhSachQuaHan[],
    int& SoLuongKetQua,
    int SoPhanTuToiDa,
    DocGia* DuLieuDocGia,
    MuonTraNode* PhieuMuon,
    const DauSach* DuLieuSach,
    int SoNgayTre
) {
    if (DanhSachQuaHan == NULL || DuLieuDocGia == NULL || PhieuMuon == NULL || SoLuongKetQua >= SoPhanTuToiDa){
        return;
    }
    ThongKeQuaHan DongCanChen;
    DongCanChen.DuLieuDocGia = DuLieuDocGia;
    DongCanChen.PhieuMuon = PhieuMuon;
    DongCanChen.DuLieuSach = DuLieuSach;
    DongCanChen.SoNgayTre = SoNgayTre;
    int ViTriChen = SoLuongKetQua;
    while (ViTriChen > 0 && ThongKeQuaHanDungTruoc(DongCanChen, DanhSachQuaHan[ViTriChen - 1])){
        DanhSachQuaHan[ViTriChen] =  DanhSachQuaHan[ViTriChen - 1];
        ViTriChen--;
    }
    DanhSachQuaHan[ViTriChen] = DongCanChen;
    SoLuongKetQua++;
}
// Duyệt cây và chèn các phiếu quá hạn vào mảng kết quả
inline void DuyetCayThongKeQuaHan(
    DocGiaNode* Root,
    const DanhSachDauSach& DanhSachDauSach,
    const NgayThangNam& NgayHienTai,
    ThongKeQuaHan DanhSachQuaHan[],
    int& SoLuongKetQua,
    int SoPhanTuToiDa
) {
    if (Root == NULL || SoLuongKetQua >= SoPhanTuToiDa){
        return;
    }
    DuyetCayThongKeQuaHan(Root->Left, DanhSachDauSach, NgayHienTai, DanhSachQuaHan, SoLuongKetQua, SoPhanTuToiDa);
    DocGia* DuLieuDocGia = &Root->ThongTin;
    for (MuonTraNode* PhieuMuon = DuLieuDocGia->MuonTraHead; PhieuMuon != NULL; PhieuMuon = PhieuMuon->Next){
        // Chỉ xét phiếu đang mượn
        if (PhieuMuon->TrangThai != 0) {
            continue;
        }
        int SoNgayTre = TinhSoNgayTre(PhieuMuon->NgayMuon, NgayHienTai);
        if (SoNgayTre <= 0) {
            continue;
        }
        char ISBNCanXuLy[15];
        LayISBNTuMaSach(PhieuMuon->MaSach, ISBNCanXuLy,15);
        const DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy);
        ChenThongKeQuaHanTheoThuTu(DanhSachQuaHan, SoLuongKetQua, SoPhanTuToiDa, DuLieuDocGia, PhieuMuon, DuLieuSach, SoNgayTre);
    }
    DuyetCayThongKeQuaHan(Root->Right, DanhSachDauSach, NgayHienTai, DanhSachQuaHan, SoLuongKetQua, SoPhanTuToiDa);
}
// Lập danh sách quá hạn theo số ngày trễ giảm dần bằng thuật toán chèn có thứ tự
inline void LapDanhSachQuaHan(
    DocGiaNode* Root,
    const DanhSachDauSach& DanhSachDauSach,
    const NgayThangNam& NgayHienTai,
    ThongKeQuaHan DanhSachQuaHan[],
    int& SoLuongKetQua,
    int SoPhanTuToiDa
) {
    SoLuongKetQua = 0;

    if (DanhSachQuaHan == NULL || SoPhanTuToiDa <= 0){
        return;
    }
    DuyetCayThongKeQuaHan(Root, DanhSachDauSach, NgayHienTai, DanhSachQuaHan, SoLuongKetQua, SoPhanTuToiDa);
}