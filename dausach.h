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
inline void TaoMaSach(const char ISBNCanXuLy[], int ChiSo, char MaSachKetQua[], int KichThuoc) {
    if (ISBNCanXuLy == NULL || MaSachKetQua == NULL || KichThuoc <= 0) {
        return;
    }
    std::snprintf(MaSachKetQua, static_cast<size_t>(KichThuoc), "%s-%d", ISBNCanXuLy, ChiSo);
}
// Tìm và trả về địa chỉ đầu sách có ISBN tương ứng
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
// Giải phóng toàn bộ danh sách liên kết các bản sao
inline void GiaiPhongDanhMucSach(DanhMucSachNode*& Head) {
    DanhMucSachNode* ConTroHienTai = Head;
    while (ConTroHienTai != NULL) {
        DanhMucSachNode* NodeTiepTheo = ConTroHienTai->Next;
        delete ConTroHienTai;
        ConTroHienTai = NodeTiepTheo;
    }
    Head = NULL;
}
// Lấy địa chỉ các bản sao vào mảng để phục vụ hiển thị
inline void LayDanhSachBanSao(
    const DauSach* DuLieuSach, const DanhMucSachNode* DanhSachKetQua[], 
    int& SoLuongKetQua, int SoPhanTuToiDa = 5000){
    SoLuongKetQua = 0;
    if (DuLieuSach == NULL || DanhSachKetQua == NULL || SoPhanTuToiDa <= 0) {
        return;
    }
    for (
        const DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead;
        ConTroHienTai != NULL && SoLuongKetQua < SoPhanTuToiDa;
        ConTroHienTai = ConTroHienTai->Next) {
        DanhSachKetQua[SoLuongKetQua] = ConTroHienTai;
        SoLuongKetQua++;
    }
}
// Lấy kệ của đầu sách từ bản sao đầu tiên
inline std::string LayKeDauSach(const DauSach* DuLieuSach) {
    if (DuLieuSach == NULL || DuLieuSach->DanhMucSachHead == NULL) {
        return "";
    }
    return std::string(DuLieuSach->DanhMucSachHead->ViTri);
}
// Cập nhật cùng một kệ cho toàn bộ bản sao của đầu sách
inline void CapNhatKeChoTatCaBanSao(DauSach* DuLieuSach, const std::string& KeMoi){
    if (DuLieuSach == NULL) {
        return;
    }
    std::string KeDaChuanHoa = ChuanHoaKe(KeMoi);
    if (KeDaChuanHoa.empty()){
        return;
    }
    for (
        DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead;
        ConTroHienTai != NULL;
        ConTroHienTai = ConTroHienTai->Next
        ){
        SaoChepChuoi(ConTroHienTai->ViTri, 20, KeDaChuanHoa);
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
// Tự động tạo bản sao cho các đầu sách trong danh sách
inline void TaoBanSaoTuDong(DauSach* DuLieuSach, int SoLuongCanXuLy, const std::string& KeNhap);
// Tự động xóa bản sao từ cuối danh sách cho các đầu sách trong danh sách
inline bool GiamBanSaoTuCuoi(DauSach* DuLieuSach, int SoLuongCanXoa);
// Kiểm tra danh sách đầu sách đã đạt số lượng tối đa hay chưa
inline bool KiemTraDanhSachDauSachDay(const DanhSachDauSach& DuLieuSach) {
    return DuLieuSach.SoLuong >= MaxDauSach;
}
// Tạo ISBN ngẫu nhiên và bảo đảm không trùng với ISBN đã có
inline std::string TaoISBNKhongTrung(const DanhSachDauSach& DanhSachDauSach) {
    static std::mt19937 Rng(static_cast<unsigned int>(std::time(NULL)));
    static std::uniform_int_distribution<int> Dist(100000000, 999999999);
    while (true) {
        int GiaTriNgauNhien = Dist(Rng);
        std::string ISBNMoi = std::to_string(GiaTriNgauNhien);
        if (TimDauSachTheoISBN(DanhSachDauSach, ISBNMoi.c_str()) == NULL) {
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
inline bool TachDauSachKhoiMang(DanhSachDauSach& DuLieuSach, DauSach* DauSachCanTach) {
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
    for (int i = ViTriCanTach; i < DuLieuSach.SoLuong - 1; i++) {
        DuLieuSach.Nodes[i] = DuLieuSach.Nodes[i + 1];
    }
    DuLieuSach.SoLuong--;
    DuLieuSach.Nodes[DuLieuSach.SoLuong] = NULL;
    return true;
}
// Xóa một bản sao cụ thể theo mã sách
inline bool XoaBanSaoTheoMaSach(DauSach* DuLieuSach, const std::string& MaSachCanXoa) {
    if (DuLieuSach == NULL) {
        return false;
    }
    DanhMucSachNode* ConTroTruoc = NULL;
    DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead;
    while (ConTroHienTai != NULL && std::strcmp(ConTroHienTai->MaSach, MaSachCanXoa.c_str()) != 0) {
        ConTroTruoc = ConTroHienTai;
        ConTroHienTai = ConTroHienTai->Next;
    }
    if (ConTroHienTai == NULL || ConTroHienTai->TrangThai == 1) {
        return false;
    }
    if (ConTroTruoc == NULL) {
        DuLieuSach->DanhMucSachHead = ConTroHienTai->Next;
    }
    else {
        ConTroTruoc->Next = ConTroHienTai->Next;
    }

    delete ConTroHienTai;
    DuLieuSach->SoLuongBanSao = DemTongSoBanSao(DuLieuSach);
    return true;
}
// Chỉ xóa ISBN khi đầu sách không còn bất kỳ bản sao nào
inline bool XoaDauSach(DanhSachDauSach& DuLieuSach, const std::string& ISBNCanXuLy) {
    int ChiSo = -1;
    for (int i = 0; i < DuLieuSach.SoLuong; i++) {
        if (DuLieuSach.Nodes[i] != NULL && std::strcmp(DuLieuSach.Nodes[i]->ISBN, ISBNCanXuLy.c_str()) == 0) {
            ChiSo = i;
            break;
        }
    }
    if (ChiSo == -1) {
        return false;
    }
    DauSach* DauSachCanXoa = DuLieuSach.Nodes[ChiSo];
    if (DemTongSoBanSao(DauSachCanXoa) > 0) {
        return false;
    }
    delete DauSachCanXoa;
    for (int i = ChiSo; i < DuLieuSach.SoLuong - 1; i++) {
        DuLieuSach.Nodes[i] = DuLieuSach.Nodes[i + 1];
    }
    DuLieuSach.SoLuong--;
    DuLieuSach.Nodes[DuLieuSach.SoLuong] = NULL;
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
    const std::string& KeNhap,
    int SoLuongBanSaoNhap,
    std::string* ThongBaoLoi = NULL
) {
    if (KiemTraDanhSachDauSachDay(DanhSachDauSach)) {
        return false;
    }
    std::string TenDaChuanHoa = ChuanHoaChuoi(TenSachNhap);
    std::string TacGiaDaChuanHoa = ChuanHoaChuoi(TacGiaNhap);
    std::string TheLoaiDaChuanHoa = ChuanHoaChuoi(TheLoaiNhap);
    std::string KeDaChuanHoa = ChuanHoaKe(KeNhap);
    if (!KiemTraThongTinDauSachNhap(
        TenDaChuanHoa, TacGiaDaChuanHoa, TheLoaiDaChuanHoa, KeDaChuanHoa,
        SoTrangNhap, NamXuatBanNhap, SoLuongBanSaoNhap, ThongBaoLoi
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
    DauSachMoi->ChiSoBanSaoLonNhat = 0;
    DauSachMoi->SoLuotMuon = 0;
    DauSachMoi->DanhMucSachHead = NULL;
    TaoBanSaoTuDong(DauSachMoi, SoLuongBanSaoNhap, KeDaChuanHoa);
    ChenDauSachTheoTen(DanhSachDauSach, DauSachMoi);
    return true;
}
// Lấy thông tin số bản sao trước khi xác nhận xóa đầu sách
inline bool LayThongTinDauSachDeXoa(
    const DanhSachDauSach& DanhSachDauSach,
    const std::string& ISBNCanXuLy,
    int& TongSoBanSao,
    int& SoSachDangMuon
) {
    DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy.c_str());
    if (DuLieuSach == NULL) {
        return false;
    }
    TongSoBanSao = DemTongSoBanSao(DuLieuSach);
    SoSachDangMuon = DemSoSachDangMuon(DuLieuSach);
    return true;
}

// ====================== QUẢN LÝ BẢN SAO ======================
// Tạo thêm các bản sao theo chỉ số lớn nhất đã từng được sử dụng
inline void TaoBanSaoTuDong(DauSach* DuLieuSach, int SoLuongCanXuLy, const std::string& KeNhap) {
    std::string KeDaChuanHoa = ChuanHoaKe(KeNhap);
    if (DuLieuSach == NULL || SoLuongCanXuLy <= 0 || KeDaChuanHoa.empty()) {
        return;
    }
    for (int i = 0; i < SoLuongCanXuLy; i++) {
        DuLieuSach->ChiSoBanSaoLonNhat++;
        DanhMucSachNode* NodeCanXuLy = new DanhMucSachNode();
        TaoMaSach(DuLieuSach->ISBN, DuLieuSach->ChiSoBanSaoLonNhat, NodeCanXuLy->MaSach, MaxMaSach);
        NodeCanXuLy->TrangThai = 0;
        SaoChepChuoi(NodeCanXuLy->ViTri, 20, KeDaChuanHoa);
        ThemSachVaoCuoiDanhMuc(DuLieuSach, NodeCanXuLy);
    }
}
// Giảm số bản sao từ cuối 
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
inline bool KiemTraSoLuongBanSaoMoi(const DauSach* DuLieuSach, int SoLuongMoi) {
    if (DuLieuSach == NULL) {
        return false;
    }
    if (SoLuongMoi < 1 || SoLuongMoi > 5000) {
        return false;
    }
    int SoSachDangMuon = DemSoSachDangMuon(DuLieuSach);
    if (SoLuongMoi < SoSachDangMuon) {
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
    const std::string& KeMoi,
    int CoThayDoiSoLuong,
    int SoLuongBanSaoMoi,
    std::string* ThongBaoLoi = NULL
) {
    DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy.c_str());
    if (DuLieuSach == NULL) {
        return false;
    }
    std::string TenDaChuanHoa = ChuanHoaChuoi(TenSachMoi);
    std::string TacGiaDaChuanHoa = ChuanHoaChuoi(TacGiaMoi);
    std::string KeDaChuanHoa = ChuanHoaKe(KeMoi);
    if (!KiemTraThongTinDauSachCapNhat(TenSachMoi, TacGiaMoi, NamXuatBanMoi, SoTrangMoi, KeMoi, ThongBaoLoi)) {
        return false;
    }
    if (CoThayDoiSoLuong == 1 && !KiemTraSoLuongBanSaoMoi(DuLieuSach, SoLuongBanSaoMoi)) {
        return false;
    }
    std::string KeDungDeTaoBanSao = KeDaChuanHoa;
    if (KeDungDeTaoBanSao.empty()) {
        KeDungDeTaoBanSao = LayKeDauSach(DuLieuSach);
    }
    // Cập nhật số lượng bản sao trước
    if (CoThayDoiSoLuong == 1) {
        if (SoLuongBanSaoMoi > DuLieuSach->SoLuongBanSao) {
            TaoBanSaoTuDong(DuLieuSach, SoLuongBanSaoMoi - DuLieuSach->SoLuongBanSao, KeDungDeTaoBanSao);
        }
        else if (SoLuongBanSaoMoi < DuLieuSach->SoLuongBanSao) {
            if (!GiamBanSaoTuCuoi(DuLieuSach, DuLieuSach->SoLuongBanSao - SoLuongBanSaoMoi)) {
                return false;
            }
        }
    }
    // Ghi nhận tên sách có thực sự thay đổi hay không
    bool CoThayDoiTen = false;
    if (!TenSachMoi.empty()) {
        CoThayDoiTen = std::strcmp(DuLieuSach->TenSach, TenDaChuanHoa.c_str()) != 0;

        SaoChepChuoi(DuLieuSach->TenSach, 100, TenDaChuanHoa);
    }
    if (!TacGiaMoi.empty()) {
        SaoChepChuoi(DuLieuSach->TacGia, 60, TacGiaDaChuanHoa);
    }
    if (NamXuatBanMoi != 0) {
        DuLieuSach->NamXuatBan = NamXuatBanMoi;
    }
    if (SoTrangMoi != 0) {
        DuLieuSach->SoTrang = SoTrangMoi;
    }
    if (!KeMoi.empty()) {
        CapNhatKeChoTatCaBanSao(DuLieuSach, KeDaChuanHoa);
    }
    // Nếu tên sách thay đổi thì tách khỏi vị trí cũ rồi chèn lại 
    if (CoThayDoiTen) {
        TachDauSachKhoiMang(DanhSachDauSach, DuLieuSach);
        ChenDauSachTheoTen(DanhSachDauSach, DuLieuSach);
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
// Kiểm tra đầu sách thứ nhất có đứng trước đầu sách thứ hai theo thứ tự thể loại và tên sách hay không
inline bool DauSachDungTruocTheoTheLoaiVaTen(const DauSach* DauSachThuNhat, const DauSach* DauSachThuHai) {
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
// Chèn một đầu sách vào mảng đang tăng dần theo thể loại và tên sách
inline void ChenDauSachTheoTheLoaiVaTen(DauSach* DanhSachDich[], int& SoPhanTu, DauSach* DauSachCanChen) {
    if (DanhSachDich == NULL || DauSachCanChen == NULL || SoPhanTu >= MaxDauSach) {
        return;
    }
    int ViTriChen = SoPhanTu;
    while (ViTriChen > 0 && DauSachDungTruocTheoTheLoaiVaTen(DauSachCanChen, DanhSachDich[ViTriChen - 1])) {
        DanhSachDich[ViTriChen] = DanhSachDich[ViTriChen - 1];
        ViTriChen--;
    }
    DanhSachDich[ViTriChen] = DauSachCanChen;
    SoPhanTu++;
}
// Lập danh sách đầu sách bằng cách lần lượt chèn từng đầu sách vào đúng vị trí
inline void LayDanhSachSapXepTheoTheLoai(const DanhSachDauSach& DanhSachNguon, DauSach* DanhSachDich[], int& SoPhanTu) {
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