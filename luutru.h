#pragma once
#include <fstream>
#include <string>
#include <cstdio>
#include "cautruc.h"
#include "dsdms.h"
#include "dsdausach.h"
#include "dsdocgia.h"
#include "dsmuontra.h"
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

// ========================= ĐƯỜNG DẪN TỆP DỮ LIỆU =========================
inline std::string DuongDanThuMucDuLieu() {
    return std::string("data");
}
inline std::string DuongDanTepDauSach() {
    return DuongDanThuMucDuLieu() + "/dausach.txt";
}
inline std::string DuongDanTepDanhMucSach() {
    return DuongDanThuMucDuLieu() + "/dms.txt";
}
inline std::string DuongDanTepDocGia() {
    return DuongDanThuMucDuLieu() + "/docgia.txt";
}
inline std::string DuongDanTepMuonTra() {
    return DuongDanThuMucDuLieu() + "/muontra.txt";
}
inline bool DamBaoThuMucDuLieu() {
#ifdef _WIN32
    int KetQuaNhap = _mkdir(DuongDanThuMucDuLieu().c_str());
    (void)KetQuaNhap;
#else
    mkdir(DuongDanThuMucDuLieu().c_str(), 0755);
#endif
    return true;
}

//========================= TIỆN ÍCH XỬ LÝ CHUỖI =========================
// Tách một dòng thành các cột theo ký tự '|'.
inline int TachChuoiTheoDauGach(const std::string& DongDuLieu,std::string CacCot[],int SoCotToiDa){
    if (CacCot == NULL || SoCotToiDa <= 0){
        return 0;
    }
    int SoLuongCot = 0;
    std::string CotHienTai = "";
    for (size_t i = 0;i < DongDuLieu.size();i++){
        char KyTuDoc = DongDuLieu[i];
        if (KyTuDoc == '|'){
            if (SoLuongCot >= SoCotToiDa){
                return SoCotToiDa + 1;
            }
            CacCot[SoLuongCot] = CotHienTai;
            SoLuongCot++;
            CotHienTai = "";
        }
        else {
            CotHienTai += KyTuDoc;
        }
    }
    if (SoLuongCot >= SoCotToiDa) {
        return SoCotToiDa + 1;
    }
    CacCot[SoLuongCot] = CotHienTai;
    SoLuongCot++;
    return SoLuongCot;
}

//========================= GIẢI PHÓNG BỘ NHỚ =========================
// ----------------- Độc giả (BST) -----------------
// Giải phóng toàn bộ cây BST độc giả
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

// ========================= LƯU / ĐỌC: ĐẦU SÁCH =========================
// Lưu danh sách đầu sách
inline bool LuuDauSach(const DanhSachDauSach& DanhSachDauSach) {
    DamBaoThuMucDuLieu();
    std::ofstream Fo(DuongDanTepDauSach().c_str());
    if (!Fo) {
        return false;
    }
    // Duyệt mảng tĩnh
    for (int i = 0; i < DanhSachDauSach.SoLuong; i++) {
        const DauSach* DuLieuSach = DanhSachDauSach.Nodes[i];
        if (DuLieuSach == NULL) {
            continue;
        }
        // Ghi trực tiếp không dùng vector nối chuỗi
        Fo << DuLieuSach->ISBN << "|" << DuLieuSach->TenSach << "|" << DuLieuSach->TacGia << "|" << DuLieuSach->TheLoai
            << "|" << DuLieuSach->SoTrang << "|" << DuLieuSach->NamXuatBan << "|" << DuLieuSach->SoLuongBanSao << "|"
            << DuLieuSach->SoLuotMuon << "\n";
    }
    return true;
}
// Đọc danh sách đầu sách
inline bool DocDauSach(DanhSachDauSach& DanhSachDauSach) {
    DanhSachDauSach.SoLuong = 0;
    std::ifstream Fi(DuongDanTepDauSach().c_str());
    if (!Fi) {
        return true;
    }
    std::string DongDuLieu;
    while (std::getline(Fi, DongDuLieu)) {
        if (DongDuLieu.empty()) {
            continue;
        }
        std::string CacCot[8];
        int SoLuongCot = TachChuoiTheoDauGach(DongDuLieu,CacCot,8);
        if (SoLuongCot != 8) {
            continue;
        }
        DauSach* DuLieuSach = new DauSach();
        SaoChepChuoi(DuLieuSach->ISBN, 15, CacCot[0]);
        SaoChepChuoi(DuLieuSach->TenSach, 100, CacCot[1]);
        SaoChepChuoi(DuLieuSach->TacGia, 60, CacCot[2]);
        SaoChepChuoi(DuLieuSach->TheLoai, 40, CacCot[3]);
        DuLieuSach->SoTrang = std::atoi(CacCot[4].c_str());
        DuLieuSach->NamXuatBan = std::atoi(CacCot[5].c_str());
        DuLieuSach->SoLuongBanSao = std::atoi(CacCot[6].c_str());
        DuLieuSach->SoLuotMuon = std::atoi(CacCot[7].c_str());
        DuLieuSach->DanhMucSachHead = NULL;
        // Thêm vào mảng tĩnh
        if (!KiemTraDanhSachDauSachDay(DanhSachDauSach)) {
            DanhSachDauSach.Nodes[DanhSachDauSach.SoLuong++] = DuLieuSach;
        }
    }
    return true;
}

