#pragma once
#include <string>
#include <random>
#include <ctime>
#include "cautruc.h"

// =================== CÁC HÀM TÌM KIẾM TRÊN CÂY ===================
// Tìm kiếm độc giả theo Mã thẻ (Binary Search)
inline DocGiaNode* TimDocGiaTheoMaThe(DocGiaNode* Root, int MaTheCanXuLy) {
    DocGiaNode* ConTroHienTai = Root;
    while (ConTroHienTai != NULL) {
        if (MaTheCanXuLy < ConTroHienTai->ThongTin.MaThe) {
            ConTroHienTai = ConTroHienTai->Left; // Nhỏ hơn -> Sang trái
        }
        else if (MaTheCanXuLy > ConTroHienTai->ThongTin.MaThe) {
            ConTroHienTai = ConTroHienTai->Right; // Lớn hơn -> Sang phải
        }
        else {
            return ConTroHienTai; // Tìm thấy
        }
    }
    return NULL; // Không tìm thấy
}
// Kiểm tra xem mã thẻ đã tồn tại hay chưa
inline bool KiemTraMaTheTonTai(DocGiaNode* Root, int MaTheCanXuLy) {
    return TimDocGiaTheoMaThe(Root, MaTheCanXuLy) != NULL;
}
// Tìm node có giá trị nhỏ nhất trong cây con (Dùng khi xóa node có 2 con)
inline DocGiaNode* TimNodeNhoNhat(DocGiaNode* Root) {
    if (Root == NULL) {
        return NULL;
    }
    while (Root->Left != NULL) {
        Root = Root->Left;
    }
    return Root;
}

// =================== THAY ĐỔI CẤU TRÚC CÂY (THÊM / XÓA NODE) =====================
// Chèn một độc giả mới vào cây (BST Insertion)
inline void ThemDocGiaVaoCay(DocGiaNode*& Root, const DocGia& GiaTriCanThem) {
    if (Root == NULL) {
        Root = new DocGiaNode(GiaTriCanThem);
        return;
    }
    DocGiaNode* ConTroHienTai = Root;
    DocGiaNode* Parent = NULL;
    // Tìm vị trí lá (leaf) phù hợp
    while (ConTroHienTai != NULL) {
        Parent = ConTroHienTai;
        if (GiaTriCanThem.MaThe < ConTroHienTai->ThongTin.MaThe) {
            ConTroHienTai = ConTroHienTai->Left;
        }
        else if (GiaTriCanThem.MaThe > ConTroHienTai->ThongTin.MaThe) {
            ConTroHienTai = ConTroHienTai->Right;
        }
        else {
            // Trùng mã thẻ -> Không thêm để bảo toàn dữ liệu cũ
            return;
        }
    }
    // Gắn node mới vào cha
    if (GiaTriCanThem.MaThe < Parent->ThongTin.MaThe) {
        Parent->Left = new DocGiaNode(GiaTriCanThem);
    }
    else {
        Parent->Right = new DocGiaNode(GiaTriCanThem);
    }
}
// Hàm đệ quy xóa node theo Mã thẻ (BST Deletion)
inline void XoaNodeDocGia(DocGiaNode*& Root, int MaTheCanXuLy) {
    if (Root == NULL) {
        return;
    }

    if (MaTheCanXuLy < Root->ThongTin.MaThe) {
        XoaNodeDocGia(Root->Left, MaTheCanXuLy);
    }
    else if (MaTheCanXuLy > Root->ThongTin.MaThe) {
        XoaNodeDocGia(Root->Right, MaTheCanXuLy);
    }
    else {
        // Đã tìm thấy node cần xóa (root hiện tại)
        DocGiaNode* NodeCanXoa = Root;
        // TH1: Node có 1 con hoặc không có con
        if (Root->Left == NULL) {
            Root = Root->Right;
            delete NodeCanXoa;
        }
        else if (Root->Right == NULL) {
            Root = Root->Left;
            delete NodeCanXoa;
        }
        // TH2: Node có 2 con
        else {
            // Tìm phần tử thay thế (nhỏ nhất bên phải)
            DocGiaNode* NodeTheMang = TimNodeNhoNhat(Root->Right);
            // Sao chép giá trị
            Root->ThongTin = NodeTheMang->ThongTin;
            // Đệ quy xóa phần tử thay thế đó
            XoaNodeDocGia(Root->Right, NodeTheMang->ThongTin.MaThe);
        }
    }
}

