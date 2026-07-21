#pragma once
#include <string>
#include <random>
#include <ctime>
#include "cautruc.h"
#include "dsdms.h"

// ==================== CÁC HÀM KIỂM TRA & TÌM KIẾM CƠ BẢN ======================
// Kiểm tra danh sách đầu sách đã đầy chưa
inline bool KiemTraDanhSachDauSachDay(const DanhSachDauSach& DuLieuSach) {
    return DuLieuSach.SoLuong >= MaxDauSach;
}
// Kiểm tra xem ISBN đã tồn tại trong danh sách chưa (trả về true/false)
inline bool KiemTraISBNTonTai(const DanhSachDauSach& DuLieuSach, const std::string& ISBNCanXuLy) {
    return TimDauSachTheoISBN(DuLieuSach, ISBNCanXuLy) != NULL;
}

// ================== NGHIỆP VỤ XỬ LÝ ISBN ====================
// Sinh mã ISBN tự động và duy nhất
// Thuật toán: Random -> Nếu trùng nhiều quá thì chuyển sang tìm Max + 1
inline std::string TaoISBNKhongTrung(const DanhSachDauSach& DanhSachDauSach) {
    static std::mt19937 Rng(static_cast<unsigned int>(std::time(NULL)));
    static std::uniform_int_distribution<int> Dist(100000000, 999999999);
    // Cách 1: Thử Random ngẫu nhiên (nhanh nếu dữ liệu ít)
    for (int i = 0; i < 5000; i++) {
        int X = Dist(Rng);
        std::string GiaTriDuKien = std::to_string(X);
        if (!KiemTraISBNTonTai(DanhSachDauSach, GiaTriDuKien)) {
            return GiaTriDuKien;
        }
    }
    // Cách 2: Nếu Random bị trùng quá nhiều (data đầy), tìm số lớn nhất hiện có + 1
    int GiaTriLonNhat = 100000000;
    for (int i = 0; i < DanhSachDauSach.SoLuong; i++) {
        DauSach* DuLieuSach = DanhSachDauSach.Nodes[i];
        if (DuLieuSach != NULL && std::strlen(DuLieuSach->ISBN) == 9 && KiemTraToanChuSo(DuLieuSach->ISBN)) {
            try {
                int GiaTriCanThem = std::stoi(DuLieuSach->ISBN);
                if (GiaTriCanThem > GiaTriLonNhat) {
                    GiaTriLonNhat = GiaTriCanThem;
                }
            }
            catch (...) {
            }
        }
    }
    if (GiaTriLonNhat < 999999999) {
        return std::to_string(GiaTriLonNhat + 1);
    }
    return "999999999";
}

