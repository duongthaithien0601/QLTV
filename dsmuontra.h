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
inline bool MuonSach(
    DocGia& DocGiaCanXuLy,
    DauSach& DuLieuSach,
    const NgayThangNam& NgayMuon,
    std::string* MaSachDaMuon = NULL,
    std::string* ThongBaoLoi = NULL)
{
    // Kiểm tra ngày mượn
    if (!KiemTraNgayHopLe(NgayMuon) || NgayMuon.Nam < 1500){
        if (ThongBaoLoi != NULL){
            *ThongBaoLoi = "Ngay muon khong hop le.";
        }
        return false;
    }
    if (SoSanhNgay(NgayMuon,LayNgayHienTai()) > 0){
        if (ThongBaoLoi != NULL){
            *ThongBaoLoi = "Ngay muon khong duoc lon hon ngay hien tai.";
        }
        return false;
    }
    // Kiểm tra trạng thái thẻ
    if (DocGiaCanXuLy.TrangThaiThe != 1){
        if (ThongBaoLoi != NULL){
            *ThongBaoLoi = "The doc gia dang bi khoa.";}
        return false;
    }
    // Tối đa ba cuốn đang mượn
    if (DemSoSachDocGiaDangMuon(DocGiaCanXuLy) >= 3){
        if (ThongBaoLoi != NULL){
            *ThongBaoLoi = "Doc gia da muon toi da 3 cuon.";
        }
        return false;
    }
    // Không được mượn nếu đang có sách quá hạn
    int SoNgayTreLonNhat = 0;
    if (KiemTraDocGiaQuaHanDenNgay(DocGiaCanXuLy,NgayMuon,&SoNgayTreLonNhat)){
        if (ThongBaoLoi != NULL){
            *ThongBaoLoi = "Doc gia dang co sach QUA HAN (" + std::to_string(SoNgayTreLonNhat) + " ngay), khong duoc muon.";
        }
        return false;
    }
    // Tìm bản sao có thể cho mượn
    DanhMucSachNode* BanSaoCoTheMuon = TimSachCoTheMuonDauTien(&DuLieuSach);
    if (BanSaoCoTheMuon == NULL){
        if (ThongBaoLoi != NULL){
            *ThongBaoLoi = "Khong con ban sao ranh de muon.";
        }
        return false;
    }
    if (!DanhDauSachDaMuon(BanSaoCoTheMuon)){
        if (ThongBaoLoi != NULL){
            *ThongBaoLoi = "Loi he thong: Khong the danh dau ban sao.";
        }
        return false;
    }
    ThemPhieuMuonChoDocGia(DocGiaCanXuLy,BanSaoCoTheMuon->MaSach,NgayMuon);
    DuLieuSach.SoLuotMuon++;
    if (MaSachDaMuon != NULL){
        *MaSachDaMuon = BanSaoCoTheMuon->MaSach;
    }
    return true;
}
// Xử lý TRẢ SÁCH
inline bool TraSach(
    DocGia& DocGiaCanXuLy,
    DanhSachDauSach& DanhSachDauSach,
    MuonTraNode* DoiTuongCanXuLy,
    const NgayThangNam& NgayTra,
    int* TongSoNgayKetQua = NULL,
    int* SoNgayTreKetQua = NULL,
    std::string* ThongBaoLoi = NULL)
{
    if (DoiTuongCanXuLy == NULL){
        if (ThongBaoLoi != NULL){
            *ThongBaoLoi = "Khong tim thay phieu muon.";
        }
        return false;
    }
    if (DoiTuongCanXuLy->TrangThai != 0){
        if (ThongBaoLoi != NULL){
            *ThongBaoLoi = "Phieu khong o trang thai DANG MUON.";
        }
        return false;
    }
    // Kiểm tra ngày trả
    if (!KiemTraNgayHopLe(NgayTra) || NgayTra.Nam < 1500){
        if (ThongBaoLoi != NULL){
            *ThongBaoLoi = "Ngay tra khong hop le.";
        }
        return false;
    }
    if (SoSanhNgay(NgayTra,LayNgayHienTai()) > 0){
        if (ThongBaoLoi != NULL){
            *ThongBaoLoi = "Ngay tra khong duoc lon hon ngay hien tai.";
        }
        return false;
    }
    if (SoSanhNgay(NgayTra,DoiTuongCanXuLy->NgayMuon) < 0){
        if (ThongBaoLoi != NULL){
            *ThongBaoLoi = "Ngay tra khong duoc nho hon ngay muon.";
        }
        return false;
    }
    std::string ISBNCanXuLy = LayISBNTuMaSach(DoiTuongCanXuLy->MaSach);
    DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach,ISBNCanXuLy);
    if (DuLieuSach == NULL){
        if (ThongBaoLoi != NULL){
            *ThongBaoLoi = "Khong tim thay dau sach cua phieu muon.";
        }
        return false;
    }
    DanhMucSachNode* BanSaoSach = TimSachTheoMaSach(DuLieuSach,DoiTuongCanXuLy->MaSach);
    if (BanSaoSach == NULL){
        if (ThongBaoLoi != NULL){
            *ThongBaoLoi = "Khong tim thay ban sao can tra.";}
        return false;
    }
    if (!DanhDauSachDaTra(BanSaoSach)){
        if (ThongBaoLoi != NULL){
            *ThongBaoLoi = "Trang thai ban sao khong hop le de tra.";
        }
        return false;
    }
    // Chỉ cập nhật phiếu sau khi mọi kiểm tra thành công
    DoiTuongCanXuLy->TrangThai = 1;
    DoiTuongCanXuLy->NgayTra = NgayTra;
    int TongSoNgay = TinhSoNgayChenhLech(NgayTra,DoiTuongCanXuLy->NgayMuon);
    int SoNgayTre = std::max(0,TongSoNgay - HanMuonNgay);
    if (TongSoNgayKetQua != NULL){
        *TongSoNgayKetQua = TongSoNgay;
    }
    if (SoNgayTreKetQua != NULL){
        *SoNgayTreKetQua = SoNgayTre;
    }
    (void)DocGiaCanXuLy;
    return true;
}