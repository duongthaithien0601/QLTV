#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif
#pragma once
#include <string>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <sstream>
#include <cstring>
#include <cstdio>

#define MaxDocGia 20000
#define MaxDauSach 1000
#define MaxMaSach 20

const int HanMuonNgay = 7;

//====================== NGAY THANG NAM ======================
struct NgayThangNam {
    int Ngay;
    int Thang;
    int Nam;
};

//====================== DANH MUC SACH ======================
struct DanhMucSachNode {
    char MaSach[MaxMaSach];
    int TrangThai;

    DanhMucSachNode* Next;

    DanhMucSachNode() {
        TrangThai = 0;
        Next = NULL;
    }
};

//====================== DAU SACH ======================
struct DauSach {
    char ISBN[15];
    char TenSach[100];
    char TacGia[60];
    char TheLoai[40];
    int SoTrang;
    int NamXuatBan;
    int SoLuongBanSao;
    int SoLuotMuon;

    DanhMucSachNode* DanhMucSachHead;

    DauSach() {
        SoTrang = 0;
        NamXuatBan = 0;
        SoLuongBanSao = 0;
        SoLuotMuon = 0;
        DanhMucSachHead = NULL;
    }
};

//====================== DANH SACH DAU SACH ======================
struct DanhSachDauSach {
    DauSach* Nodes[MaxDauSach];
    int SoLuong;

    DanhSachDauSach() {
        SoLuong = 0;
        for (int i = 0; i < MaxDauSach; i++)
            Nodes[i] = NULL;
    }
};

//====================== MUON TRA ======================
struct MuonTraNode {
    char MaSach[MaxMaSach];
    int TrangThai;

    NgayThangNam NgayMuon;
    NgayThangNam NgayTra;
    MuonTraNode* Next;

    MuonTraNode() {
        TrangThai = 0;
        Next = NULL;
    }
};

//====================== DOC GIA ======================
struct DocGia {
    int MaThe;
    char Ho[50];
    char Ten[30];
    char Phai[5];
    int TrangThaiThe;

    MuonTraNode* MuonTraHead;

    DocGia() {
        MaThe = 0;
        TrangThaiThe = 1;
        MuonTraHead = NULL;
    }
};

//====================== CAY DOC GIA ======================
struct DocGiaNode {
    DocGia ThongTin;
    DocGiaNode* Left;
    DocGiaNode* Right;

    DocGiaNode(const DocGia& X) {
        ThongTin = X;
        Left = NULL;
        Right = NULL;
    }
};