// ===================== THÊM / XÓA ĐẦU SÁCH =====================
// Hàm tiện ích: Chuẩn hóa tên sách để so sánh (In hoa, bỏ dấu, cắt khoảng trắng)
inline std::string TaoKhoaTenSach(const DauSach* DuLieuSach) {
    return ChuyenThanhChuHoa(CatKhoangTrangHaiDau(DuLieuSach->TenSach));
}
// Thêm đầu sách mới vào danh sách (Chèn có sắp xếp - Insertion Sort logic)
// Giúp danh sách luôn tăng dần theo tên ngay khi thêm
inline bool ChenDauSachTheoTen(DanhSachDauSach& DuLieuSach, DauSach* ConTroHienTai) {
    if (KiemTraDanhSachDauSachDay(DuLieuSach)) {
        return false;
    }
    // 1. Tìm vị trí (k) cần chèn để đảm bảo thứ tự
    int k = 0;
    std::string KhoaCanChen = TaoKhoaTenSach(ConTroHienTai);
    while (k < DuLieuSach.SoLuong) {
        std::string KhoaHienTai = TaoKhoaTenSach(DuLieuSach.Nodes[k]);
        if (KhoaHienTai > KhoaCanChen) {
            break;
        } // Đã tìm thấy vị trí lớn hơn
        k++;
    }
    // 2. Dời các phần tử từ k về sau sang phải 1 bước
    for (int i = DuLieuSach.SoLuong; i > k; i--) {
        DuLieuSach.Nodes[i] = DuLieuSach.Nodes[i - 1];
    }
    // 3. Chèn phần tử mới vào vị trí k
    DuLieuSach.Nodes[k] = ConTroHienTai;
    DuLieuSach.SoLuong++;
    return true;
}
// Xóa đầu sách theo ISBN
// Lưu ý: Chỉ xóa được khi không có độc giả nào đang mượn sách này
inline bool XoaDauSach(DanhSachDauSach& DuLieuSach, const std::string& ISBNCanXuLy, std::string* ThongBaoLoi = NULL) {
    // 1. Tìm vị trí cần xóa
    int ChiSo = -1;
    for (int i = 0; i < DuLieuSach.SoLuong; i++) {
        if (DuLieuSach.Nodes[i]->ISBN == ISBNCanXuLy) {
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
    // 2. Kiểm tra ràng buộc: Sách đang được mượn thì không được xóa
    if (DemSoSachDangMuon(ConTroHienTai) > 0) {
        if (ThongBaoLoi) {
            *ThongBaoLoi = "Khong the xoa: dang co doc gia muon.";
        }
        return false;
    }
    // 3. Giải phóng bộ nhớ (Danh sách bản sao + Cấu trúc đầu sách)
    GiaiPhongDanhMucSach(ConTroHienTai->DanhMucSachHead);
    delete ConTroHienTai;
    // 4. Dời các phần tử phía sau về trước để lấp chỗ trống
    for (int i = ChiSo; i < DuLieuSach.SoLuong - 1; i++) {
        DuLieuSach.Nodes[i] = DuLieuSach.Nodes[i + 1];
    }
    DuLieuSach.SoLuong--; // Giảm số lượng phần tử
    return true;
}

// ====================== QUẢN LÝ BẢN SAO ======================
// Tự động tạo các bản sao cho đầu sách.
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
// Xóa bớt bản sao (ưu tiên xóa từ cuối lên, bỏ qua sách đang mượn)
inline bool GiamBanSaoTuCuoi(DauSach* DuLieuSach, int SoLuongCanXoa) {
    if (DuLieuSach == NULL || SoLuongCanXoa <= 0) {
        return true;
    }
    // 1. Đưa DSLK vào mảng tạm để dễ duyệt ngược
    DanhMucSachNode* Nodes[5000]; // Giới hạn 5000 bản sao
    int SoLuongTimThay = 0;
    for (DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead; ConTroHienTai != NULL;
        ConTroHienTai = ConTroHienTai->Next) {
        if (SoLuongTimThay < 5000) {
            Nodes[SoLuongTimThay++] = ConTroHienTai;
        }
    }
    int DaXoa = 0;
    // 2. Duyệt từ cuối về đầu
    for (int i = SoLuongTimThay - 1; i >= 0 && DaXoa < SoLuongCanXoa; --i) {
        DanhMucSachNode* ChiSoHienTai = Nodes[i];
        // Không xóa sách đang được mượn
        if (ChiSoHienTai->TrangThai == 1) {
            continue;
        }
        // Xử lý nối dây trước khi xóa
        if (ChiSoHienTai == DuLieuSach->DanhMucSachHead) {
            DuLieuSach->DanhMucSachHead = ChiSoHienTai->Next; // Xóa đầu
        }
        else {
            // Nối Node trước đó với Node nằm sau Node hiện tại.
            if (i > 0) {
                Nodes[i - 1]->Next = ChiSoHienTai->Next;
            }
        }
        delete ChiSoHienTai;
        DuLieuSach->SoLuongBanSao--;
        DaXoa++;
    }
    return (DaXoa == SoLuongCanXoa); // Trả về true nếu xóa đủ số lượng yêu cầu
}

// ====================  TÌM KIẾM THEO TÊN ====================
// Tìm kiếm tuyến tính theo tên sách
inline void TimDauSachTheoTen(const DanhSachDauSach& DuLieuSach,
    const std::string& TuKhoaGoc,
    DauSach* DanhSachKetQua[],
    int& SoLuongTimThay) {
    SoLuongTimThay = 0;
    std::string TuKhoaTimKiem = ChuyenThanhChuHoa(CatKhoangTrangHaiDau(TuKhoaGoc)); // Chuẩn hóa từ khóa
    if (TuKhoaTimKiem.empty()) {
        return;
    }
    for (int i = 0; i < DuLieuSach.SoLuong; i++) {
        std::string Key = TaoKhoaTenSach(DuLieuSach.Nodes[i]);
        // Kiểm tra chuỗi con (Substring search)
        if (Key.find(TuKhoaTimKiem) != std::string::npos) {
            DanhSachKetQua[SoLuongTimThay++] = DuLieuSach.Nodes[i];
        }
    }
}

// ===================== DỌN DẸP BỘ NHỚ =========================
// Giải phóng toàn bộ mảng đầu sách và các bản sao đi kèm
inline void GiaiPhongDanhSachDauSach(DanhSachDauSach& DuLieuSach) {
    for (int i = 0; i < DuLieuSach.SoLuong; i++) {
        if (DuLieuSach.Nodes[i] != NULL) {
            // Xóa DSLK bản sao trước
            GiaiPhongDanhMucSach(DuLieuSach.Nodes[i]->DanhMucSachHead);
            // Xóa cấu trúc đầu sách
            delete DuLieuSach.Nodes[i];
            DuLieuSach.Nodes[i] = NULL;
        }
    }
    DuLieuSach.SoLuong = 0;
}

// ================ THUẬT TOÁN SẮP XẾP (QUICK SORT) ==================
// Hàm đổi chỗ hai con trỏ đầu sách
inline void HoanDoiDauSach(DauSach*& GiaTriThuNhat, DauSach*& GiaTriThuHai) {
    DauSach* Temp = GiaTriThuNhat;
    GiaTriThuNhat = GiaTriThuHai;
    GiaTriThuHai = Temp;
}
// --- 1. Sắp xếp theo TÊN SÁCH (A -> Z) ---
inline int PhanHoachTheoTenSach(DauSach* MangDuLieu[],int ChiSoTrai,int ChiSoPhai){
    std::string GiaTriChot = TaoKhoaTenSach(MangDuLieu[ChiSoPhai]);
    int i = ChiSoTrai - 1;
    for (int j = ChiSoTrai;j < ChiSoPhai;j++){
        if (TaoKhoaTenSach(MangDuLieu[j]) < GiaTriChot){
            i++;
            HoanDoiDauSach(MangDuLieu[i],MangDuLieu[j]);
        }
    }
    HoanDoiDauSach(MangDuLieu[i + 1],MangDuLieu[ChiSoPhai]);
    return i + 1;
}
// Hàm chính Quick Sort theo TÊN SÁCH
inline void QuickSortTheoTenSach(DauSach* MangDuLieu[], int ChiSoTrai, int ChiSoPhai) {
    if (ChiSoTrai < ChiSoPhai) {
        int ViTriPhanHoach = PhanHoachTheoTenSach(MangDuLieu, ChiSoTrai, ChiSoPhai);
        QuickSortTheoTenSach(MangDuLieu, ChiSoTrai, ViTriPhanHoach - 1);
        QuickSortTheoTenSach(MangDuLieu, ViTriPhanHoach + 1, ChiSoPhai);
    }
}
// --- 2. Sắp xếp theo THỂ LOẠI (Ưu tiên Thể loại -> Tên sách) ---
inline int PhanHoachTheoTheLoai(DauSach* MangDuLieu[], int ChiSoTrai, int ChiSoPhai) {
    DauSach* ConTroHienTai = MangDuLieu[ChiSoPhai];
    int i = ChiSoTrai - 1;
    for (int j = ChiSoTrai; j < ChiSoPhai; j++) {
        bool Condition = false;
        if (std::strcmp(MangDuLieu[j]->TheLoai, ConTroHienTai->TheLoai) < 0) {
            Condition = true;
        }
        else if (std::strcmp(MangDuLieu[j]->TheLoai, ConTroHienTai->TheLoai) == 0 &&
            std::strcmp(MangDuLieu[j]->TenSach, ConTroHienTai->TenSach) < 0) {
            Condition = true;
        }
        if (Condition) {
            i++;
            HoanDoiDauSach(MangDuLieu[i], MangDuLieu[j]);
        }
    }
    HoanDoiDauSach(MangDuLieu[i + 1], MangDuLieu[ChiSoPhai]);
    return i + 1;
}
// Hàm chính Quick Sort theo THỂ LOẠI
inline void QuickSortTheoTheLoai(DauSach* MangDuLieu[], int ChiSoTrai, int ChiSoPhai) {
    if (ChiSoTrai < ChiSoPhai) {
        int ViTriPhanHoach = PhanHoachTheoTheLoai(MangDuLieu, ChiSoTrai, ChiSoPhai);
        QuickSortTheoTheLoai(MangDuLieu, ChiSoTrai, ViTriPhanHoach - 1);
        QuickSortTheoTheLoai(MangDuLieu, ViTriPhanHoach + 1, ChiSoPhai);
    }
}
// Sao chép dữ liệu từ danh sách chính sang mảng tạm rồi sắp xếp
inline void LayDanhSachSapXepTheoTheLoai(const DanhSachDauSach& DanhSachNguon, DauSach* DanhSachDich[], int& SoPhanTu) {
    SoPhanTu = DanhSachNguon.SoLuong;
    // Copy con trỏ
    for (int i = 0; i < SoPhanTu; i++) {
        DanhSachDich[i] = DanhSachNguon.Nodes[i];
    }
    // Sắp xếp trên mảng copy
    if (SoPhanTu > 0) {
        QuickSortTheoTheLoai(DanhSachDich, 0, SoPhanTu - 1);
    }
}
