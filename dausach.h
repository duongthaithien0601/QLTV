#pragma once
#include <string>
#include <random>
#include <ctime>
#include "cautruc.h"

// ======================= DỮ LIỆU KẾT QUẢ =======================
struct ThongTinDauSachTheoTheLoai {
    DauSach* DuLieuSach;
    int LaDauNhom;
    int SoDauSachTrongNhom;

    ThongTinDauSachTheoTheLoai() {
        DuLieuSach = NULL;
        LaDauNhom = 0;
        SoDauSachTrongNhom = 0;
    }
};

// ======================= HÀM HỖ TRỢ ĐẦU SÁCH =======================
// Lấy ISBN từ mã sách
inline void LayISBNTuMaSach(const char MaSach[], char ISBNKetQua[], int KichThuoc) {
    if (MaSach == NULL || ISBNKetQua == NULL || KichThuoc <= 0) {
        return;
    }
    int i = 0;
    while (i < KichThuoc - 1 && MaSach[i] != '\0' && MaSach[i] != '-') {
        ISBNKetQua[i] = MaSach[i];
        i++;
    }
    ISBNKetQua[i] = '\0';
}
// Tạo mã sách từ ISBN và số thứ tự bản sao
inline std::string TaoMaSach(const std::string& ISBNCanXuLy, int ChiSo) {
    std::ostringstream Oss;
    Oss << CatKhoangTrangHaiDau(ISBNCanXuLy) << "-" << ChiSo;
    return Oss.str();
}
// Tìm đầu sách trong danh sách theo ISBN
inline DauSach* TimDauSachTheoISBN(const DanhSachDauSach& DuLieuSach, const char ISBNCanXuLy[]) {
    if (ISBNCanXuLy == NULL) {
        return NULL;
    }
    for (int i = 0; i < DuLieuSach.SoLuong; i++) {
        if (DuLieuSach.Nodes[i] != NULL && std::strcmp(DuLieuSach.Nodes[i]->ISBN, ISBNCanXuLy) == 0) {
            return DuLieuSach.Nodes[i];
        }
    }
    return NULL;
}

// ======================= ĐẾM BẢN SAO =======================
// Đếm tổng số bản sao của một đầu sách
inline int DemTongSoBanSao(const DauSach* DuLieuSach) {
    if (DuLieuSach == NULL) {
        return 0;
    }
    int Dem = 0;
    for (const DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead; ConTroHienTai != NULL; ConTroHienTai = ConTroHienTai->Next) {
        Dem++;
    }
    return Dem;
}
// Đếm số bản sao đang được mượn của một đầu sách
inline int DemSoSachDangMuon(const DauSach* DuLieuSach) {
    if (DuLieuSach == NULL) {
        return 0;
    }
    int Dem = 0;
    for (const DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead; ConTroHienTai != NULL; ConTroHienTai = ConTroHienTai->Next) {
        if (ConTroHienTai->TrangThai == 1) {
            Dem++;
        }
    }
    return Dem;
}

// ======================= NGHIỆP VỤ DANH MỤC SÁCH =======================
// =================== TIỆN ÍCH DSLK ===================
// Tìm bản sao đầu tiên đang ở trạng thái có thể cho mượn
inline DanhMucSachNode* TimSachCoTheMuonDauTien(DauSach* DuLieuSach) {
    if (DuLieuSach == NULL) {
        return NULL;
    }
    for (DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead; ConTroHienTai != NULL; ConTroHienTai = ConTroHienTai->Next) {
        if (ConTroHienTai->TrangThai == 0) {
            return ConTroHienTai;
        }
    }
    return NULL;
}
// Tìm một bản sao trong danh mục theo mã sách
inline DanhMucSachNode* TimSachTheoMaSach(DauSach* DuLieuSach, const char MaSach[]) {
    if (DuLieuSach == NULL || MaSach == NULL) {
        return NULL;
    }
    for (DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead; ConTroHienTai != NULL; ConTroHienTai = ConTroHienTai->Next) {
        if (std::strcmp(ConTroHienTai->MaSach, MaSach) == 0) {
            return ConTroHienTai;
        }
    }
    return NULL;
}

