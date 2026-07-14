#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
#include "cautruc.h"
#include "dsdms.h"
#include "dsdausach.h"
#include "dsdocgia.h"
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
// Tách chuỗi theo ký tự '|'
inline std::vector<std::string> TachChuoiTheoDauGach(const std::string& DongDuLieu) {
    std::vector<std::string> KetQuaDauRa;
    std::string ChiSoHienTai;
    for (size_t i = 0; i < DongDuLieu.size(); i++) {
        char KyTuDoc = DongDuLieu[i];
        if (KyTuDoc == '|') {
            KetQuaDauRa.push_back(ChiSoHienTai);
            ChiSoHienTai.clear();
        }
        else {
            ChiSoHienTai.push_back(KyTuDoc);
        }
    }
    KetQuaDauRa.push_back(ChiSoHienTai);
    return KetQuaDauRa;
}
// Nối chuỗi với ký tự '|'
inline std::string NoiChuoiBangDauGach(const std::vector<std::string>& GiaTriCanThem) {
    std::ostringstream Oss;
    for (size_t i = 0; i < GiaTriCanThem.size(); i++) {
        if (i > 0) {
            Oss << '|';
        }
        Oss << GiaTriCanThem[i];
    }
    return Oss.str();
}
template <typename T> inline std::string ChuyenThanhChuoi(T GiaTriCanThem) {
    std::ostringstream Oss;
    Oss << GiaTriCanThem;
    return Oss.str();
}

// ========================= TIỆN ÍCH XỬ LÝ NGÀY THÁNG =========================
// Kiểm tra tính hợp lệ của ngày tháng
inline std::string ChuyenNgayThanhChuoi(const NgayThangNam& NgayCanXuLy) {
    if (!KiemTraNgayHopLe(NgayCanXuLy)) {
        return std::string("0/0/0");
    }
    std::ostringstream Oss;
    if (NgayCanXuLy.Ngay < 10) {
        Oss << '0';
    }
    Oss << NgayCanXuLy.Ngay << '/';
    if (NgayCanXuLy.Thang < 10) {
        Oss << '0';
    }
    Oss << NgayCanXuLy.Thang << '/';
    Oss << NgayCanXuLy.Nam;
    return Oss.str();
}
// Phân tích chuỗi thành ngày tháng
inline bool PhanTichChuoiNgay(const std::string& ChuoiNhap, NgayThangNam& KetQuaDauRa) {
    if (ChuoiNhap.empty()) {
        KetQuaDauRa = NgayThangNam{ 0, 0, 0 };
        return true;
    }
    int GiaTriThuNhat = 0, GiaTriThuHai = 0, KyTuDoc = 0;
    char Sep1 = 0, Sep2 = 0;
    std::istringstream Iss(ChuoiNhap);
    if ((Iss >> GiaTriThuNhat >> Sep1 >> GiaTriThuHai >> Sep2 >> KyTuDoc)) {
        if (Sep1 == '/' && Sep2 == '/') {
            KetQuaDauRa.Ngay = GiaTriThuNhat;
            KetQuaDauRa.Thang = GiaTriThuHai;
            KetQuaDauRa.Nam = KyTuDoc;
            return KiemTraNgayHopLe(KetQuaDauRa);
        }
        if (Sep1 == '-' && Sep2 == '-') {
            KetQuaDauRa.Nam = GiaTriThuNhat;
            KetQuaDauRa.Thang = GiaTriThuHai;
            KetQuaDauRa.Ngay = KyTuDoc;
            return KiemTraNgayHopLe(KetQuaDauRa);
        }
    }
    if (ChuoiNhap.size() == 8) {
        KetQuaDauRa.Ngay = std::atoi(ChuoiNhap.substr(0, 2).c_str());
        KetQuaDauRa.Thang = std::atoi(ChuoiNhap.substr(2, 2).c_str());
        KetQuaDauRa.Nam = std::atoi(ChuoiNhap.substr(4, 4).c_str());
        return KiemTraNgayHopLe(KetQuaDauRa);
    }
    return false;
}

