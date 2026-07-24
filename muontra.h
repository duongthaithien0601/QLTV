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

    ThongTinSachDangMuon() {
        PhieuMuon = NULL;
        DuLieuSach = NULL;
        TongSoNgay = 0;
    }
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
    int TrangThai){
    MuonTraNode* NodeCanXuLy = new MuonTraNode();
    SaoChepChuoi(NodeCanXuLy->MaSach, MaxMaSach, MaSach);
    NodeCanXuLy->NgayMuon = NgayMuon;
    NodeCanXuLy->NgayTra = NgayTra;
    NodeCanXuLy->TrangThai = TrangThai;
    NodeCanXuLy->Next = DocGiaCanXuLy.MuonTraHead;
    DocGiaCanXuLy.MuonTraHead = NodeCanXuLy;
}

// ========== KIỂM TRA TÌNH TRẠNG SÁCH TRÊN TOÀN HỆ THỐNG ====================
// Lấy các phiếu đang mượn của một độc giả vào mảng
inline void LayDanhSachPhieuDangMuon(
    const DocGia& DocGiaCanXuLy,
    MuonTraNode* DanhSachKetQua[],
    int& SoLuongKetQua,
    int SoPhanTuToiDa = 100
) {
    SoLuongKetQua = 0;
    if (DanhSachKetQua == NULL || SoPhanTuToiDa <= 0) {
        return;
    }

    for (MuonTraNode* ConTroHienTai = DocGiaCanXuLy.MuonTraHead;
        ConTroHienTai != NULL && SoLuongKetQua < SoPhanTuToiDa;
        ConTroHienTai = ConTroHienTai->Next) {
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
    for (MuonTraNode* ConTroHienTai = DocGiaCanXuLy.MuonTraHead; ConTroHienTai != NULL; ConTroHienTai = ConTroHienTai->Next) {
        if (ConTroHienTai->TrangThai == 0 && std::strcmp(ConTroHienTai->MaSach, MaSachCanTim) == 0) {
            return ConTroHienTai;
        }
    }
    return NULL;
}

// ===================== KIỂM TRA QUÁ HẠN =======================
inline bool KiemTraDocGiaQuaHanDenNgay(const DocGia& DocGiaCanXuLy,
    const NgayThangNam& NgayHienTai,
    int* KetQuaSoNgayTreLonNhat = NULL) {
    int SoNgayTreLonNhat = 0;
    bool CoSachQuaHan = false;
    for (MuonTraNode* ConTroHienTai = DocGiaCanXuLy.MuonTraHead; ConTroHienTai != NULL;
        ConTroHienTai = ConTroHienTai->Next) {
        if (ConTroHienTai->TrangThai == 0) {
            int Tre = TinhSoNgayTre(ConTroHienTai->NgayMuon, NgayHienTai);
            if (Tre > 0) {
                CoSachQuaHan = true;
                if (Tre > SoNgayTreLonNhat) {
                    SoNgayTreLonNhat = Tre;
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
    std::string* MaSachDaMuon = NULL,
    std::string* ThongBaoLoi = NULL)
{
    if (!KiemTraNgayHopLe(NgayMuon) || NgayMuon.Nam < 1500) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Ngay muon khong hop le.";
        }
        return false;
    }
    if (SoSanhNgay(NgayMuon, LayNgayHienTai()) > 0) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Ngay muon khong duoc lon hon ngay hien tai.";
        }
        return false;
    }
    if (DocGiaCanXuLy.TrangThaiThe != 1) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "The doc gia dang bi khoa.";
        }
        return false;
    }
    if (DemSoSachDocGiaDangMuon(DocGiaCanXuLy) >= 3) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Doc gia da muon toi da 3 cuon.";
        }
        return false;
    }
    int SoNgayTreLonNhat = 0;
    if (KiemTraDocGiaQuaHanDenNgay(DocGiaCanXuLy, NgayMuon, &SoNgayTreLonNhat)) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Doc gia dang co sach QUA HAN (" + std::to_string(SoNgayTreLonNhat) + " ngay), khong duoc muon.";
        }
        return false;
    }
    DanhMucSachNode* BanSaoCoTheMuon = TimSachCoTheMuonDauTien(&DuLieuSach);
    if (BanSaoCoTheMuon == NULL) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Khong con ban sao ranh de muon.";
        }
        return false;
    }
    if (!DanhDauSachDaMuon(BanSaoCoTheMuon)) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Loi he thong: Khong the danh dau ban sao.";
        }
        return false;
    }
    NgayThangNam NgayTra{ 0,0,0 };
    ThemPhieuMuonTraChoDocGia(DocGiaCanXuLy, BanSaoCoTheMuon->MaSach, NgayMuon, NgayTra, 0);
    DuLieuSach.SoLuotMuon++;
    if (MaSachDaMuon != NULL) {
        *MaSachDaMuon = BanSaoCoTheMuon->MaSach;
    }
    return true;
}
// Kiểm tra điều kiện và thực hiện trả một bản sao
inline bool TraSach(
    DocGia& DocGiaCanXuLy,
    DanhSachDauSach& DanhSachDauSach,
    MuonTraNode* DoiTuongCanXuLy,
    const NgayThangNam& NgayTra,
    int* TongSoNgayKetQua = NULL,
    int* SoNgayTreKetQua = NULL,
    std::string* ThongBaoLoi = NULL)
{
    if (DoiTuongCanXuLy == NULL) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Khong tim thay phieu muon.";
        }
        return false;
    }
    if (DoiTuongCanXuLy->TrangThai != 0) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Phieu khong o trang thai DANG MUON.";
        }
        return false;
    }
    if (!KiemTraNgayHopLe(NgayTra) || NgayTra.Nam < 1500) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Ngay tra khong hop le.";
        }
        return false;
    }
    if (SoSanhNgay(NgayTra, LayNgayHienTai()) > 0) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Ngay tra khong duoc lon hon ngay hien tai.";
        }
        return false;
    }
    if (SoSanhNgay(NgayTra, DoiTuongCanXuLy->NgayMuon) < 0) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Ngay tra khong duoc nho hon ngay muon.";
        }
        return false;
    }
    char ISBNCanXuLy[15];
    LayISBNTuMaSach(DoiTuongCanXuLy->MaSach, ISBNCanXuLy, 15);
    DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy);
    if (DuLieuSach == NULL) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Khong tim thay dau sach cua phieu muon.";
        }
        return false;
    }
    DanhMucSachNode* BanSaoSach = TimSachTheoMaSach(DuLieuSach, DoiTuongCanXuLy->MaSach);
    if (BanSaoSach == NULL) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Khong tim thay ban sao can tra.";
        }
        return false;
    }
    if (!DanhDauSachDaTra(BanSaoSach)) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Trang thai ban sao khong hop le de tra.";
        }
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
    (void)DocGiaCanXuLy;
    return true;
}
// Thực hiện mượn sách dựa trên mã thẻ và ISBN
inline bool MuonSachTheoMaTheVaISBN(
    DocGiaNode* Root,
    DanhSachDauSach& DanhSachDauSach,
    int MaTheCanXuLy,
    const std::string& ISBNCanXuLy,
    const NgayThangNam& NgayMuon,
    std::string* MaSachDaMuon = NULL,
    std::string* ThongBaoLoi = NULL
) {
    DocGiaNode* NodeDocGia = TimDocGiaTheoMaThe(Root, MaTheCanXuLy);
    if (NodeDocGia == NULL) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Khong tim thay doc gia.";
        }
        return false;
    }
    DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy.c_str());
    if (DuLieuSach == NULL) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Khong tim thay dau sach voi ISBN da nhap.";
        }
        return false;
    }
    return MuonSach(NodeDocGia->ThongTin, *DuLieuSach, NgayMuon, MaSachDaMuon, ThongBaoLoi);
}
// Thực hiện trả sách dựa trên mã thẻ và mã sách
inline bool TraSachTheoMaTheVaMaSach(
    DocGiaNode* Root,
    DanhSachDauSach& DanhSachDauSach,
    int MaTheCanXuLy,
    const std::string& MaSachCanTra,
    const NgayThangNam& NgayTra,
    int* TongSoNgayKetQua = NULL,
    int* SoNgayTreKetQua = NULL,
    std::string* ThongBaoLoi = NULL
) {
    DocGiaNode* NodeDocGia = TimDocGiaTheoMaThe(Root, MaTheCanXuLy);
    if (NodeDocGia == NULL) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Khong tim thay doc gia.";
        }
        return false;
    }
    MuonTraNode* PhieuCanTra = TimPhieuDangMuonTheoMaSach(NodeDocGia->ThongTin, MaSachCanTra.c_str());
    if (PhieuCanTra == NULL) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Khong tim thay sach dang muon can tra.";
        }
        return false;
    }
    return TraSach(NodeDocGia->ThongTin, DanhSachDauSach, PhieuCanTra, NgayTra, TongSoNgayKetQua, SoNgayTreKetQua, ThongBaoLoi);
}
// Lập danh sách sách đang mượn của độc giả tại ngày kiểm tra
inline bool LapDanhSachSachDangMuonCuaDocGia(
    const DanhSachDauSach& DanhSachDauSach,
    DocGiaNode* Root,
    int MaTheCanXuLy,
    const NgayThangNam& NgayKiemTra,
    ThongTinSachDangMuon DanhSachKetQua[],
    int& SoLuongKetQua,
    DocGia*& DocGiaKetQua,
    int SoPhanTuToiDa = 100,
    std::string* ThongBaoLoi = NULL
) {
    SoLuongKetQua = 0;
    DocGiaKetQua = NULL;
    if (!KiemTraNgayHopLe(NgayKiemTra) || NgayKiemTra.Nam < 1500 ||
        SoSanhNgay(NgayKiemTra, LayNgayHienTai()) > 0) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Ngay kiem tra khong hop le.";
        }
        return false;
    }
    DocGiaNode* NodeDocGia = TimDocGiaTheoMaThe(Root, MaTheCanXuLy);
    if (NodeDocGia == NULL) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Ma the khong hop le hoac khong ton tai.";
        }
        return false;
    }
    DocGiaKetQua = &NodeDocGia->ThongTin;
    MuonTraNode* DanhSachPhieu[100];
    int SoLuongPhieu = 0;
    int GioiHan = SoPhanTuToiDa;
    if (GioiHan > 100) {
        GioiHan = 100;
    }
    LayDanhSachPhieuDangMuon(NodeDocGia->ThongTin, DanhSachPhieu, SoLuongPhieu, GioiHan);
    for (int i = 0; i < SoLuongPhieu && SoLuongKetQua < SoPhanTuToiDa; i++) {
        MuonTraNode* PhieuMuon = DanhSachPhieu[i];
        int TongSoNgay = TinhSoNgayChenhLech(NgayKiemTra, PhieuMuon->NgayMuon);
        if (TongSoNgay < 0) {
            continue;
        }
        ThongTinSachDangMuon& DongKetQua = DanhSachKetQua[SoLuongKetQua];
        char ISBNCanXuLy[15];
        LayISBNTuMaSach(PhieuMuon->MaSach, ISBNCanXuLy, 15);
        DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy);
        DongKetQua.PhieuMuon = PhieuMuon;
        DongKetQua.DuLieuSach = DuLieuSach;
        DongKetQua.TongSoNgay = TongSoNgay;
        SoLuongKetQua++;
    }
    return true;
}