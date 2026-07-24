#pragma once
#include <fstream>
#include <string>
#include <cstdio>
#include "cautruc.h"
#include "dausach.h"
#include "docgia.h"
#include "muontra.h"
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

// ========================= ĐƯỜNG DẪN TỆP DỮ LIỆU =========================
// Trả về đường dẫn thư mục chứa dữ liệu
inline std::string DuongDanThuMucDuLieu() {
    return std::string("data");
}
// Trả về đường dẫn tệp lưu đầu sách
inline std::string DuongDanTepDauSach() {
    return DuongDanThuMucDuLieu() + "/dausach.txt";
}
// Trả về đường dẫn tệp lưu danh mục sách
inline std::string DuongDanTepDanhMucSach() {
    return DuongDanThuMucDuLieu() + "/danhmucsach.txt";
}
// Trả về đường dẫn tệp lưu độc giả
inline std::string DuongDanTepDocGia() {
    return DuongDanThuMucDuLieu() + "/docgia.txt";
}
// Trả về đường dẫn tệp lưu mượn trả
inline std::string DuongDanTepMuonTra() {
    return DuongDanThuMucDuLieu() + "/muontra.txt";
}
// Tạo thư mục dữ liệu nếu thư mục chưa tồn tại
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
inline int TachChuoiTheoDauGach(const std::string& DongDuLieu, std::string CacCot[], int SoCotToiDa) {
    if (CacCot == NULL || SoCotToiDa <= 0) {
        return 0;
    }
    int SoLuongCot = 0;
    std::string CotHienTai = "";
    for (size_t i = 0;i < DongDuLieu.size();i++) {
        char KyTuDoc = DongDuLieu[i];
        if (KyTuDoc == '|') {
            if (SoLuongCot >= SoCotToiDa) {
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

// ========================= LƯU / ĐỌC: ĐẦU SÁCH =========================
// Lưu danh sách đầu sách xuống tệp
inline bool LuuDauSach(const DanhSachDauSach& DanhSachDauSach) {
    DamBaoThuMucDuLieu();
    std::ofstream Fo(DuongDanTepDauSach().c_str());
    if (!Fo) {
        return false;
    }
    for (int i = 0; i < DanhSachDauSach.SoLuong; i++) {
        const DauSach* DuLieuSach = DanhSachDauSach.Nodes[i];
        if (DuLieuSach == NULL) {
            continue;
        }
        Fo << DuLieuSach->ISBN << "|" << DuLieuSach->TenSach << "|" << DuLieuSach->TacGia << "|" << DuLieuSach->TheLoai << "|" << DuLieuSach->SoTrang << "|" 
            << DuLieuSach->NamXuatBan << "|" << DuLieuSach->SoLuongBanSao << "|" << DuLieuSach->SoLuotMuon << "\n";
    }
    return true;
}
// Đọc danh sách đầu sách từ tệp
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
        int SoLuongCot = TachChuoiTheoDauGach(DongDuLieu, CacCot, 8);
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
        if (!KiemTraDanhSachDauSachDay(DanhSachDauSach)) {
            DanhSachDauSach.Nodes[DanhSachDauSach.SoLuong++] = DuLieuSach;
        }
    }
    return true;
}

// ========================= LƯU / ĐỌC: DANH MỤC SÁCH =========================
// Lưu danh mục các bản sao xuống tệp
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
        while (ConTroHienTai != NULL) {
            Fo << DuLieuSach->ISBN << "|" << ConTroHienTai->MaSach << "|" << ConTroHienTai->TrangThai << "\n";
            ConTroHienTai = ConTroHienTai->Next;
        }
    }
    return true;
}
// Đọc danh mục các bản sao từ tệp
inline bool DocDanhMucSach(DanhSachDauSach& DanhSachDauSach) {
    std::ifstream Fi(DuongDanTepDanhMucSach().c_str());
    if (!Fi) { return true; }
    std::string DongDuLieu;
    while (std::getline(Fi, DongDuLieu)) {
        if (DongDuLieu.empty()) {
            continue;
        }
        std::string CacCot[3];
        int SoLuongCot = TachChuoiTheoDauGach(DongDuLieu, CacCot, 3);
        if (SoLuongCot != 3) {
            continue;
        }
        std::string ISBNCanXuLy = CacCot[0];
        std::string MaSach = CacCot[1];
        std::string ChuoiTrangThai = CacCot[2];
        DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy.c_str());
        if (DuLieuSach == NULL) {
            continue;
        }
        DanhMucSachNode* NodeCanXuLy = new DanhMucSachNode();
        SaoChepChuoi(NodeCanXuLy->MaSach, MaxMaSach, MaSach);
        int TrangThaiTam = std::atoi(ChuoiTrangThai.c_str());
        NodeCanXuLy->TrangThai = (TrangThaiTam == 1 ? 1 : 0);
        ThemSachVaoCuoiDanhMuc(DuLieuSach, NodeCanXuLy);
    }

    return true;
}