//========================= GIẢI PHÓNG BỘ NHỚ =========================
// ----------------- Danh mục sách -----------------
inline void GiaiPhongDanhMucSachLuuTru(DanhMucSachNode*& Head) {
    GiaiPhongDanhMucSach(Head);
}
// ----------------- Đầu sách -----------------
inline void GiaiPhongVectorDauSach(std::vector<DauSach*>& DanhSachDauSach) {
    for (size_t i = 0; i < DanhSachDauSach.size(); i++) {
        DauSach* DuLieuSach = DanhSachDauSach[i];
        if (DuLieuSach != NULL) {
            GiaiPhongDanhMucSachLuuTru(DuLieuSach->DanhMucSachHead);
            delete DuLieuSach;
        }
    }
    DanhSachDauSach.clear();
}
// ----------------- Độc giả (BST) -----------------
inline void GiaiPhongDanhSachMuonTra(MuonTraNode*& Head) {
    MuonTraNode* ConTroHienTai = Head;
    while (ConTroHienTai != NULL) {
        MuonTraNode* NodeCanXoa = ConTroHienTai;
        ConTroHienTai = ConTroHienTai->Next;
        delete NodeCanXoa;
    }
    Head = NULL;
}
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
        int ChiSoCot = 0;
        std::string ChiSoHienTai = "";
        for (char KyTuDoc : DongDuLieu) {
            if (KyTuDoc == '|') {
                if (ChiSoCot < 8) {
                    CacCot[ChiSoCot++] = ChiSoHienTai;
                }
                ChiSoHienTai = "";
            }
            else {
                ChiSoHienTai += KyTuDoc;
            }
        }
        if (ChiSoCot < 8) {
            CacCot[ChiSoCot++] = ChiSoHienTai;
        }

        if (ChiSoCot < 8) {
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
    // Duyệt mảng tĩnh Nodes theo SoLuong phần tử.
    for (int i = 0; i < DanhSachDauSach.SoLuong; i++) {
        const DauSach* DuLieuSach = DanhSachDauSach.Nodes[i];
        if (DuLieuSach == NULL) {
            continue;
        }
        DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead;
        while (ConTroHienTai != NULL) {
            Fo << DuLieuSach->ISBN << "|" << ConTroHienTai->MaSach << "|" << (int)ConTroHienTai->TrangThai << "|"
                << ConTroHienTai->ViTri << "\n";

            ConTroHienTai = ConTroHienTai->Next;
        }
    }
    return true;
}
// 2. Đọc danh mục sách
inline bool DocDanhMucSach(DanhSachDauSach& DanhSachDauSach) {
    std::ifstream Fi(DuongDanTepDanhMucSach().c_str());
    if (!Fi) {
        return true;
    } // File chưa tồn tại thì coi như rỗng, không lỗi
    std::string DongDuLieu;
    while (std::getline(Fi, DongDuLieu)) {
        if (DongDuLieu.empty()) {
            continue;
        }
        size_t P1 = DongDuLieu.find('|');
        if (P1 == std::string::npos) {
            continue;
        }
        size_t P2 = DongDuLieu.find('|', P1 + 1);
        if (P2 == std::string::npos) {
            continue;
        }
        size_t P3 = DongDuLieu.find('|', P2 + 1);
        if (P3 == std::string::npos) {
            continue;
        }
        std::string ISBNCanXuLy = DongDuLieu.substr(0, P1);
        std::string MaSach = DongDuLieu.substr(P1 + 1, P2 - P1 - 1);
        std::string STrangThai = DongDuLieu.substr(P2 + 1, P3 - P2 - 1);
        std::string ViTri = DongDuLieu.substr(P3 + 1);
        // Tìm đầu sách trong danh sách đã load
        DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy);
        if (DuLieuSach == NULL) {
            continue;
        } // Nếu không thấy ISBN tương ứng thì bỏ qua
        // Tạo node bản sao
        int TrangThaiTam = std::atoi(STrangThai.c_str());
        int ThoiGianHienTai = (TrangThaiTam == 1 ? 1 : 0);
        DanhMucSachNode* NodeCanXuLy = new DanhMucSachNode();
        SaoChepChuoi(NodeCanXuLy->MaSach, MaxMaSach, MaSach);
        NodeCanXuLy->TrangThai = ThoiGianHienTai;
        SaoChepChuoi(NodeCanXuLy->ViTri, 50, ViTri);
        // Thêm vào DSLK đơn
        ThemSachVaoCuoiDanhMuc(DuLieuSach, NodeCanXuLy);
    }
    return true;
}

