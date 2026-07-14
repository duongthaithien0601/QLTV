#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif
#pragma once
#include <string>
#include <vector>
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

//====================== NGAY ======================

struct NgayThangNam {
    int Ngay;
    int Thang;
    int Nam;
};

//====================== DANH MUC SACH ======================

struct DanhMucSachNode {
    char MaSach[MaxMaSach];
    int TrangThai;               // 0: Cho muon, 1: Da muon, 
    char ViTri[50];

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

    NgayThangNam NgayMuon;
    NgayThangNam NgayTra;

    int TrangThai;    // 0: Dang muon    // 1: Da tra   

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
    char Phai[5];               // "Nam" hoac "Nu"

    int TrangThaiThe;    // 0: Khoa    // 1: Hoat dong

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

//====================== THONG KE QUA HAN ======================

struct ThongKeQuaHan {
    int MaThe;

    char HoTen[100];
    char MaSach[MaxMaSach];
    char ISBN[15];
    char TenSach[100];

    NgayThangNam NgayMuon;

    int SoNgayTre;

    ThongKeQuaHan() {
        MaThe = 0;
        SoNgayTre = 0;
    }
};

// ======================= HÀM TIỆN ÍCH =======================
// Sao chep std::string vao mang char an toan.
inline void SaoChepChuoi(char ChuoiDich[], int KichThuoc, const std::string& ChuoiNguon) {
    if (ChuoiDich == NULL || KichThuoc <= 0) {
        return;
    }

    std::strncpy(ChuoiDich, ChuoiNguon.c_str(), static_cast<size_t>(KichThuoc - 1));
    ChuoiDich[KichThuoc - 1] = '\0';
}

// Cắt khoảng trắng đầu
inline std::string CatKhoangTrangDau(const std::string& ChuoiNhap) {
    size_t i = 0;
    while (i < ChuoiNhap.size() && std::isspace(static_cast<unsigned char>(ChuoiNhap[i])) != 0) {
        i++;
    }
    return ChuoiNhap.substr(i);
}
// Cắt khoảng trắng đuôi
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
// Cắt 2 đầu
inline std::string CatKhoangTrangHaiDau(const std::string& ChuoiNhap) {
    return CatKhoangTrangCuoi(CatKhoangTrangDau(ChuoiNhap));
}
// Hàm nén khoảng trắng
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
// Hàm chuyển chữ hoa
inline std::string ChuyenThanhChuHoa(std::string ChuoiNhap) {
    std::transform(ChuoiNhap.begin(), ChuoiNhap.end(), ChuoiNhap.begin(), [](unsigned char KyTuDoc) {
        return std::toupper(KyTuDoc);
        });
    return ChuoiNhap;
}
// Hàm chuẩn hóa tổng hợp (Sửa lại để kết hợp 2 hàm trên)
inline std::string ChuanHoaChuoi(const std::string& ChuoiNhap) {
    std::string ChuoiDaRutGon = RutGonKhoangTrang(ChuoiNhap);
    return ChuyenThanhChuHoa(ChuoiDaRutGon);
}
// ----------------- Ngày tháng -----------------
// Kiểm tra tính hợp lệ của ngày tháng
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
// Phân tích chuỗi thành ngày tháng định dạng dd/mm/yyyy
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
// Phân tích chuỗi thành ngày tháng định dạng dd/mm/yyyy.
inline NgayThangNam PhanTichNgayDDMMYYYY(const std::string& ChuoiNhap) {
    NgayThangNam NgayCanXuLy{ 0, 0, 0 };
    (void)PhanTichNgayDDMMYYYY(ChuoiNhap, NgayCanXuLy);
    return NgayCanXuLy;
}
// Lấy ngày hiện tại
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
// Tính số ngày giữa 2 ngày
inline int TinhKhoangCachNgay(const NgayThangNam& GiaTriThuNhat, const NgayThangNam& GiaTriThuHai) {
    if (!KiemTraNgayHopLe(GiaTriThuNhat) || !KiemTraNgayHopLe(GiaTriThuHai)) {
        return 0;
    }
    return ChuyenNgayThanhSoThuTu(GiaTriThuHai) - ChuyenNgayThanhSoThuTu(GiaTriThuNhat);
}
// Tính số ngày từ a đến b (b - a)
inline int TinhSoNgayChenhLech(const NgayThangNam& GiaTriThuHai, const NgayThangNam& GiaTriThuNhat) {
    return TinhKhoangCachNgay(GiaTriThuNhat, GiaTriThuHai);
}

// ----------------- Validate -----------------
// Kiểm tra tên (không chứa số và ký tự đặc biệt)
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
// Kiểm tra chuỗi toàn chữ số
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
// Kiểm tra chuỗi toàn chữ cái
inline bool KiemTraToanChuCai(const std::string& ChuoiNhap) {
    if (ChuoiNhap.empty()) {
        return false;
    }
    for (char KyTuDoc : ChuoiNhap) {
        if (!std::isalpha(static_cast<unsigned char>(KyTuDoc))) {
            return false;
        }
    }
    return true;
}
// ----------------- Mã sách / ISBN -----------------
// Lấy ISBN từ mã sách
inline std::string LayISBNTuMaSach(const std::string& MaSach) {
    size_t ViTri = MaSach.find('-');
    if (ViTri == std::string::npos) {
        return MaSach;
    }
    return MaSach.substr(0, ViTri);
}
// Tạo mã sách từ ISBN và chỉ số
inline std::string TaoMaSach(const std::string& ISBNCanXuLy, int ChiSo) {
    std::ostringstream Oss;
    Oss << CatKhoangTrangHaiDau(ISBNCanXuLy) << "-" << ChiSo;
    return Oss.str();
}