// ==================== TIỆN ÍCH & LOGIC NGHIỆP VỤ ====================
// Đếm số lượng sách độc giả đang mượn (để kiểm tra điều kiện xóa)
inline int DemSoSachDocGiaDangMuon(const DocGia& DocGiaCanXuLy) {
    int SoLuongDem = 0;
    for (MuonTraNode* ConTroHienTai = DocGiaCanXuLy.MuonTraHead; ConTroHienTai != NULL;
        ConTroHienTai = ConTroHienTai->Next) {
        if (ConTroHienTai->TrangThai == 0) {
            SoLuongDem++;
        }
    }
    return SoLuongDem;
}
// Xóa độc giả an toàn (Chỉ xóa nếu không còn giữ sách thư viện)
inline bool XoaDocGiaNeuKhongMuonSach(DocGiaNode*& Root, int MaTheCanXuLy) {
    DocGiaNode* ConTroHienTai = TimDocGiaTheoMaThe(Root, MaTheCanXuLy);
    if (ConTroHienTai == NULL) {
        return false;
    } // Không tồn tại
    // Nếu đang mượn sách -> Từ chối xóa
    if (DemSoSachDocGiaDangMuon(ConTroHienTai->ThongTin) > 0) {
        return false;
    }
    // Thực hiện xóa
    XoaNodeDocGia(Root, MaTheCanXuLy);
    return true;
}
// Sinh mã thẻ ngẫu nhiên và đảm bảo duy nhất
inline int TaoMaTheKhongTrung(DocGiaNode* Root) {
    static std::mt19937 Rng(static_cast<unsigned>(std::time(NULL)));
    std::uniform_int_distribution<int> Dist(100000, 999999);
    // Thử random 2048 lần
    for (int i = 0; i < 2048; i++) {
        int GiaTriDuKien = Dist(Rng);
        if (!KiemTraMaTheTonTai(Root, GiaTriDuKien)) {
            return GiaTriDuKien;
        }
    }
    for (int GiaTriDuKien = 100000; GiaTriDuKien <= 999999; GiaTriDuKien++) {
        if (!KiemTraMaTheTonTai(Root, GiaTriDuKien)) {
            return GiaTriDuKien;
        }
    }
    return 999999;
}
// Đếm tổng số node (số lượng độc giả) trên cây
inline int DemTongDocGia(DocGiaNode* Root) {
    if (Root == NULL) {
        return 0;
    }
    return 1 + DemTongDocGia(Root->Left) + DemTongDocGia(Root->Right);
}

// ===================== CHUYỂN ĐỔI DỮ LIỆU CÂY -> MẢNG ===========================
// Duyệt cây theo thứ tự LNR (Left-Node-Right) để lấy danh sách đã sắp xếp sơ bộ theo Mã thẻ
// Kết quả lưu vào mảng tĩnh arr[] để phục vụ in ấn hoặc sắp xếp lại theo tên
inline void DuyetLNRChuyenThanhMang(DocGiaNode* Root, DocGia* MangDuLieu[], int& SoPhanTu) {
    if (Root == NULL) {
        return;
    }
    DuyetLNRChuyenThanhMang(Root->Left, MangDuLieu, SoPhanTu);
    if (SoPhanTu < MaxDocGia) {
        MangDuLieu[SoPhanTu] = &Root->ThongTin;
        SoPhanTu++;
    }
    DuyetLNRChuyenThanhMang(Root->Right, MangDuLieu, SoPhanTu);
}

