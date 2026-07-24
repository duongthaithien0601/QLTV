#pragma once
#include <string>
#include <random>
#include <ctime>
#include "cautruc.h"

// =================== CÁC HÀM TÌM KIẾM TRÊN CÂY ===================
// Tìm độc giả trên cây nhị phân theo mã thẻ
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
// Kiểm tra mã thẻ đã tồn tại trên cây độc giả hay chưa
inline bool KiemTraMaTheTonTai(DocGiaNode* Root, int MaTheCanXuLy) {
    return TimDocGiaTheoMaThe(Root, MaTheCanXuLy) != NULL;
}

// =================== THAY ĐỔI CẤU TRÚC CÂY (THÊM / XÓA NODE) =====================
// Thêm một độc giả vào cây nhị phân tìm kiếm
inline void ThemDocGiaVaoCay(DocGiaNode*& Root, const DocGia& GiaTriCanThem) {
    if (Root == NULL) {
        Root = new DocGiaNode(GiaTriCanThem);
        return;
    }
    DocGiaNode* ConTroHienTai = Root;
    DocGiaNode* Parent = NULL;
    while (ConTroHienTai != NULL) {
        Parent = ConTroHienTai;
        if (GiaTriCanThem.MaThe < ConTroHienTai->ThongTin.MaThe) {
            ConTroHienTai = ConTroHienTai->Left;
        }
        else if (GiaTriCanThem.MaThe > ConTroHienTai->ThongTin.MaThe) {
            ConTroHienTai = ConTroHienTai->Right;
        }
        else {
            return;
        }
    }
    if (GiaTriCanThem.MaThe < Parent->ThongTin.MaThe) {
        Parent->Left = new DocGiaNode(GiaTriCanThem);
    }
    else {
        Parent->Right = new DocGiaNode(GiaTriCanThem);
    }
}
// Giải phóng danh sách mượn trả của một độc giả
inline void GiaiPhongDanhSachMuonTra(MuonTraNode*& Head) {
    while (Head != NULL) {
        MuonTraNode* NodeCanXoa = Head;
        Head = Head->Next;
        delete NodeCanXoa;
    }
}
// Xóa node độc giả khỏi cây nhị phân theo mã thẻ
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
        if (Root->Left == NULL) {
            DocGiaNode* NodeCanXoa = Root;
            Root = Root->Right;
            GiaiPhongDanhSachMuonTra(NodeCanXoa->ThongTin.MuonTraHead);
            delete NodeCanXoa;
        }
        else if (Root->Right == NULL) {
            DocGiaNode* NodeCanXoa = Root;
            Root = Root->Left;
            GiaiPhongDanhSachMuonTra(NodeCanXoa->ThongTin.MuonTraHead);
            delete NodeCanXoa;
        }
        else {
            DocGiaNode* ChaNodeTheMang = Root;
            DocGiaNode* NodeTheMang = Root->Right;
            while (NodeTheMang->Left != NULL) {
                ChaNodeTheMang = NodeTheMang;
                NodeTheMang = NodeTheMang->Left;
            }
            DocGia ThongTinTam = Root->ThongTin;
            Root->ThongTin = NodeTheMang->ThongTin;
            NodeTheMang->ThongTin = ThongTinTam;
            if (ChaNodeTheMang == Root) {
                ChaNodeTheMang->Right = NodeTheMang->Right;
            }
            else {
                ChaNodeTheMang->Left = NodeTheMang->Right;
            }
            GiaiPhongDanhSachMuonTra(NodeTheMang->ThongTin.MuonTraHead);
            delete NodeTheMang;
        }
    }
}

// ==================== TIỆN ÍCH & LOGIC NGHIỆP VỤ ====================
// Đếm số sách độc giả hiện đang mượn
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
// Xóa độc giả nếu không còn giữ sách của thư viện
inline bool XoaDocGiaNeuKhongMuonSach(DocGiaNode*& Root, int MaTheCanXuLy) {
    DocGiaNode* ConTroHienTai = TimDocGiaTheoMaThe(Root, MaTheCanXuLy);
    if (ConTroHienTai == NULL) {
        return false;
    } // Không tồn tại
    if (DemSoSachDocGiaDangMuon(ConTroHienTai->ThongTin) > 0) {
        return false;
    }
    XoaNodeDocGia(Root, MaTheCanXuLy);
    return true;
}
// Tạo mã thẻ ngẫu nhiên và bảo đảm không trùng
inline int TaoMaTheKhongTrung(DocGiaNode* Root) {
    static std::mt19937 Rng(static_cast<unsigned int>(std::time(NULL)));
    static std::uniform_int_distribution<int> Dist(100000, 999999);
    while (true) {
        int MaTheMoi = Dist(Rng);
        if (!KiemTraMaTheTonTai(Root, MaTheMoi)) {
            return MaTheMoi;
        }
    }
}
// Đếm tổng số độc giả trên cây
inline int DemTongDocGia(DocGiaNode* Root) {
    if (Root == NULL) {
        return 0;
    }
    return 1 + DemTongDocGia(Root->Left) + DemTongDocGia(Root->Right);
}