// =================== THÊM / GỠ BẢN SAO ===================
// Thêm một bản sao vào cuối danh mục sách
inline void ThemSachVaoCuoiDanhMuc(DauSach* DuLieuSach, DanhMucSachNode* NodeCanXuLy) {
    if (DuLieuSach == NULL || NodeCanXuLy == NULL) {
        return;
    }
    NodeCanXuLy->Next = NULL;
    if (DuLieuSach->DanhMucSachHead == NULL) {
        DuLieuSach->DanhMucSachHead = NodeCanXuLy;
    }
    else {
        DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead;
        while (ConTroHienTai->Next != NULL) {
            ConTroHienTai = ConTroHienTai->Next;
        }
        ConTroHienTai->Next = NodeCanXuLy;
    }
    DuLieuSach->SoLuongBanSao++;
}
// Tách một bản sao ra khỏi danh mục sách
inline bool TachSachKhoiDanhMuc(DauSach* DuLieuSach, DanhMucSachNode* DoiTuongCanXuLy) {
    if (DuLieuSach == NULL || DoiTuongCanXuLy == NULL) {
        return false;
    }
    DanhMucSachNode* ChiSoTruoc = NULL;
    DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead;
    while (ConTroHienTai != NULL) {
        if (ConTroHienTai == DoiTuongCanXuLy) {
            if (ChiSoTruoc == NULL) {
                DuLieuSach->DanhMucSachHead = ConTroHienTai->Next;
            }
            else {
                ChiSoTruoc->Next = ConTroHienTai->Next;
            }
            DuLieuSach->SoLuongBanSao--;
            return true;
        }
        ChiSoTruoc = ConTroHienTai;
        ConTroHienTai = ConTroHienTai->Next;
    }
    return false;
}
// Giải phóng toàn bộ danh sách liên kết các bản sao
inline void GiaiPhongDanhMucSach(DanhMucSachNode*& Head) {
    DanhMucSachNode* ConTroHienTai = Head;
    while (ConTroHienTai != NULL) {
        DanhMucSachNode* Nxt = ConTroHienTai->Next;
        delete ConTroHienTai;
        ConTroHienTai = Nxt;
    }
    Head = NULL;
}
// Lấy thông tin các bản sao để phục vụ hiển thị
inline void LayDanhSachBanSao(const DauSach* DuLieuSach, const DanhMucSachNode* DanhSachKetQua[], int& SoLuongKetQua, int SoPhanTuToiDa = 5000){
    SoLuongKetQua = 0;
    if (DuLieuSach == NULL || DanhSachKetQua == NULL || SoPhanTuToiDa <= 0) {
        return;
    }
    for (
        const DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead;
        ConTroHienTai != NULL && SoLuongKetQua < SoPhanTuToiDa;
        ConTroHienTai = ConTroHienTai->Next){
        DanhSachKetQua[SoLuongKetQua] = ConTroHienTai;
        SoLuongKetQua++;
    }
}

// =================== ĐÁNH DẤU MƯỢN / TRẢ ===================
// Chuyển trạng thái bản sao từ có thể mượn sang đã mượn
inline bool DanhDauSachDaMuon(DanhMucSachNode* NodeCanXuLy) {
    if (NodeCanXuLy == NULL) {
        return false;
    }
    if (NodeCanXuLy->TrangThai != 0) {
        return false;
    }
    NodeCanXuLy->TrangThai = 1;
    return true;
}
// Chuyển trạng thái bản sao từ đã mượn sang có thể mượn
inline bool DanhDauSachDaTra(DanhMucSachNode* NodeCanXuLy) {
    if (NodeCanXuLy == NULL) {
        return false;
    }
    if (NodeCanXuLy->TrangThai != 1) {
        return false;
    }
    NodeCanXuLy->TrangThai = 0;
    return true;
}

// =================== KHÁC ===================
// Đồng bộ số lượng bản sao theo số node thực tế trong danh mục
inline void CapNhatSoLuongBanSao(DauSach* DuLieuSach) {
    if (DuLieuSach == NULL) {
        return;
    }
    DuLieuSach->SoLuongBanSao = DemTongSoBanSao(DuLieuSach);
}
// Đặt tất cả bản sao về trạng thái có thể cho mượn
inline void DatLaiTrangThaiTatCaBanSao(DanhSachDauSach& DanhSachDauSach) {
    for (int i = 0;i < DanhSachDauSach.SoLuong;i++) {
        DauSach* DuLieuSach = DanhSachDauSach.Nodes[i];
        if (DuLieuSach == NULL) {
            continue;
        }
        for (DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead;ConTroHienTai != NULL;ConTroHienTai = ConTroHienTai->Next) {
            ConTroHienTai->TrangThai = 0;
        }
    }
}

