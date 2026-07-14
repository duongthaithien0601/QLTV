#pragma once
#include <string>
#include <algorithm>
#include "cautruc.h"
#include "dsdms.h"
#include "dsdocgia.h"
#include "dsdausach.h"

// ===================== TIỆN ÍCH CƠ BẢN & HỖ TRỢ =========================
// Lấy tên sách dựa vào ISBN (Dùng để hiển thị thông báo nhanh)
inline std::string LayTenSachTheoISBN(const DanhSachDauSach& DanhSachDauSach, const std::string& ISBNCanXuLy) {
    const DauSach* ConTroHienTai = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy);
    if (ConTroHienTai == NULL) {
        return "";
    }
    return ConTroHienTai->TenSach;
}
// Tạo node mượn trả mới và gắn vào đầu danh sách liên kết của độc giả (Lưu ý: Hành động này chưa kiểm tra điều kiện
// mượn, chỉ đơn thuần là thêm dữ liệu)
inline void ThemPhieuMuonChoDocGia(DocGia& DocGiaCanXuLy, const std::string& MaSach, const NgayThangNam& NgayMuon) {
    MuonTraNode* NodeCanXuLy = new MuonTraNode();
    SaoChepChuoi(NodeCanXuLy->MaSach, MaxMaSach, MaSach);
    NodeCanXuLy->TrangThai = 0;
    NodeCanXuLy->NgayMuon = NgayMuon;
    // Thêm vào đầu danh sách (LIFO)
    NodeCanXuLy->Next = DocGiaCanXuLy.MuonTraHead;
    DocGiaCanXuLy.MuonTraHead = NodeCanXuLy;
}
// Trích xuất danh sách các sách ĐANG MƯỢN ra mảng (để hiển thị lên bảng)
inline void LayDanhSachDangMuon(DocGia& DocGiaCanXuLy, MuonTraNode* DanhSachKetQua[], int& SoLuongKetQua) {
    SoLuongKetQua = 0;
    for (MuonTraNode* ConTroHienTai = DocGiaCanXuLy.MuonTraHead; ConTroHienTai != NULL;
        ConTroHienTai = ConTroHienTai->Next) {
        if (ConTroHienTai->TrangThai == 0) {
            DanhSachKetQua[SoLuongKetQua++] = ConTroHienTai;
        }
    }
}

// ========== KIỂM TRA TÌNH TRẠNG SÁCH TRÊN TOÀN HỆ THỐNG ====================
// Kiểm tra xem một đầu sách (ISBN) có đang được độc giả nào mượn không
// Mục đích: Chặn xóa đầu sách nếu đang có người mượn
inline int DemSoSachDangMuonTheoISBN(DocGiaNode* Root, const std::string& ISBNCanXuLy) {
    int SoLuongDem = 0;
    // 1. Chuyển cây độc giả sang mảng tuyến tính để dễ duyệt
    static DocGia* MangDuLieu[MaxDocGia];
    int SoPhanTu = 0;
    DuyetLNRChuyenThanhMang(Root, MangDuLieu, SoPhanTu);
    // 2. Duyệt qua từng độc giả
    for (int i = 0; i < SoPhanTu; i++) {
        DocGia* DocGiaCanXuLy = MangDuLieu[i];
        // Duyệt qua danh sách mượn trả của từng người
        for (MuonTraNode* ConTroHienTai = DocGiaCanXuLy->MuonTraHead; ConTroHienTai;
            ConTroHienTai = ConTroHienTai->Next) {
            // Nếu đang mượn VÀ đúng ISBN cần tìm
            if (ConTroHienTai->TrangThai == 0 && LayISBNTuMaSach(ConTroHienTai->MaSach) == ISBNCanXuLy) {
                SoLuongDem++;
            }
        }
    }
    return SoLuongDem;
}