// ========================= LƯU / ĐỌC: ĐỘC GIẢ (BST) =========================
// Lưu cây BST độc giả
inline void LuuDocGiaTheoThuTu(std::ofstream& Fo, DocGiaNode* Root) {
    if (Root == NULL) {
        return;
    }
    LuuDocGiaTheoThuTu(Fo, Root->Left);
    {
        const DocGia& DocGiaCanXuLy = Root->ThongTin;
        std::vector<std::string> CacCot;
        CacCot.push_back(ChuyenThanhChuoi(DocGiaCanXuLy.MaThe));
        CacCot.push_back(DocGiaCanXuLy.Ho);
        CacCot.push_back(DocGiaCanXuLy.Ten);
        CacCot.push_back(DocGiaCanXuLy.Phai);
        CacCot.push_back(ChuyenThanhChuoi(DocGiaCanXuLy.TrangThaiThe));
        Fo << NoiChuoiBangDauGach(CacCot) << "\n";
    }
    LuuDocGiaTheoThuTu(Fo, Root->Right);
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
        std::vector<std::string> CacCot = TachChuoiTheoDauGach(DongDuLieu);
        if (CacCot.size() < 5) {
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
inline void DuyetCayLuuMuonTra(std::ofstream& Fo, DocGiaNode* Root) {
    if (Root == NULL) {
        return;
    }
    DuyetCayLuuMuonTra(Fo, Root->Left);
    {
        const DocGia& DocGiaCanXuLy = Root->ThongTin;
        MuonTraNode* ConTroHienTai = DocGiaCanXuLy.MuonTraHead;
        while (ConTroHienTai != NULL) {
            std::vector<std::string> CacCot;
            CacCot.push_back(ChuyenThanhChuoi(DocGiaCanXuLy.MaThe));
            CacCot.push_back(ConTroHienTai->MaSach);
            CacCot.push_back(ChuyenNgayThanhChuoi(ConTroHienTai->NgayMuon));
            CacCot.push_back(ChuyenNgayThanhChuoi(ConTroHienTai->NgayTra));
            CacCot.push_back(ChuyenThanhChuoi(static_cast<int>(ConTroHienTai->TrangThai)));
            Fo << NoiChuoiBangDauGach(CacCot) << "\n";
            ConTroHienTai = ConTroHienTai->Next;
        }
    }
    DuyetCayLuuMuonTra(Fo, Root->Right);
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
inline bool DocMuonTra(const DanhSachDauSach& DanhSachDauSach, DocGiaNode*& Root) {
    std::ifstream Fi(DuongDanTepMuonTra().c_str());
    if (!Fi) {
        return true;
    }
    std::string DongDuLieu;
    while (std::getline(Fi, DongDuLieu)) {
        if (DongDuLieu.empty()) {
            continue;
        }
        // Tách chuỗi thủ công
        std::string CacCot[5];
        int ChiSoCot = 0;
        std::string ChiSoHienTai = "";
        for (char KyTuDoc : DongDuLieu) {
            if (KyTuDoc == '|') {
                if (ChiSoCot < 5) {
                    CacCot[ChiSoCot++] = ChiSoHienTai;
                }
                ChiSoHienTai = "";
            }
            else {
                ChiSoHienTai += KyTuDoc;
            }
        }
        if (ChiSoCot < 5) {
            CacCot[ChiSoCot++] = ChiSoHienTai;
        }
        if (ChiSoCot < 5) {
            continue;
        }
        int MaTheCanXuLy = std::atoi(CacCot[0].c_str());
        std::string MaSach = CacCot[1];
        NgayThangNam NgayMuon{ 0, 0, 0 }, NgayTra{ 0, 0, 0 };
        PhanTichChuoiNgay(CacCot[2], NgayMuon);
        PhanTichChuoiNgay(CacCot[3], NgayTra);
        int TrangThaiTam = std::atoi(CacCot[4].c_str());
        DocGiaNode* DgNode = TimDocGiaTheoMaThe(Root, MaTheCanXuLy);
        if (DgNode == NULL) {
            continue;
        }
        MuonTraNode* NodeCanXuLy = new MuonTraNode();
        SaoChepChuoi(NodeCanXuLy->MaSach, MaxMaSach, MaSach);
        NodeCanXuLy->NgayMuon = NgayMuon;
        NodeCanXuLy->NgayTra = NgayTra;
        NodeCanXuLy->TrangThai = static_cast<int>(TrangThaiTam);
        NodeCanXuLy->Next = DgNode->ThongTin.MuonTraHead;
        DgNode->ThongTin.MuonTraHead = NodeCanXuLy;
        DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, LayISBNTuMaSach(MaSach));
        if (DuLieuSach != NULL) {
            DanhMucSachNode* Copy = TimSachTheoMaSach(DuLieuSach, MaSach);
            if (Copy != NULL) {
                if (NodeCanXuLy->TrangThai == 0) {
                    Copy->TrangThai = 1;
                }
                else {
                    Copy->TrangThai = 0;
                }
            }
        }
    }
    return true;
}

// ========================= LƯU / ĐỌC: TẤT CẢ DỮ LIỆU =========================
// Lưu tất cả dữ liệu
inline bool LuuToanBoDuLieu(const DanhSachDauSach& DanhSachDauSach, DocGiaNode* Root) {
    bool Ok1 = LuuDauSach(DanhSachDauSach);
    bool Ok2 = LuuDanhMucSach(DanhSachDauSach);
    bool Ok3 = LuuDocGia(Root);
    bool Ok4 = LuuMuonTra(Root);
    return Ok1 && Ok2 && Ok3 && Ok4;
}
inline bool DocToanBoDuLieu(DanhSachDauSach& DanhSachDauSach, DocGiaNode*& Root) {
    GiaiPhongDanhSachDauSach(DanhSachDauSach);
    GiaiPhongCayDocGia(Root);
    bool Ok1 = DocDauSach(DanhSachDauSach);
    bool Ok2 = DocDanhMucSach(DanhSachDauSach);
    for (int i = 0; i < DanhSachDauSach.SoLuong; ++i) {
        CapNhatSoLuongBanSao(DanhSachDauSach.Nodes[i]);
    }
    bool Ok3 = DocDocGia(Root);
    bool Ok4 = DocMuonTra(DanhSachDauSach, Root);
    return Ok1 && Ok2 && Ok3 && Ok4;
}