// ======================= NGHIỆP VỤ ĐẦU SÁCH =======================
inline void TaoBanSaoTuDong(DauSach* DuLieuSach, int SoLuongCanXuLy);
inline bool GiamBanSaoTuCuoi(DauSach* DuLieuSach, int SoLuongCanXoa);

// ==================== CÁC HÀM KIỂM TRA & TÌM KIẾM CƠ BẢN ======================
// Kiểm tra danh sách đầu sách đã đạt số lượng tối đa hay chưa
inline bool KiemTraDanhSachDauSachDay(const DanhSachDauSach& DuLieuSach) {
    return DuLieuSach.SoLuong >= MaxDauSach;
}
// Kiểm tra ISBN đã tồn tại trong danh sách đầu sách hay chưa
inline bool KiemTraISBNTonTai(const DanhSachDauSach& DuLieuSach, const std::string& ISBNCanXuLy) {
    return TimDauSachTheoISBN(DuLieuSach, ISBNCanXuLy.c_str()) != NULL;
}

// ================== NGHIỆP VỤ XỬ LÝ ISBN ====================
// Tạo ISBN ngẫu nhiên và bảo đảm không trùng với ISBN đã có
inline std::string TaoISBNKhongTrung(const DanhSachDauSach& DanhSachDauSach) {
    static std::mt19937 Rng(static_cast<unsigned int>(std::time(NULL)));
    static std::uniform_int_distribution<int> Dist(100000000, 999999999);
    while (true) {
        int GiaTriNgauNhien = Dist(Rng);
        std::string ISBNMoi = std::to_string(GiaTriNgauNhien);
        if (!KiemTraISBNTonTai(DanhSachDauSach, ISBNMoi)) {
            return ISBNMoi;
        }
    }
}