// ========================= LƯU / ĐỌC: DANH MỤC SÁCH =========================
// 1. Lưu danh mục sách
inline bool LuuDanhMucSach(const DanhSachDauSach& DanhSachDauSach) {
    DamBaoThuMucDuLieu();
    std::ofstream Fo(DuongDanTepDanhMucSach().c_str());
    if (!Fo) {
        return false;
    }
    for (int i = 0; i < DanhSachDauSach.SoLuong; i++) {
        const DauSach* DuLieuSach = DanhSachDauSach.Nodes[i];
        if (DuLieuSach == NULL) {
            continue;
        }
        DanhMucSachNode* ConTroHienTai =
            DuLieuSach->DanhMucSachHead;
        while (ConTroHienTai != NULL) {Fo << DuLieuSach->ISBN << "|"<< ConTroHienTai->MaSach << "|" << ConTroHienTai->TrangThai << "\n";
            ConTroHienTai = ConTroHienTai->Next;
        }
    }
    return true;
}
// 2. Đọc danh mục sách
inline bool DocDanhMucSach(DanhSachDauSach& DanhSachDauSach) {
    std::ifstream Fi(DuongDanTepDanhMucSach().c_str());
    if (!Fi) {return true;}
    std::string DongDuLieu;
    while (std::getline(Fi, DongDuLieu)) {
        if (DongDuLieu.empty()) {
            continue;
        }           
        std::string CacCot[3];
        int SoLuongCot = TachChuoiTheoDauGach(DongDuLieu,CacCot,3);
        if (SoLuongCot != 3) {
            continue;
        }
        std::string ISBNCanXuLy = CacCot[0];
        std::string MaSach = CacCot[1];
        std::string ChuoiTrangThai = CacCot[2];
        DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach,ISBNCanXuLy);
        if (DuLieuSach == NULL) {
            continue;
        }
        DanhMucSachNode* NodeCanXuLy =  new DanhMucSachNode();
        SaoChepChuoi(NodeCanXuLy->MaSach,MaxMaSach,MaSach
        );
        // Nếu không thấy ISBN tương ứng thì bỏ qua
        // Tạo node bản sao
        int TrangThaiTam = std::atoi(ChuoiTrangThai.c_str());
        NodeCanXuLy->TrangThai =(TrangThaiTam == 1 ? 1 : 0);
        // Thêm vào DSLK đơn
        ThemSachVaoCuoiDanhMuc(DuLieuSach,NodeCanXuLy);
    }

    return true;
}

