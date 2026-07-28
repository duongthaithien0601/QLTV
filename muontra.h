#pragma once
#include <string>
#include "cautruc.h"
#include "dausach.h"
#include "docgia.h"

// ======================= DỮ LIỆU KẾT QUẢ =======================
struct ThongTinSachDangMuon {
    MuonTraNode* PhieuMuon;
    const DauSach* DuLieuSach;
    int TongSoNgay;
};

// ===================== TIỆN ÍCH CƠ BẢN & HỖ TRỢ =========================
// Lấy tên sách tương ứng với ISBN
inline const char* LayTenSachTheoISBN(const DanhSachDauSach& DanhSachDauSach, const char ISBNCanXuLy[]) {
    const DauSach* ConTroHienTai = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy);
    if (ConTroHienTai == NULL) {
        return "";
    }
    return ConTroHienTai->TenSach;
}
// Tạo phiếu mượn trả mới và thêm vào đầu danh sách của độc giả
inline void ThemPhieuMuonTraChoDocGia(
    DocGia& DocGiaCanXuLy,
    const char MaSach[],
    const NgayThangNam& NgayMuon,
    const NgayThangNam& NgayTra,
    int TrangThai
) {
    MuonTraNode* NodeMoi = new MuonTraNode();
    SaoChepChuoi(NodeMoi->MaSach, MaxMaSach, MaSach);
    NodeMoi->NgayMuon = NgayMuon;
    NodeMoi->NgayTra = NgayTra;
    NodeMoi->TrangThai = TrangThai;
    NodeMoi->Next = DocGiaCanXuLy.MuonTraHead;
    DocGiaCanXuLy.MuonTraHead = NodeMoi;
}

// ========== KIỂM TRA TÌNH TRẠNG SÁCH TRÊN TOÀN HỆ THỐNG ====================
// Lấy địa chỉ các phiếu đang mượn của một độc giả vào mảng tối đa 3 phần tử
inline void LayDanhSachPhieuDangMuon(const DocGia& DocGiaCanXuLy, MuonTraNode* DanhSachKetQua[], int& SoLuongKetQua) {
    SoLuongKetQua = 0;
    for (
        MuonTraNode* ConTroHienTai = DocGiaCanXuLy.MuonTraHead;
        ConTroHienTai != NULL && SoLuongKetQua < 3;
        ConTroHienTai = ConTroHienTai->Next
        ) {
        if (ConTroHienTai->TrangThai == 0) {
            DanhSachKetQua[SoLuongKetQua] = ConTroHienTai;
            SoLuongKetQua++;
        }
    }
}
// Tìm phiếu đang mượn theo mã sách
inline MuonTraNode* TimPhieuDangMuonTheoMaSach(DocGia& DocGiaCanXuLy, const char MaSachCanTim[]) {
    if (MaSachCanTim == NULL) {
        return NULL;
    }
    for (
        MuonTraNode* ConTroHienTai = DocGiaCanXuLy.MuonTraHead;
        ConTroHienTai != NULL;
        ConTroHienTai = ConTroHienTai->Next
        ) {
        if (ConTroHienTai->TrangThai == 0 && std::strcmp(ConTroHienTai->MaSach, MaSachCanTim) == 0) {
            return ConTroHienTai;
        }
    }
    return NULL;
}
// ===================== KIỂM TRA QUÁ HẠN =======================
inline bool KiemTraDocGiaQuaHanDenNgay(const DocGia& DocGiaCanXuLy, const NgayThangNam& NgayKiemTra, int* KetQuaSoNgayTreLonNhat = NULL) {
    int SoNgayTreLonNhat = 0;
    bool CoSachQuaHan = false;
    for (
        const MuonTraNode* ConTroHienTai = DocGiaCanXuLy.MuonTraHead;
        ConTroHienTai != NULL;
        ConTroHienTai = ConTroHienTai->Next
        ) {
        if (ConTroHienTai->TrangThai == 0) {
            int SoNgayTre = TinhSoNgayTre(ConTroHienTai->NgayMuon, NgayKiemTra);
            if (SoNgayTre > 0) {
                CoSachQuaHan = true;
                if (SoNgayTre > SoNgayTreLonNhat) {
                    SoNgayTreLonNhat = SoNgayTre;
                }
            }
        }
    }
    if (KetQuaSoNgayTreLonNhat != NULL) {
        *KetQuaSoNgayTreLonNhat = SoNgayTreLonNhat;
    }
    return CoSachQuaHan;
}