// ===================== THÊM / XÓA ĐẦU SÁCH =====================
// Chèn đầu sách vào đúng vị trí theo thứ tự tên tăng dần
inline bool ChenDauSachTheoTen(DanhSachDauSach& DuLieuSach, DauSach* ConTroHienTai) {
    if (KiemTraDanhSachDauSachDay(DuLieuSach)) {
        return false;
    }
    int k = 0;
    while (k < DuLieuSach.SoLuong) {
        if (std::strcmp(DuLieuSach.Nodes[k]->TenSach, ConTroHienTai->TenSach) > 0) {
            break;
        }
        k++;
    }
    for (int i = DuLieuSach.SoLuong; i > k; i--) {
        DuLieuSach.Nodes[i] = DuLieuSach.Nodes[i - 1];
    }
    DuLieuSach.Nodes[k] = ConTroHienTai;
    DuLieuSach.SoLuong++;
    return true;
}
// Tách một đầu sách khỏi mảng nhưng không xóa vùng nhớ của đầu sách
inline bool TachDauSachKhoiMang(DanhSachDauSach& DuLieuSach, DauSach* DauSachCanTach){
    if (DauSachCanTach == NULL) {
        return false;
    }
    int ViTriCanTach = -1;
    for (int i = 0; i < DuLieuSach.SoLuong; i++) {
        if (DuLieuSach.Nodes[i] == DauSachCanTach) {
            ViTriCanTach = i;
            break;
        }
    }
    if (ViTriCanTach == -1) {
        return false;
    }
    for (int i = ViTriCanTach; i < DuLieuSach.SoLuong - 1; i++){
        DuLieuSach.Nodes[i] = DuLieuSach.Nodes[i + 1];
    }
    DuLieuSach.SoLuong--;
    DuLieuSach.Nodes[DuLieuSach.SoLuong] = NULL;
    return true;
}
// Xóa đầu sách theo ISBN khi không còn bản sao đang được mượn
inline bool XoaDauSach(DanhSachDauSach& DuLieuSach, const std::string& ISBNCanXuLy, std::string* ThongBaoLoi = NULL) {
    int ChiSo = -1;
    for (int i = 0; i < DuLieuSach.SoLuong; i++) {
        if (std::strcmp(DuLieuSach.Nodes[i]->ISBN, ISBNCanXuLy.c_str()) == 0) {
            ChiSo = i;
            break;
        }
    }
    if (ChiSo == -1) {
        if (ThongBaoLoi) {
            *ThongBaoLoi = "Khong tim thay ISBN.";
        }
        return false;
    }
    DauSach* ConTroHienTai = DuLieuSach.Nodes[ChiSo];
    if (DemSoSachDangMuon(ConTroHienTai) > 0) {
        if (ThongBaoLoi) {
            *ThongBaoLoi = "Khong the xoa: dang co doc gia muon.";
        }
        return false;
    }
    GiaiPhongDanhMucSach(ConTroHienTai->DanhMucSachHead);
    delete ConTroHienTai;
    for (int i = ChiSo; i < DuLieuSach.SoLuong - 1; i++) {
        DuLieuSach.Nodes[i] = DuLieuSach.Nodes[i + 1];
    }
    DuLieuSach.SoLuong--;
    return true;
}
// Tạo đầy đủ dữ liệu đầu sách mới và chèn vào danh sách
inline bool ThemDauSachMoi(
    DanhSachDauSach& DanhSachDauSach,
    const std::string& ISBNCanXuLy,
    const std::string& TenSachNhap,
    int SoTrangNhap,
    const std::string& TacGiaNhap,
    int NamXuatBanNhap,
    const std::string& TheLoaiNhap,
    int SoLuongBanSaoNhap,
    std::string* ThongBaoLoi = NULL
) {
    if (KiemTraDanhSachDauSachDay(DanhSachDauSach)) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Danh sach dau sach da day.";
        }
        return false;
    }
    if (ISBNCanXuLy.empty() || KiemTraISBNTonTai(DanhSachDauSach, ISBNCanXuLy)) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "ISBN khong hop le hoac da ton tai.";
        }
        return false;
    }
    std::string TenDaChuanHoa = ChuanHoaChuoi(TenSachNhap);
    std::string TacGiaDaChuanHoa = ChuanHoaChuoi(TacGiaNhap);
    std::string TheLoaiDaChuanHoa = ChuanHoaChuoi(TheLoaiNhap);
    if (!KiemTraThongTinDauSachNhap(
        TenDaChuanHoa,
        TacGiaDaChuanHoa,
        TheLoaiDaChuanHoa,
        SoTrangNhap,
        NamXuatBanNhap,
        SoLuongBanSaoNhap,
        ThongBaoLoi
    )) {
        return false;
    }
    DauSach* DauSachMoi = new DauSach();
    SaoChepChuoi(DauSachMoi->ISBN, 15, ISBNCanXuLy);
    SaoChepChuoi(DauSachMoi->TenSach, 100, TenDaChuanHoa);
    SaoChepChuoi(DauSachMoi->TacGia, 60, TacGiaDaChuanHoa);
    SaoChepChuoi(DauSachMoi->TheLoai, 40, TheLoaiDaChuanHoa);
    DauSachMoi->SoTrang = SoTrangNhap;
    DauSachMoi->NamXuatBan = NamXuatBanNhap;
    DauSachMoi->SoLuongBanSao = 0;
    DauSachMoi->SoLuotMuon = 0;
    DauSachMoi->DanhMucSachHead = NULL;

    TaoBanSaoTuDong(DauSachMoi, SoLuongBanSaoNhap);
    if (!ChenDauSachTheoTen(DanhSachDauSach, DauSachMoi)) {
        GiaiPhongDanhMucSach(DauSachMoi->DanhMucSachHead);
        delete DauSachMoi;
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Khong the chen dau sach vao danh sach.";
        }
        return false;
    }
    return true;
}
// Lấy thông tin số bản sao trước khi xác nhận xóa đầu sách
inline bool LayThongTinDauSachDeXoa(
    const DanhSachDauSach& DanhSachDauSach,
    const std::string& ISBNCanXuLy,
    int& TongSoBanSao,
    int& SoSachDangMuon,
    std::string* ThongBaoLoi = NULL
) {
    DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy.c_str());
    if (DuLieuSach == NULL) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Khong tim thay ISBN.";
        }
        return false;
    }
    TongSoBanSao = DemTongSoBanSao(DuLieuSach);
    SoSachDangMuon = DemSoSachDangMuon(DuLieuSach);
    return true;
}