// ========================= LƯU / ĐỌC: ĐỘC GIẢ (BST) =========================
// Duyệt cây theo thứ tự và ghi từng độc giả xuống tệp
inline void LuuDocGiaTheoThuTu(std::ofstream& Fo, DocGiaNode* Root) {
    if (Root == NULL) {
        return;
    }
    LuuDocGiaTheoThuTu(Fo, Root->Left);
    const DocGia& DocGiaCanXuLy = Root->ThongTin;
    Fo
        << DocGiaCanXuLy.MaThe << "|"
        << DocGiaCanXuLy.Ho << "|"
        << DocGiaCanXuLy.Ten << "|"
        << DocGiaCanXuLy.Phai << "|"
        << DocGiaCanXuLy.TrangThaiThe
        << "\n";
    LuuDocGiaTheoThuTu(Fo, Root->Right);
}
// Lưu toàn bộ cây độc giả xuống tệp
inline bool LuuDocGia(DocGiaNode* Root) {
    DamBaoThuMucDuLieu();
    std::ofstream Fo(DuongDanTepDocGia().c_str());
    if (!Fo) {
        return false;
    }
    LuuDocGiaTheoThuTu(Fo, Root);
    return true;
}
// Đọc dữ liệu độc giả từ tệp và dựng lại cây
inline bool DocDocGia(DocGiaNode*& Root) {
    Root = NULL;
    std::ifstream Fi(DuongDanTepDocGia().c_str());
    if (!Fi) {
        return true;
    }
    std::string DongDuLieu;
    while (std::getline(Fi, DongDuLieu)) {
        std::string CacCot[5];
        int SoLuongCot = TachChuoiTheoDauGach(DongDuLieu, CacCot, 5);
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
// Duyệt cây độc giả và ghi các phiếu mượn trả xuống tệp
inline void DuyetCayLuuMuonTra(std::ofstream& Fo, DocGiaNode* Root) {
    if (Root == NULL) {
        return;
    }
    DuyetCayLuuMuonTra(Fo, Root->Left);
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
    DuyetCayLuuMuonTra(Fo, Root->Right);
}
// Lưu toàn bộ danh sách mượn trả xuống tệp
inline bool LuuMuonTra(DocGiaNode* Root) {
    DamBaoThuMucDuLieu();
    std::ofstream Fo(DuongDanTepMuonTra().c_str());
    if (!Fo) {
        return false;
    }
    DuyetCayLuuMuonTra(Fo, Root);
    return true;
}
// Đọc dữ liệu mượn trả và khôi phục trạng thái các bản sao
inline bool DocMuonTra(
    const DanhSachDauSach& DanhSachDauSach, DocGiaNode*& Root) {
    std::ifstream Fi(DuongDanTepMuonTra().c_str());
    if (!Fi) {
        return true;
    }
    NgayThangNam NgayHienTai = LayNgayHienTai();
    std::string DongDuLieu;
    while (std::getline(Fi, DongDuLieu)) {
        if (DongDuLieu.empty()) {
            continue;
        }
        std::string CacCot[5];
        int SoLuongCot = TachChuoiTheoDauGach(DongDuLieu, CacCot, 5);
        if (SoLuongCot != 5) {
            continue;
        }
        int MaTheCanXuLy = std::atoi(CacCot[0].c_str());
        std::string MaSach = CacCot[1];
        int TrangThaiTam = std::atoi(CacCot[4].c_str());
        if (TrangThaiTam != 0 && TrangThaiTam != 1) {
            continue;
        }
        NgayThangNam NgayMuon{ 0, 0, 0 };
        NgayThangNam NgayTra{ 0, 0, 0 };
        if (!PhanTichNgayDDMMYYYY(CacCot[2], NgayMuon) || SoSanhNgay(NgayMuon, NgayHienTai) > 0) {
            continue;
        }
        if (TrangThaiTam == 1) {
            if (!PhanTichNgayDDMMYYYY(CacCot[3], NgayTra) || SoSanhNgay(NgayTra, NgayMuon) < 0 || SoSanhNgay(NgayTra, NgayHienTai) > 0) {
                continue;
            }
        }
        DocGiaNode* DgNode = TimDocGiaTheoMaThe(Root, MaTheCanXuLy);
        if (DgNode == NULL) {
            continue;
        }
        char ISBNCanXuLy[15];
        LayISBNTuMaSach(MaSach.c_str(), ISBNCanXuLy, 15);
        DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy);
        DanhMucSachNode* BanSaoSach = NULL;
        if (DuLieuSach != NULL) {
            BanSaoSach = TimSachTheoMaSach(DuLieuSach, MaSach.c_str());
        }
        ThemPhieuMuonTraChoDocGia(DgNode->ThongTin, MaSach.c_str(), NgayMuon, NgayTra, TrangThaiTam);
    }
    return true;
}

// ========================= LƯU / ĐỌC: TẤT CẢ DỮ LIỆU =========================
// Lưu toàn bộ dữ liệu của chương trình
inline bool LuuToanBoDuLieu(const DanhSachDauSach& DanhSachDauSach, DocGiaNode* Root) {
    bool Ok1 = LuuDauSach(DanhSachDauSach);
    bool Ok2 = LuuDanhMucSach(DanhSachDauSach);
    bool Ok3 = LuuDocGia(Root);
    bool Ok4 = LuuMuonTra(Root);
    return Ok1 && Ok2 && Ok3 && Ok4;
}
// Đọc và khôi phục toàn bộ dữ liệu của chương trình
inline bool DocToanBoDuLieu(DanhSachDauSach& DanhSachDauSach, DocGiaNode*& Root) {
    GiaiPhongDanhSachDauSach(DanhSachDauSach);
    GiaiPhongCayDocGia(Root);
    bool Ok1 = DocDauSach(DanhSachDauSach);
    bool Ok2 = DocDanhMucSach(DanhSachDauSach);
    for (int i = 0; i < DanhSachDauSach.SoLuong; i++) {
        CapNhatSoLuongBanSao(DanhSachDauSach.Nodes[i]);
    }
    DatLaiTrangThaiTatCaBanSao(DanhSachDauSach);
    bool Ok3 = DocDocGia(Root);
    bool Ok4 = DocMuonTra(DanhSachDauSach, Root);
    return Ok1 && Ok2 && Ok3 && Ok4;
}