// ========================= LƯU / ĐỌC: ĐỘC GIẢ (BST) =========================
// Lưu cây BST độc giả
inline void LuuDocGiaTheoThuTu(std::ofstream& Fo,DocGiaNode* Root){
    if (Root == NULL) {
        return;
    }
    LuuDocGiaTheoThuTu(Fo,Root->Left);
    const DocGia& DocGiaCanXuLy = Root->ThongTin;
    Fo
        << DocGiaCanXuLy.MaThe << "|"
        << DocGiaCanXuLy.Ho << "|"
        << DocGiaCanXuLy.Ten << "|"
        << DocGiaCanXuLy.Phai << "|"
        << DocGiaCanXuLy.TrangThaiThe
        << "\n";
    LuuDocGiaTheoThuTu(Fo,Root->Right);
}
// Đọc cây BST độc giả
inline bool LuuDocGia(DocGiaNode* Root) {
    DamBaoThuMucDuLieu();
    std::ofstream Fo(DuongDanTepDocGia().c_str());
    if (!Fo) {
        return false;
    }
    LuuDocGiaTheoThuTu(Fo, Root);
    return true;
}
// Đọc cây BST độc giả
inline bool DocDocGia(DocGiaNode*& Root) {
    Root = NULL;
    std::ifstream Fi(DuongDanTepDocGia().c_str());
    if (!Fi) {
        return true;
    }
    std::string DongDuLieu;
    while (std::getline(Fi, DongDuLieu)) {
        std::string CacCot[5];
        int SoLuongCot = TachChuoiTheoDauGach(DongDuLieu,CacCot,5);
        if (SoLuongCot != 5) {
            continue;
        }
        DocGia DocGiaCanXuLy;
        DocGiaCanXuLy.MaThe = std::atoi(CacCot[0].c_str());
        SaoChepChuoi(DocGiaCanXuLy.Ho, 50, CacCot[1]);
        SaoChepChuoi(DocGiaCanXuLy.Ten, 30, CacCot[2]);
        std::string PhaiTok = CatKhoangTrangHaiDau(CacCot[3]);
        if (PhaiTok == "0") {
            SaoChepChuoi(DocGiaCanXuLy.Phai, 5, "Nam");
        }
        else if (PhaiTok == "1") {
            SaoChepChuoi(DocGiaCanXuLy.Phai, 5, "Nu");
        }
        else {
            SaoChepChuoi(DocGiaCanXuLy.Phai, 5, PhaiTok);
        }
        int Raw = std::atoi(CacCot[4].c_str());
        DocGiaCanXuLy.TrangThaiThe = (Raw != 0 ? 1 : 0);
        DocGiaCanXuLy.MuonTraHead = NULL;
        ThemDocGiaVaoCay(Root, DocGiaCanXuLy);
    }
    return true;
}