// ====================== QUẢN LÝ BẢN SAO ======================
// Tạo thêm các bản sao có mã sách tự động cho đầu sách
inline void TaoBanSaoTuDong(DauSach* DuLieuSach, int SoLuongCanXuLy) {
    if (DuLieuSach == NULL || SoLuongCanXuLy <= 0) {
        return;
    }
    int ChiSoBatDau = DuLieuSach->SoLuongBanSao + 1;
    for (int i = 0; i < SoLuongCanXuLy; i++) {
        DanhMucSachNode* NodeCanXuLy = new DanhMucSachNode();
        SaoChepChuoi(
            NodeCanXuLy->MaSach,
            MaxMaSach,
            TaoMaSach(DuLieuSach->ISBN, ChiSoBatDau + i)
        );
        NodeCanXuLy->TrangThai = 0;
        ThemSachVaoCuoiDanhMuc(DuLieuSach, NodeCanXuLy);
    }
}
// Giảm số bản sao từ cuối danh mục và bỏ qua sách đang mượn
inline bool GiamBanSaoTuCuoi(DauSach* DuLieuSach, int SoLuongCanXoa) {
    if (DuLieuSach == NULL || SoLuongCanXoa <= 0) {
        return true;
    }
    DanhMucSachNode* Nodes[5000];
    int SoLuongTimThay = 0;
    for (DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead; ConTroHienTai != NULL;
        ConTroHienTai = ConTroHienTai->Next) {
        if (SoLuongTimThay < 5000) {
            Nodes[SoLuongTimThay++] = ConTroHienTai;
        }
    }
    int DaXoa = 0;
    for (int i = SoLuongTimThay - 1; i >= 0 && DaXoa < SoLuongCanXoa; --i) {
        DanhMucSachNode* ChiSoHienTai = Nodes[i];
        if (ChiSoHienTai->TrangThai == 1) {
            continue;
        }
        if (ChiSoHienTai == DuLieuSach->DanhMucSachHead) {
            DuLieuSach->DanhMucSachHead = ChiSoHienTai->Next;
        }
        else {
            if (i > 0) {
                Nodes[i - 1]->Next = ChiSoHienTai->Next;
            }
        }
        delete ChiSoHienTai;
        DuLieuSach->SoLuongBanSao--;
        DaXoa++;
    }
    return (DaXoa == SoLuongCanXoa);
}
// Kiểm tra số lượng bản sao mới trước khi cập nhật
inline bool KiemTraSoLuongBanSaoMoi(
    const DauSach* DuLieuSach,
    int SoLuongMoi,
    std::string* ThongBaoLoi = NULL
) {
    if (DuLieuSach == NULL) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Khong tim thay dau sach.";
        }
        return false;
    }
    if (SoLuongMoi < 1 || SoLuongMoi > 5000) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "So luong ban sao phai tu 1 den 5000.";
        }
        return false;
    }
    int SoSachDangMuon = DemSoSachDangMuon(DuLieuSach);
    if (SoLuongMoi < SoSachDangMuon) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Khong the giam vi co " +
                std::to_string(SoSachDangMuon) +
                " cuon dang duoc muon.";
        }
        return false;
    }
    return true;
}
// Cập nhật thông tin và số lượng bản sao của đầu sách
inline bool CapNhatThongTinDauSach(
    DanhSachDauSach& DanhSachDauSach,
    const std::string& ISBNCanXuLy,
    const std::string& TenSachMoi,
    const std::string& TacGiaMoi,
    int NamXuatBanMoi,
    int SoTrangMoi,
    int CoThayDoiSoLuong,
    int SoLuongBanSaoMoi,
    std::string* ThongBaoLoi = NULL
) {
    DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy.c_str());
    if (DuLieuSach == NULL) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Khong tim thay ISBN.";
        }
        return false;
    }
    std::string TenDaChuanHoa = ChuanHoaChuoi(TenSachMoi);
    std::string TacGiaDaChuanHoa = ChuanHoaChuoi(TacGiaMoi);
    if (!KiemTraThongTinDauSachCapNhat(TenSachMoi, TacGiaMoi, NamXuatBanMoi, SoTrangMoi, ThongBaoLoi)){
        return false;
    }
    if (CoThayDoiSoLuong == 1 && !KiemTraSoLuongBanSaoMoi(DuLieuSach, SoLuongBanSaoMoi, ThongBaoLoi)){
        return false;
    }
    // Cập nhật số lượng bản sao trước
    if (CoThayDoiSoLuong == 1){
        if (SoLuongBanSaoMoi > DuLieuSach->SoLuongBanSao){
            TaoBanSaoTuDong(DuLieuSach, SoLuongBanSaoMoi - DuLieuSach->SoLuongBanSao);
        }
        else if (SoLuongBanSaoMoi < DuLieuSach->SoLuongBanSao){
            if (!GiamBanSaoTuCuoi(DuLieuSach, DuLieuSach->SoLuongBanSao - SoLuongBanSaoMoi)){
                if (ThongBaoLoi != NULL) {
                    *ThongBaoLoi =
                        "Khong the giam du so luong ban sao.";
                }
                return false;
            }
        }
    }
    // Ghi nhận tên sách có thực sự thay đổi hay không
    bool CoThayDoiTen = false;
    if (!TenSachMoi.empty()) {
        CoThayDoiTen = std::strcmp(DuLieuSach->TenSach, TenDaChuanHoa.c_str()) != 0;
        SaoChepChuoi(DuLieuSach->TenSach, 100,  TenDaChuanHoa);
    }
    if (!TacGiaMoi.empty()){
        SaoChepChuoi(DuLieuSach->TacGia, 60, TacGiaDaChuanHoa);
    }
    if (NamXuatBanMoi != 0) {
        DuLieuSach->NamXuatBan = NamXuatBanMoi;
    }
    if (SoTrangMoi != 0) {
        DuLieuSach->SoTrang = SoTrangMoi;
    }
    // Nếu tên sách thay đổi thì tách khỏi vị trí cũ rồi chèn lại bằng Insert Order
    if (CoThayDoiTen) {
        if (!TachDauSachKhoiMang(DanhSachDauSach, DuLieuSach)){
            if (ThongBaoLoi != NULL) {
                *ThongBaoLoi =
                    "Khong the tach dau sach khoi vi tri cu.";
            }
            return false;
        }
        if (!ChenDauSachTheoTen(DanhSachDauSach, DuLieuSach)){
            if (ThongBaoLoi != NULL) {
                *ThongBaoLoi =
                    "Khong the chen lai dau sach theo ten.";
            }
            return false;
        }
    }
    return true;
}