// ================== THUẬT TOÁN SẮP XẾP ==================
// Hoán đổi 2 con trỏ DocGia*
inline void HoanDoiDocGia(DocGia*& GiaTriThuNhat, DocGia*& GiaTriThuHai) {
    DocGia* Temp = GiaTriThuNhat;
    GiaTriThuNhat = GiaTriThuHai;
    GiaTriThuHai = Temp;
}
// --- 1. Sắp xếp Độc giả theo MÃ THẺ (Tăng dần) (Đệ Quy) ---
inline int PhanHoachDocGiaTheoMaThe(DocGia* MangDuLieu[], int ChiSoTrai, int ChiSoPhai) {
    int GiaTriChot = MangDuLieu[ChiSoPhai]->MaThe;
    int i = ChiSoTrai - 1;
    for (int j = ChiSoTrai; j < ChiSoPhai; j++) {
        if (MangDuLieu[j]->MaThe < GiaTriChot) {
            i++;
            HoanDoiDocGia(MangDuLieu[i], MangDuLieu[j]);
        }
    }
    HoanDoiDocGia(MangDuLieu[i + 1], MangDuLieu[ChiSoPhai]);
    return i + 1;
}
// Hàm chính Quick Sort theo mã thẻ (Chia để trị)
inline void QuickSortDocGiaTheoMaThe(DocGia* MangDuLieu[], int ChiSoTrai, int ChiSoPhai) {
    if (ChiSoTrai < ChiSoPhai) {
        int ViTriPhanHoach = PhanHoachDocGiaTheoMaThe(MangDuLieu, ChiSoTrai, ChiSoPhai);
        QuickSortDocGiaTheoMaThe(MangDuLieu, ChiSoTrai, ViTriPhanHoach - 1);
        QuickSortDocGiaTheoMaThe(MangDuLieu, ViTriPhanHoach + 1, ChiSoPhai);
    }
}
// --- 2. Sắp xếp Độc giả theo TÊN + HỌ (A -> Z) ---
// Ưu tiên: Tên -> Họ -> Mã thẻ
inline int PhanHoachDocGiaTheoTenHo(DocGia* MangDuLieu[], int ChiSoTrai, int ChiSoPhai) {
    DocGia* GiaTriChot = MangDuLieu[ChiSoPhai];
    int i = ChiSoTrai - 1;
    for (int j = ChiSoTrai; j < ChiSoPhai; j++) {
        bool Condition = false;
        // So sánh Tên trước
        if (std::strcmp(MangDuLieu[j]->Ten, GiaTriChot->Ten) < 0) {
            Condition = true;
        }
        else if (std::strcmp(MangDuLieu[j]->Ten, GiaTriChot->Ten) == 0) {
            // Tên trùng thì so sánh Họ
            if (std::strcmp(MangDuLieu[j]->Ho, GiaTriChot->Ho) < 0) {
                Condition = true;
            }
            // Nếu Tên và Họ đều trùng, so sánh Mã thẻ để ổn định vị trí
            else if (std::strcmp(MangDuLieu[j]->Ho, GiaTriChot->Ho) == 0 && MangDuLieu[j]->MaThe < GiaTriChot->MaThe) {
                Condition = true;
            }
        }
        if (Condition) {
            i++;
            HoanDoiDocGia(MangDuLieu[i], MangDuLieu[j]);
        }
    }
    HoanDoiDocGia(MangDuLieu[i + 1], MangDuLieu[ChiSoPhai]);
    return i + 1;
}
// Hàm chính Quick Sort theo TÊN + HỌ
inline void QuickSortDocGiaTheoTenHo(DocGia* MangDuLieu[], int ChiSoTrai, int ChiSoPhai) {
    if (ChiSoTrai < ChiSoPhai) {
        int ViTriPhanHoach = PhanHoachDocGiaTheoTenHo(MangDuLieu, ChiSoTrai, ChiSoPhai);
        QuickSortDocGiaTheoTenHo(MangDuLieu, ChiSoTrai, ViTriPhanHoach - 1);
        QuickSortDocGiaTheoTenHo(MangDuLieu, ViTriPhanHoach + 1, ChiSoPhai);
    }
}