// =================== MƯỢN / TRẢ ==================
// Kiểm tra điều kiện và thực hiện cho độc giả mượn một bản sao
inline bool MuonSach(
    DocGia& DocGiaCanXuLy,
    DauSach& DuLieuSach,
    const NgayThangNam& NgayMuon,
    std::string* MaSachDaMuon = NULL
) {
    if (!KiemTraNgayKhongVuotQuaHienTai(NgayMuon)) {
        return false;
    }
    if (DocGiaCanXuLy.TrangThaiThe != 1) {
        return false;
    }
    if (DemSoSachDocGiaDangMuon(DocGiaCanXuLy) >= 3) {
        return false;
    }
    int SoNgayTreLonNhat = 0;
    if (KiemTraDocGiaQuaHanDenNgay(DocGiaCanXuLy, NgayMuon, &SoNgayTreLonNhat)) {
        return false;
    }
    DanhMucSachNode* BanSaoCoTheMuon = TimSachCoTheMuonDauTien(&DuLieuSach);
    if (BanSaoCoTheMuon == NULL) {
        return false;
    }
    DanhDauSachDaMuon(BanSaoCoTheMuon);
    NgayThangNam NgayTra{ 0, 0, 0 };
    ThemPhieuMuonTraChoDocGia(DocGiaCanXuLy, BanSaoCoTheMuon->MaSach, NgayMuon, NgayTra, 0);
    DuLieuSach.SoLuotMuon++;
    if (MaSachDaMuon != NULL) {
        *MaSachDaMuon = BanSaoCoTheMuon->MaSach;
    }
    return true;
}
// Kiểm tra điều kiện và thực hiện trả một bản sao
inline bool TraSach(
    DanhSachDauSach& DanhSachDauSach,
    MuonTraNode* DoiTuongCanXuLy,
    const NgayThangNam& NgayTra,
    int* TongSoNgayKetQua = NULL,
    int* SoNgayTreKetQua = NULL
) {
    if (!KiemTraNgayKhongVuotQuaHienTai(NgayTra)) {
        return false;
    }
    if (SoSanhNgay(NgayTra, DoiTuongCanXuLy->NgayMuon) < 0) {
        return false;
    }
    char ISBNCanXuLy[15];
    LayISBNTuMaSach(DoiTuongCanXuLy->MaSach, ISBNCanXuLy, 15);
    DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy);
    if (DuLieuSach == NULL) {
        return false;
    }
    DanhMucSachNode* BanSaoSach = TimSachTheoMaSach(DuLieuSach, DoiTuongCanXuLy->MaSach);
    if (BanSaoSach == NULL) {
        return false;
    }
    if (!DanhDauSachDaTra(BanSaoSach)) {
        return false;
    }
    DoiTuongCanXuLy->TrangThai = 1;
    DoiTuongCanXuLy->NgayTra = NgayTra;
    int TongSoNgay = TinhSoNgayChenhLech(NgayTra, DoiTuongCanXuLy->NgayMuon);
    int SoNgayTre = TinhSoNgayTre(DoiTuongCanXuLy->NgayMuon, NgayTra);
    if (TongSoNgayKetQua != NULL) {
        *TongSoNgayKetQua = TongSoNgay;
    }
    if (SoNgayTreKetQua != NULL) {
        *SoNgayTreKetQua = SoNgayTre;
    }
    return true;
}
// Lập danh sách tối đa 3 sách đang mượn của độc giả tại ngày kiểm tra
inline bool LapDanhSachSachDangMuonCuaDocGia(
    const DanhSachDauSach& DanhSachDauSach,
    DocGiaNode* Root,
    int MaTheCanXuLy,
    const NgayThangNam& NgayKiemTra,
    ThongTinSachDangMuon DanhSachKetQua[],
    int& SoLuongKetQua,
    DocGia*& DocGiaKetQua
) {
    SoLuongKetQua = 0;
    DocGiaKetQua = NULL;
    if (!KiemTraNgayKhongVuotQuaHienTai(NgayKiemTra)) {
        return false;
    }
    DocGiaNode* NodeDocGia = TimDocGiaTheoMaThe(Root, MaTheCanXuLy);
    if (NodeDocGia == NULL) {
        return false;
    }
    DocGiaKetQua = &NodeDocGia->ThongTin;
    MuonTraNode* DanhSachPhieu[3];
    int SoLuongPhieu = 0;
    LayDanhSachPhieuDangMuon(NodeDocGia->ThongTin, DanhSachPhieu, SoLuongPhieu);
    for (int i = 0; i < SoLuongPhieu; i++) {
        MuonTraNode* PhieuMuon = DanhSachPhieu[i];
        int TongSoNgay = TinhSoNgayChenhLech(NgayKiemTra, PhieuMuon->NgayMuon);
        if (TongSoNgay < 0) {
            continue;
        }
        char ISBNCanXuLy[15];
        LayISBNTuMaSach(PhieuMuon->MaSach, ISBNCanXuLy, 15);
        const DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy);
        ThongTinSachDangMuon& DongKetQua = DanhSachKetQua[SoLuongKetQua];
        DongKetQua.PhieuMuon = PhieuMuon;
        DongKetQua.DuLieuSach = DuLieuSach;
        DongKetQua.TongSoNgay = TongSoNgay;
        SoLuongKetQua++;
    }
    return true;
}