// ====================  TÌM KIẾM THEO TÊN ====================
inline void TimDauSachTheoTen(const DanhSachDauSach& DuLieuSach, const char TuKhoaTimKiem[], DauSach* DanhSachKetQua[], int& SoLuongTimThay) {
    SoLuongTimThay = 0;
    if (TuKhoaTimKiem == NULL || TuKhoaTimKiem[0] == '\0') {
        return;
    }
    for (int i = 0; i < DuLieuSach.SoLuong; i++) {
        if (std::strstr(DuLieuSach.Nodes[i]->TenSach, TuKhoaTimKiem) != NULL) {
            DanhSachKetQua[SoLuongTimThay++] = DuLieuSach.Nodes[i];
        }
    }
}

// ===================== DỌN DẸP BỘ NHỚ =========================
inline void GiaiPhongDanhSachDauSach(DanhSachDauSach& DuLieuSach) {
    for (int i = 0; i < DuLieuSach.SoLuong; i++) {
        if (DuLieuSach.Nodes[i] != NULL) {
            GiaiPhongDanhMucSach(DuLieuSach.Nodes[i]->DanhMucSachHead);
            delete DuLieuSach.Nodes[i];
            DuLieuSach.Nodes[i] = NULL;
        }
    }
    DuLieuSach.SoLuong = 0;
}