// ========================= LƯU / ĐỌC: MƯỢN TRẢ SÁCH =========================
// Lưu danh sách mượn trả sách
inline void DuyetCayLuuMuonTra(std::ofstream& Fo,DocGiaNode* Root){
    if (Root == NULL) {
        return;
    }
    DuyetCayLuuMuonTra(Fo,Root->Left);
    const DocGia& DocGiaCanXuLy = Root->ThongTin;
    MuonTraNode* ConTroHienTai = DocGiaCanXuLy.MuonTraHead;
    while (ConTroHienTai != NULL) {
        Fo
            << DocGiaCanXuLy.MaThe << "|"
            << ConTroHienTai->MaSach << "|"
            << ChuyenNgayThanhChuoi(
                ConTroHienTai->NgayMuon
            )
            << "|"
            << ChuyenNgayThanhChuoi(
                ConTroHienTai->NgayTra
            )
            << "|"
            << ConTroHienTai->TrangThai
            << "\n";
        ConTroHienTai =
            ConTroHienTai->Next;
    }
    DuyetCayLuuMuonTra(Fo,Root->Right);
}
// Đọc danh sách mượn trả sách
inline bool LuuMuonTra(DocGiaNode* Root) {
    DamBaoThuMucDuLieu();
    std::ofstream Fo(DuongDanTepMuonTra().c_str());
    if (!Fo) {
        return false;
    }
    DuyetCayLuuMuonTra(Fo, Root);
    return true;
}
// Đọc danh sách mượn trả sách
inline bool DocMuonTra(
    const DanhSachDauSach& DanhSachDauSach,DocGiaNode*& Root){
    std::ifstream Fi( DuongDanTepMuonTra().c_str() );
    if (!Fi){
        return true;
    }
    NgayThangNam NgayHienTai = LayNgayHienTai();
    std::string DongDuLieu;
    while (std::getline(Fi, DongDuLieu)){
        if (DongDuLieu.empty()){
            continue;
        }
        std::string CacCot[5];
        int SoLuongCot = TachChuoiTheoDauGach(DongDuLieu,CacCot,5);
        if (SoLuongCot != 5) {
            continue;
        }
        int MaTheCanXuLy =  std::atoi(CacCot[0].c_str());
        std::string MaSach =  CacCot[1];
        int TrangThaiTam = std::atoi(CacCot[4].c_str());
        if (TrangThaiTam != 0 && TrangThaiTam != 1){
            continue;
        }
        NgayThangNam NgayMuon{ 0, 0, 0 };
        NgayThangNam NgayTra{ 0, 0, 0 };
        // Ngày mượn phải hợp lệ và không được ở tương lai
        if (!PhanTichNgayDDMMYYYY(CacCot[2], NgayMuon) || SoSanhNgay(NgayMuon, NgayHienTai) > 0){
            continue;
        }
        // Phiếu đã trả phải có ngày trả hợp lệ
        if (TrangThaiTam == 1){
            if (!PhanTichNgayDDMMYYYY(CacCot[3], NgayTra) || SoSanhNgay(NgayTra, NgayMuon) < 0 || SoSanhNgay(NgayTra, NgayHienTai) > 0){
                continue;
            }
        }
        DocGiaNode* DgNode = TimDocGiaTheoMaThe(Root,MaTheCanXuLy);
        if (DgNode == NULL){
            continue;
        }
        DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach,LayISBNTuMaSach(MaSach));
        DanhMucSachNode* BanSaoSach = NULL;
        if (DuLieuSach != NULL){
            BanSaoSach = TimSachTheoMaSach(DuLieuSach,MaSach);
        }
        if (TrangThaiTam == 0) {
            if (!DanhDauSachDaMuon(BanSaoSach)) {
                continue;
            }
        }
        ThemPhieuMuonTraChoDocGia(
            DgNode->ThongTin,
            MaSach,
            NgayMuon,
            NgayTra,
            TrangThaiTam
        );
    }
    return true;
}

// ========================= LƯU / ĐỌC: TẤT CẢ DỮ LIỆU =========================
// Lưu tất cả dữ liệu
inline bool LuuToanBoDuLieu(const DanhSachDauSach& DanhSachDauSach,DocGiaNode* Root){
    bool Ok1 = LuuDauSach(DanhSachDauSach);
    bool Ok2 = LuuDanhMucSach(DanhSachDauSach);
    bool Ok3 = LuuDocGia(Root);
    bool Ok4 = LuuMuonTra(Root);
    return Ok1 && Ok2 && Ok3 && Ok4;
}
inline bool DocToanBoDuLieu(DanhSachDauSach& DanhSachDauSach,DocGiaNode*& Root){
    GiaiPhongDanhSachDauSach(DanhSachDauSach);
    GiaiPhongCayDocGia(Root);
    bool Ok1 = DocDauSach(DanhSachDauSach);
    bool Ok2 = DocDanhMucSach(DanhSachDauSach);
    for (int i = 0;i < DanhSachDauSach.SoLuong;i++){
        CapNhatSoLuongBanSao(DanhSachDauSach.Nodes[i]);
    }
    // Không dùng trạng thái cũ trong dms.txt.
    // Trạng thái thật được phục hồi từ phiếu đang mượn.
    DatLaiTrangThaiTatCaBanSao(DanhSachDauSach);
    bool Ok3 = DocDocGia(Root);
    bool Ok4 = DocMuonTra(DanhSachDauSach,Root);
    // Sắp xếp lại mảng đầu sách sau khi nạp tệp
    if (DanhSachDauSach.SoLuong > 1){
        QuickSortTheoTenSach(DanhSachDauSach.Nodes,0,DanhSachDauSach.SoLuong - 1);
    }
    return Ok1 && Ok2 && Ok3 && Ok4;
}