// =================== NGHIỆP VỤ THÊM / CẬP NHẬT ĐỘC GIẢ ===================
// Kiểm tra dữ liệu và thêm độc giả mới vào cây
inline bool ThemDocGia(
    DocGiaNode*& Root,
    int MaTheCanXuLy,
    const std::string& HoNhap,
    const std::string& TenNhap,
    const std::string& PhaiNhap,
    int TrangThaiNhap,
    std::string* ThongBaoLoi = NULL
){
    std::string HoDaChuanHoa = ChuanHoaChuoi(HoNhap);
    std::string TenDaChuanHoa = ChuanHoaChuoi(TenNhap);
    if (MaTheCanXuLy <= 0 || KiemTraMaTheTonTai(Root, MaTheCanXuLy)) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Ma the khong hop le hoac da ton tai.";
        }
        return false;
    }
    if (!KiemTraThongTinDocGiaNhap(HoDaChuanHoa, TenDaChuanHoa, PhaiNhap, TrangThaiNhap, ThongBaoLoi)){
        return false;
    }
    DocGia DocGiaCanThem;
    DocGiaCanThem.MaThe = MaTheCanXuLy;
    SaoChepChuoi(DocGiaCanThem.Ho, 50, HoDaChuanHoa);
    SaoChepChuoi(DocGiaCanThem.Ten, 30, TenDaChuanHoa);
    SaoChepChuoi(DocGiaCanThem.Phai, 5, PhaiNhap);
    DocGiaCanThem.TrangThaiThe = TrangThaiNhap;
    DocGiaCanThem.MuonTraHead = NULL;
    ThemDocGiaVaoCay(Root, DocGiaCanThem);
    return true;
}
// Cập nhật thông tin của độc giả theo mã thẻ
inline bool CapNhatThongTinDocGia(
    DocGiaNode* Root,
    int MaTheCanXuLy,
    const std::string& HoMoi,
    const std::string& TenMoi,
    const std::string& PhaiMoi,
    int TrangThaiMoi,
    std::string* ThongBaoLoi = NULL
) {
    DocGiaNode* NodeDocGia = TimDocGiaTheoMaThe(Root, MaTheCanXuLy);
    if (NodeDocGia == NULL) {
        if (ThongBaoLoi != NULL) {
            *ThongBaoLoi = "Khong tim thay doc gia.";
        }
        return false;
    }
    std::string HoDaChuanHoa = ChuanHoaChuoi(HoMoi);
    std::string TenDaChuanHoa = ChuanHoaChuoi(TenMoi);
    if (!KiemTraThongTinDocGiaCapNhat(HoMoi, TenMoi, PhaiMoi, TrangThaiMoi, ThongBaoLoi)){
        return false;
    }
    if (!HoMoi.empty()) {
        SaoChepChuoi(NodeDocGia->ThongTin.Ho, 50, HoDaChuanHoa);
    }
    if (!TenMoi.empty()) {
        SaoChepChuoi(NodeDocGia->ThongTin.Ten, 30, TenDaChuanHoa);
    }
    SaoChepChuoi(NodeDocGia->ThongTin.Phai, 5, PhaiMoi);
    NodeDocGia->ThongTin.TrangThaiThe = TrangThaiMoi;
    return true;
}
// ================== CHÈN CÓ THỨ TỰ ĐỘC GIẢ ==================
// Kiểm tra độc giả thứ nhất có đứng trước độc giả thứ hai theo thứ tự tên, họ và mã thẻ hay không
inline bool DocGiaDungTruocTheoTenHo(const DocGia* DocGiaThuNhat, const DocGia* DocGiaThuHai){
    int KetQuaSoSanhTen = std::strcmp(DocGiaThuNhat->Ten, DocGiaThuHai->Ten);
    if (KetQuaSoSanhTen < 0) {
        return true;
    }
    if (KetQuaSoSanhTen > 0) {
        return false;
    }
    int KetQuaSoSanhHo = std::strcmp(DocGiaThuNhat->Ho, DocGiaThuHai->Ho);
    if (KetQuaSoSanhHo < 0) {
        return true;
    }
    if (KetQuaSoSanhHo > 0) {
        return false;
    }
    return DocGiaThuNhat->MaThe < DocGiaThuHai->MaThe;
}
// Chèn một độc giả vào mảng đang tăng dần theo mã thẻ
inline void ChenDocGiaTheoMaThe(DocGia* MangDuLieu[], int& SoPhanTu, DocGia* DocGiaCanChen){
    if (MangDuLieu == NULL || DocGiaCanChen == NULL || SoPhanTu >= MaxDocGia){
        return;
    }
    int ViTriChen = SoPhanTu;
    while (ViTriChen > 0 && DocGiaCanChen->MaThe < MangDuLieu[ViTriChen - 1]->MaThe){
        MangDuLieu[ViTriChen] = MangDuLieu[ViTriChen - 1];
        ViTriChen--;
    }
    MangDuLieu[ViTriChen] = DocGiaCanChen;
    SoPhanTu++;
}
// Chèn một độc giả vào mảng đang tăng dần theo tên, họ và mã thẻ
inline void ChenDocGiaTheoTenHo(DocGia* MangDuLieu[], int& SoPhanTu, DocGia* DocGiaCanChen){
    if (MangDuLieu == NULL || DocGiaCanChen == NULL || SoPhanTu >= MaxDocGia){
        return;
    }
    int ViTriChen = SoPhanTu;
    while (ViTriChen > 0 && DocGiaDungTruocTheoTenHo(DocGiaCanChen, MangDuLieu[ViTriChen - 1])){
        MangDuLieu[ViTriChen] = MangDuLieu[ViTriChen - 1];
        ViTriChen--;
    }
    MangDuLieu[ViTriChen] = DocGiaCanChen;
    SoPhanTu++;
}
// Duyệt cây và lần lượt chèn độc giả vào mảng theo mã thẻ tăng dần
inline void DuyetCayChenDocGiaTheoMaThe(DocGiaNode* Root, DocGia* MangDuLieu[], int& SoPhanTu){
    if (Root == NULL || SoPhanTu >= MaxDocGia) {
        return;
    }
    ChenDocGiaTheoMaThe(MangDuLieu, SoPhanTu, &Root->ThongTin);
    DuyetCayChenDocGiaTheoMaThe(Root->Left, MangDuLieu, SoPhanTu);
    DuyetCayChenDocGiaTheoMaThe(Root->Right, MangDuLieu,  SoPhanTu);
}
// Duyệt cây và lần lượt chèn độc giả vào mảng theo tên, họ và mã thẻ tăng dần
inline void DuyetCayChenDocGiaTheoTenHo(DocGiaNode* Root, DocGia* MangDuLieu[], int& SoPhanTu){
    if (Root == NULL || SoPhanTu >= MaxDocGia) {
        return;
    }
    ChenDocGiaTheoTenHo(MangDuLieu, SoPhanTu, &Root->ThongTin);
    DuyetCayChenDocGiaTheoTenHo(Root->Left, MangDuLieu, SoPhanTu);
    DuyetCayChenDocGiaTheoTenHo(Root->Right, MangDuLieu, SoPhanTu);
}
// Lập danh sách độc giả theo mã thẻ bằng thuật toán chèn có thứ tự
inline void LapDanhSachDocGiaTheoMaThe(DocGiaNode* Root, DocGia* DanhSachKetQua[], int& SoLuongKetQua){
    SoLuongKetQua = 0;
    DuyetCayChenDocGiaTheoMaThe(Root, DanhSachKetQua, SoLuongKetQua);
}
// Lập danh sách độc giả theo tên và họ bằng thuật toán chèn có thứ tự
inline void LapDanhSachDocGiaTheoTenHo(DocGiaNode* Root, DocGia* DanhSachKetQua[], int& SoLuongKetQua){
    SoLuongKetQua = 0;
    DuyetCayChenDocGiaTheoTenHo(Root, DanhSachKetQua, SoLuongKetQua);
}

// =================== GIẢI PHÓNG BỘ NHỚ ===================
inline void GiaiPhongCayDocGia(DocGiaNode*& Root) {
    if (Root == NULL) {
        return;
    }
    GiaiPhongCayDocGia(Root->Left);
    GiaiPhongCayDocGia(Root->Right);
    GiaiPhongDanhSachMuonTra(Root->ThongTin.MuonTraHead);
    delete Root;
    Root = NULL;
}