// ================ CHÈN CÓ THỨ TỰ THEO THỂ LOẠI VÀ TÊN SÁCH ==================
// Kiểm tra đầu sách thứ nhất có đứng trước đầu sách thứ hai
// theo thứ tự thể loại và tên sách hay không
inline bool DauSachDungTruocTheoTheLoaiVaTen(const DauSach* DauSachThuNhat, const DauSach* DauSachThuHai){
    int KetQuaSoSanhTheLoai = std::strcmp(DauSachThuNhat->TheLoai, DauSachThuHai->TheLoai);
    if (KetQuaSoSanhTheLoai < 0) {
        return true;
    }
    if (KetQuaSoSanhTheLoai > 0) {
        return false;
    }
    int KetQuaSoSanhTenSach = std::strcmp(DauSachThuNhat->TenSach, DauSachThuHai->TenSach);
    if (KetQuaSoSanhTenSach < 0) {
        return true;
    }
    if (KetQuaSoSanhTenSach > 0) {
        return false;
    }
    return std::strcmp(DauSachThuNhat->ISBN, DauSachThuHai->ISBN) < 0;
}
// Chèn một đầu sách vào mảng đang tăng dần
// theo thể loại và tên sách
inline void ChenDauSachTheoTheLoaiVaTen(DauSach* DanhSachDich[], int& SoPhanTu, DauSach* DauSachCanChen){
    if (DanhSachDich == NULL || DauSachCanChen == NULL || SoPhanTu >= MaxDauSach){
        return;
    }
    int ViTriChen = SoPhanTu;
    while (ViTriChen > 0 && DauSachDungTruocTheoTheLoaiVaTen(DauSachCanChen, DanhSachDich[ViTriChen - 1])){
        DanhSachDich[ViTriChen] = DanhSachDich[ViTriChen - 1];
        ViTriChen--;
    }
    DanhSachDich[ViTriChen] = DauSachCanChen;
    SoPhanTu++;
}
// Lập danh sách đầu sách bằng cách lần lượt
// chèn từng đầu sách vào đúng vị trí
inline void LayDanhSachSapXepTheoTheLoai(const DanhSachDauSach& DanhSachNguon, DauSach* DanhSachDich[], int& SoPhanTu){
    SoPhanTu = 0;
    if (DanhSachDich == NULL) {
        return;
    }
    for (int i = 0; i < DanhSachNguon.SoLuong; i++) {
        if (DanhSachNguon.Nodes[i] != NULL) {
            ChenDauSachTheoTheLoaiVaTen(DanhSachDich, SoPhanTu, DanhSachNguon.Nodes[i]);
        }
    }
}
// Lập dữ liệu đầu sách đã chia nhóm theo từng thể loại
inline void LapDanhSachDauSachTheoTheLoai(
    const DanhSachDauSach& DanhSachNguon,
    ThongTinDauSachTheoTheLoai DanhSachKetQua[],
    int& SoLuongKetQua,
    int SoPhanTuToiDa = MaxDauSach
) {
    SoLuongKetQua = 0;
    if (DanhSachKetQua == NULL || SoPhanTuToiDa <= 0) {
        return;
    }
    DauSach* DanhSachDaSapXep[MaxDauSach];
    int SoLuongDaSapXep = 0;
    LayDanhSachSapXepTheoTheLoai(DanhSachNguon, DanhSachDaSapXep, SoLuongDaSapXep);
    for (int i = 0; i < SoLuongDaSapXep && SoLuongKetQua < SoPhanTuToiDa; i++) {
        DauSach* DuLieuSach = DanhSachDaSapXep[i];
        if (DuLieuSach == NULL) {
            continue;
        }
        ThongTinDauSachTheoTheLoai& DongKetQua = DanhSachKetQua[SoLuongKetQua];
        DongKetQua.DuLieuSach = DuLieuSach;
        DongKetQua.LaDauNhom = 0;
        DongKetQua.SoDauSachTrongNhom = 0;
        if (i == 0 || std::strcmp(DanhSachDaSapXep[i - 1]->TheLoai, DuLieuSach->TheLoai) != 0) {
            DongKetQua.LaDauNhom = 1;
            for (int j = i; j < SoLuongDaSapXep; j++) {
                if (std::strcmp(DanhSachDaSapXep[j]->TheLoai, DuLieuSach->TheLoai) != 0) {
                    break;
                }
                DongKetQua.SoDauSachTrongNhom++;
            }
        }
        SoLuongKetQua++;
    }
}