// ======================= XỬ LÝ CHUỖI DÙNG CHUNG =======================
// Sao chép chuỗi std::string vào mảng char an toàn
inline void SaoChepChuoi(char ChuoiDich[], int KichThuoc, const std::string& ChuoiNguon) {
    if (ChuoiDich == NULL || KichThuoc <= 0) {
        return;
    }

    std::strncpy(ChuoiDich, ChuoiNguon.c_str(), static_cast<size_t>(KichThuoc - 1));
    ChuoiDich[KichThuoc - 1] = '\0';
}
// Sao chép mảng char vào mảng char an toàn
inline void SaoChepChuoi(char ChuoiDich[], int KichThuoc, const char ChuoiNguon[]) {
    if (ChuoiDich == NULL || ChuoiNguon == NULL || KichThuoc <= 0) {
        return;
    }
    std::strncpy(ChuoiDich, ChuoiNguon, static_cast<size_t>(KichThuoc - 1)
    );
    ChuoiDich[KichThuoc - 1] = '\0';
}
// Loại bỏ các khoảng trắng ở đầu chuỗi
inline std::string CatKhoangTrangDau(const std::string& ChuoiNhap) {
    size_t i = 0;
    while (i < ChuoiNhap.size() && std::isspace(static_cast<unsigned char>(ChuoiNhap[i])) != 0) {
        i++;
    }
    return ChuoiNhap.substr(i);
}
// Loại bỏ các khoảng trắng ở cuối chuỗi
inline std::string CatKhoangTrangCuoi(const std::string& ChuoiNhap) {
    if (ChuoiNhap.empty()) {
        return ChuoiNhap;
    }
    size_t i = ChuoiNhap.size();
    while (i > 0 && std::isspace(static_cast<unsigned char>(ChuoiNhap[i - 1])) != 0) {
        i--;
    }
    return ChuoiNhap.substr(0, i);
}
// Loại bỏ các khoảng trắng ở hai đầu chuỗi
inline std::string CatKhoangTrangHaiDau(const std::string& ChuoiNhap) {
    return CatKhoangTrangCuoi(CatKhoangTrangDau(ChuoiNhap));
}
// Rút gọn nhiều khoảng trắng liên tiếp thành một khoảng trắng
inline std::string RutGonKhoangTrang(const std::string& ChuoiNhap) {
    std::string KetQua;
    bool KyTuTruocLaKhoangTrang = true;
    for (size_t i = 0; i < ChuoiNhap.length(); i++) {
        if (std::isspace(static_cast<unsigned char>(ChuoiNhap[i]))) {
            if (!KyTuTruocLaKhoangTrang) {
                KetQua += ' ';
                KyTuTruocLaKhoangTrang = true;
            }
        }
        else {
            KetQua += ChuoiNhap[i];
            KyTuTruocLaKhoangTrang = false;
        }
    }
    if (!KetQua.empty() && KetQua.back() == ' ') {
        KetQua.pop_back();
    }
    return KetQua;
}
// Chuyển toàn bộ ký tự trong chuỗi thành chữ hoa
inline std::string ChuyenThanhChuHoa(std::string ChuoiNhap) {
    std::transform(ChuoiNhap.begin(), ChuoiNhap.end(), ChuoiNhap.begin(), [](unsigned char KyTuDoc) {
        return std::toupper(KyTuDoc);
        });
    return ChuoiNhap;
}
// Chuẩn hóa chuỗi bằng cách rút gọn khoảng trắng và chuyển thành chữ hoa
inline std::string ChuanHoaChuoi(const std::string& ChuoiNhap) {
    std::string ChuoiDaRutGon = RutGonKhoangTrang(ChuoiNhap);
    return ChuyenThanhChuHoa(ChuoiDaRutGon);
}
// ======================= XỬ LÝ NGÀY THÁNG =======================
// Kiểm tra ngày, tháng và năm có tạo thành một ngày hợp lệ hay không
inline bool KiemTraNgayHopLe(const NgayThangNam& GiaTriThuNhat) {
    if (GiaTriThuNhat.Nam <= 0 || GiaTriThuNhat.Thang < 1 || GiaTriThuNhat.Thang > 12 || GiaTriThuNhat.Ngay <= 0) {
        return false;
    }
    static const int SoNgayTrongThang[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (GiaTriThuNhat.Thang == 2) {
        int SoNgayThangHai = 28;
        if ((GiaTriThuNhat.Nam % 400 == 0) || (GiaTriThuNhat.Nam % 4 == 0 && GiaTriThuNhat.Nam % 100 != 0)) {
            SoNgayThangHai = 29;
        }
        return GiaTriThuNhat.Ngay <= SoNgayThangHai;
    }
    return GiaTriThuNhat.Ngay <= SoNgayTrongThang[GiaTriThuNhat.Thang];
}
// Chuyển chuỗi định dạng dd/mm/yyyy thành dữ liệu ngày tháng
inline bool PhanTichNgayDDMMYYYY(const std::string& ChuoiNhap, NgayThangNam& KetQuaDauRa) {
    int NgayCanXuLy, M, Y;
    char DauPhanCachThuNhat, DauPhanCachThuHai;
    if (std::sscanf(ChuoiNhap.c_str(), "%d%c%d%c%d", &NgayCanXuLy, &DauPhanCachThuNhat, &M, &DauPhanCachThuHai, &Y) ==
        5) {
        if (DauPhanCachThuNhat == '/' && DauPhanCachThuHai == '/') {
            KetQuaDauRa.Ngay = NgayCanXuLy;
            KetQuaDauRa.Thang = M;
            KetQuaDauRa.Nam = Y;
            return KiemTraNgayHopLe(KetQuaDauRa);
        }
    }
    return false;
}
// Chuyển chuỗi định dạng dd/mm/yyyy thành dữ liệu ngày tháng
inline NgayThangNam PhanTichNgayDDMMYYYY(const std::string& ChuoiNhap) {
    NgayThangNam NgayCanXuLy{ 0, 0, 0 };
    (void)PhanTichNgayDDMMYYYY(ChuoiNhap, NgayCanXuLy);
    return NgayCanXuLy;
}
// Chuyển dữ liệu ngày tháng thành chuỗi định dạng dd/mm/yyyy
inline std::string ChuyenNgayThanhChuoi(const NgayThangNam& NgayCanXuLy) {
    if (!KiemTraNgayHopLe(NgayCanXuLy)) {
        return "0/0/0";
    }
    char BoDem[16];
    std::snprintf(
        BoDem,
        sizeof(BoDem),
        "%02d/%02d/%04d",
        NgayCanXuLy.Ngay,
        NgayCanXuLy.Thang,
        NgayCanXuLy.Nam
    );

    return std::string(BoDem);
}
// Chuyển ngày tháng thành số thứ tự để phục vụ so sánh và tính khoảng cách
inline int ChuyenNgayThanhSoThuTu(const NgayThangNam& GiaTriThuNhat) {
    int Y = GiaTriThuNhat.Nam;
    int M = GiaTriThuNhat.Thang;
    int NgayCanXuLy = GiaTriThuNhat.Ngay;
    if (M < 3) {
        Y -= 1;
        M += 12;
    }
    return 365 * Y + Y / 4 - Y / 100 + Y / 400 + (153 * M - 457) / 5 + NgayCanXuLy - 306;
}
// Tính số ngày chênh lệch giữa hai mốc thời gian
inline int TinhSoNgayChenhLech(const NgayThangNam& NgaySau, const NgayThangNam& NgayTruoc) {
    if (!KiemTraNgayHopLe(NgaySau) || !KiemTraNgayHopLe(NgayTruoc)) {
        return 0;
    }

    return ChuyenNgayThanhSoThuTu(NgaySau) - ChuyenNgayThanhSoThuTu(NgayTruoc);
}
// Tính số ngày quá hạn dựa trên thời hạn mượn quy định
inline int TinhSoNgayTre(const NgayThangNam& NgayMuon, const NgayThangNam& NgayKiemTra) {
    int TongSoNgay = TinhSoNgayChenhLech(NgayKiemTra, NgayMuon);
    if (TongSoNgay <= HanMuonNgay) {
        return 0;
    }
    return TongSoNgay - HanMuonNgay;
}
// Lấy ngày hiện tại từ hệ thống
inline NgayThangNam LayNgayHienTai() {
    std::time_t ThoiGianHienTai = std::time(NULL);
    std::tm ThoiGianDiaPhuong{};
#ifdef _WIN32
    localtime_s(
        &ThoiGianDiaPhuong,
        &ThoiGianHienTai
    );
#else
    std::tm* ConTroThoiGian =
        std::localtime(&ThoiGianHienTai);

    if (ConTroThoiGian != NULL)
    {
        ThoiGianDiaPhuong = *ConTroThoiGian;
    }
#endif
    NgayThangNam KetQua;
    KetQua.Ngay = ThoiGianDiaPhuong.tm_mday;
    KetQua.Thang = ThoiGianDiaPhuong.tm_mon + 1;
    KetQua.Nam = ThoiGianDiaPhuong.tm_year + 1900;
    return KetQua;
}
// So sánh hai ngày và trả về kết quả nhỏ hơn, bằng hoặc lớn hơn
inline int SoSanhNgay(const NgayThangNam& GiaTriThuNhat, const NgayThangNam& GiaTriThuHai) {
    int SoThuTuThuNhat = ChuyenNgayThanhSoThuTu(GiaTriThuNhat);
    int SoThuTuThuHai = ChuyenNgayThanhSoThuTu(GiaTriThuHai);
    if (SoThuTuThuNhat < SoThuTuThuHai) {
        return -1;
    }
    if (SoThuTuThuNhat > SoThuTuThuHai) {
        return 1;
    }
    return 0;
}
// ======================= KIỂM TRA DỮ LIỆU DÙNG CHUNG =======================
// Kiểm tra chuỗi tên không chứa chữ số hoặc ký tự đặc biệt
inline bool KiemTraTenHopLe(const std::string& ChuoiNhap) {
    for (unsigned char KyTuDoc : ChuoiNhap) {
        if (std::isdigit(KyTuDoc)) {
            return false;
        }
        if (std::ispunct(KyTuDoc)) {
            return false;
        }
    }
    return true;
}
// Kiểm tra chuỗi chỉ chứa các chữ số
inline bool KiemTraToanChuSo(const std::string& ChuoiNhap) {
    if (ChuoiNhap.empty()) {
        return false;
    }
    for (char KyTuDoc : ChuoiNhap) {
        if (!std::isdigit(static_cast<unsigned char>(KyTuDoc))) {
            return false;
        }
    }
    return true;
}
// Kiểm tra một số nguyên có nằm trong khoảng cho phép hay không
inline bool KiemTraSoTrongKhoang(int GiaTri, int GiaTriNhoNhat, int GiaTriLonNhat) {
    return GiaTri >= GiaTriNhoNhat && GiaTri <= GiaTriLonNhat;
}
// Kiểm tra ngày hợp lệ và không lớn hơn ngày hiện tại
inline bool KiemTraNgayKhongVuotQuaHienTai(const NgayThangNam& NgayCanKiemTra) {
    return KiemTraNgayHopLe(NgayCanKiemTra) &&
        NgayCanKiemTra.Nam >= 1500 &&
        SoSanhNgay(NgayCanKiemTra, LayNgayHienTai()) <= 0;
}
// Kiểm tra toàn bộ dữ liệu nhập khi thêm đầu sách mới
inline bool KiemTraThongTinDauSachNhap(
    const std::string& TenSach,
    const std::string& TacGia,
    const std::string& TheLoai,
    int SoTrang,
    int NamXuatBan,
    int SoLuongBanSao,
    std::string* ThongBaoLoi = NULL
) {
    if (TenSach.empty()) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Ten sach khong hop le.";
        }
        return false;
    }
    if (TacGia.empty() || !KiemTraTenHopLe(TacGia)) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Tac gia khong hop le.";
        }
        return false;
    }
    if (TheLoai.empty() || !KiemTraTenHopLe(TheLoai)) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "The loai khong hop le.";
        }
        return false;
    }
    if (!KiemTraSoTrongKhoang(SoTrang, 1, 5000)) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "So trang phai tu 1 den 5000.";
        }
        return false;
    }
    if (!KiemTraSoTrongKhoang(NamXuatBan, 1500, LayNgayHienTai().Nam)) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Nam xuat ban khong hop le.";
        }
        return false;
    }
    if (!KiemTraSoTrongKhoang(SoLuongBanSao, 1, 5000)) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "So luong ban sao phai tu 1 den 5000.";
        }
        return false;
    }
    return true;
}
// Kiểm tra toàn bộ dữ liệu nhập khi thêm độc giả mới
inline bool KiemTraThongTinDocGiaNhap(
    const std::string& Ho,
    const std::string& Ten,
    const std::string& Phai,
    int TrangThaiThe,
    std::string* ThongBaoLoi = NULL
) {
    if (Ho.empty() || !KiemTraTenHopLe(Ho)) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Ho va ten dem khong hop le.";
        }
        return false;
    }
    if (Ten.empty() || !KiemTraTenHopLe(Ten)) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Ten khong hop le.";
        }
        return false;
    }
    if (Phai != "Nam" && Phai != "Nu") {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Gioi tinh khong hop le.";
        }
        return false;
    }
    if (TrangThaiThe != 0 && TrangThaiThe != 1) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Trang thai the khong hop le.";
        }
        return false;
    }
    return true;
}
// Kiểm tra các dữ liệu được nhập khi cập nhật đầu sách
inline bool KiemTraThongTinDauSachCapNhat(
    const std::string& TenSach,
    const std::string& TacGia,
    int NamXuatBan,
    int SoTrang,
    std::string* ThongBaoLoi = NULL
) {
    if (!TenSach.empty() && ChuanHoaChuoi(TenSach).empty()) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Ten sach khong hop le.";
        }
        return false;
    }
    if (!TacGia.empty() && !KiemTraTenHopLe(ChuanHoaChuoi(TacGia))) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Tac gia khong hop le.";
        }
        return false;
    }
    if (NamXuatBan != 0 && !KiemTraSoTrongKhoang(NamXuatBan, 1500, LayNgayHienTai().Nam)) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Nam xuat ban khong hop le.";
        }
        return false;
    }
    if (SoTrang != 0 && !KiemTraSoTrongKhoang(SoTrang, 1, 5000)) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "So trang phai tu 1 den 5000.";
        }
        return false;
    }
    return true;
}
// Kiểm tra các dữ liệu được nhập khi cập nhật độc giả
inline bool KiemTraThongTinDocGiaCapNhat(
    const std::string& Ho,
    const std::string& Ten,
    const std::string& Phai,
    int TrangThaiThe,
    std::string* ThongBaoLoi = NULL
) {
    if (!Ho.empty() && !KiemTraTenHopLe(ChuanHoaChuoi(Ho))) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Ho va ten dem khong hop le.";
        }
        return false;
    }
    if (!Ten.empty() && !KiemTraTenHopLe(ChuanHoaChuoi(Ten))) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Ten khong hop le.";
        }
        return false;
    }
    if (Phai != "Nam" && Phai != "Nu") {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Gioi tinh khong hop le.";
        }
        return false;
    }
    if (TrangThaiThe != 0 && TrangThaiThe != 1) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Trang thai the khong hop le.";
        }
        return false;
    }
    return true;
}