// ===================== KIỂM TRA QUÁ HẠN =======================
// Kiểm tra độc giả có giữ sách quá hạn không
inline bool KiemTraDocGiaQuaHanDenNgay(const DocGia& DocGiaCanXuLy,
    const NgayThangNam& NgayHienTai,
    int* KetQuaSoNgayTreLonNhat = NULL) {
    int SoNgayTreLonNhat = 0;
    bool CoSachQuaHan = false;
    for (MuonTraNode* ConTroHienTai = DocGiaCanXuLy.MuonTraHead; ConTroHienTai != NULL;
        ConTroHienTai = ConTroHienTai->Next) {
        if (ConTroHienTai->TrangThai == 0) {
            int TongSoNgay = TinhSoNgayChenhLech(NgayHienTai, ConTroHienTai->NgayMuon);
            int Tre = std::max(0, TongSoNgay - HanMuonNgay);
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
// Xử lý MƯỢN SÁCH
// Trả về true nếu mượn thành công, false nếu vi phạm quy định
inline bool MuonSach(DocGia& DocGiaCanXuLy,
    DauSach& DuLieuSach,
    const NgayThangNam& NgayMuon,
    std::string* MaSachDaMuon = NULL,
    std::string* ThongBaoLoi = NULL) {
    // 1. Kiểm tra trạng thái thẻ
    if (DocGiaCanXuLy.TrangThaiThe != 1) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "The doc gia dang bi khoa.";
        }
        return false;
    }
    // 2. Kiểm tra số lượng sách đang mượn (Tối đa 3 cuốn)
    if (DemSoSachDocGiaDangMuon(DocGiaCanXuLy) >= 3) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Doc gia da muon toi da 3 cuon.";
        }
        return false;
    }
    // 3. Kiểm tra sách quá hạn
    int SoNgayTreLonNhat = 0;
    if (KiemTraDocGiaQuaHanDenNgay(DocGiaCanXuLy, NgayMuon, &SoNgayTreLonNhat)) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi =
                "Doc gia dang co sach QUA HAN (" + std::to_string(SoNgayTreLonNhat) + " ngay), khong duoc muon.";
        }
        return false;
    }
    // 4. Tìm bản sao còn trong kho
    DanhMucSachNode* BanSaoCoTheMuon = TimSachCoTheMuonDauTien(&DuLieuSach);
    if (BanSaoCoTheMuon == NULL) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Khong con ban sao ranh de muon.";
        }
        return false;
    }
    // 5. Cập nhật trạng thái bản sao -> ĐÃ MƯỢN
    if (!DanhDauSachDaMuon(BanSaoCoTheMuon)) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Loi he thong: Khong the danh dau ban sao.";
        }
        return false;
    }
    // 6. Ghi nhận phiếu mượn vào hồ sơ độc giả
    ThemPhieuMuonChoDocGia(DocGiaCanXuLy, BanSaoCoTheMuon->MaSach, NgayMuon);
    DuLieuSach.SoLuotMuon += 1; // Tăng thống kê lượt mượn
    // Trả về mã sách vừa mượn
    if (MaSachDaMuon != NULL) {
        *MaSachDaMuon = BanSaoCoTheMuon->MaSach;
    }
    return true;
}
// Xử lý TRẢ SÁCH
inline bool TraSach(DocGia& DocGiaCanXuLy,
    DanhSachDauSach& DanhSachDauSach,
    MuonTraNode* DoiTuongCanXuLy,
    const NgayThangNam& NgayTra,
    int* TongSoNgayKetQua = NULL,
    int* SoNgayTreKetQua = NULL,
    std::string* ThongBaoLoi = NULL) {
    // 1. Kiểm tra tính hợp lệ của phiếu mượn
    if (DoiTuongCanXuLy == NULL) {
        if (ThongBaoLoi) {
            *ThongBaoLoi = "Khong tim thay phieu muon.";
        }
        return false;
    }
    if (DoiTuongCanXuLy->TrangThai != 0) {
        if (ThongBaoLoi) {
            *ThongBaoLoi = "Phieu khong o trang thai DANG MUON.";
        }
        return false;
    }
    // 2. Cập nhật trạng thái phiếu -> ĐÃ TRẢ
    DoiTuongCanXuLy->TrangThai = 1;
    DoiTuongCanXuLy->NgayTra = NgayTra;
    // 3. Cập nhật trạng thái bản sao trong kho -> CHO MƯỢN
    const std::string ISBNCanXuLy = LayISBNTuMaSach(DoiTuongCanXuLy->MaSach);
    DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy);
    if (DuLieuSach != NULL) {
        DanhMucSachNode* BanSaoSach = TimSachTheoMaSach(DuLieuSach, DoiTuongCanXuLy->MaSach);
        if (BanSaoSach != NULL) {
            DanhDauSachDaTra(BanSaoSach);
        }
    }
    // 4. Tính toán số liệu (Số ngày mượn, Số ngày trễ)
    const int TongSoNgay = TinhSoNgayChenhLech(NgayTra, DoiTuongCanXuLy->NgayMuon);
    const int Tre = std::max(0, TongSoNgay - HanMuonNgay);
    if (TongSoNgayKetQua) {
        *TongSoNgayKetQua = TongSoNgay;
    }
    if (SoNgayTreKetQua) {
        *SoNgayTreKetQua = Tre;
    }
    (void)DocGiaCanXuLy;
    return true;
}

