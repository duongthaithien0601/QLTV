#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include "tui.h"
#include "cautruc.h"
#include "dsdocgia.h"
#include "dsdausach.h"
#include "dsdms.h"
#include "dsmuontra.h"
#include "thongke.h"

namespace menutui {
    //==================== Menu helpers ====================//
    inline void VeKhungMenu(const std::string& TieuDe, int SoLuongMuc, int& ChieuRongKetQua, int& ChieuCaoKetQua) {
        int ChieuCao = 5 + SoLuongMuc + 3;
        if (ChieuCao < 12) {
            ChieuCao = 12;
        }
        int ChieuRong = 118;
        tui::XoaManHinh();
        tui::VeKhung(2, 1, ChieuRong, ChieuCao, TieuDe);
        ChieuRongKetQua = ChieuRong;
        ChieuCaoKetQua = ChieuCao;
    }
    inline void InMucMenu(int X, int Y, const std::string& NoiDung, bool DangChon) {
        tui::DiChuyenConTro(X, Y);
        if (DangChon) {
            std::cout << "\x1b[7m" << NoiDung << "\x1b[0m";
            tui::DatMau(tui::MauNoiBat);
        }
        else {
            tui::DatMau(tui::MauNoiBat);
            std::cout << NoiDung;
        }
    }
    inline int MenuMuiTen(int X, int /*y*/, const std::string& TieuDe, const std::vector<std::string>& DanhSachMuc) {
        const int MENUY = 6;
        int ChiSoHienTai = 0;
        int ChieuRong = 0, ChieuCao = 0;
        VeKhungMenu(TieuDe, (int)DanhSachMuc.size(), ChieuRong, ChieuCao);
        int FooterY = 1 + ChieuCao - 2;
        int Pad = 0;
        for (auto& ChuoiNhap : DanhSachMuc) {
            Pad = std::max(Pad, (int)ChuoiNhap.size());
        }
        auto PaintOne = [&](int ChiSo, bool DangChon) {
            tui::DiChuyenConTro(X, MENUY + ChiSo);
            if (DangChon) {
                std::cout << "\x1b[7m" << DanhSachMuc[ChiSo];
                int Sp = Pad - (int)DanhSachMuc[ChiSo].size();
                if (Sp > 0) {
                    std::cout << std::string(Sp, ' ');
                }
                std::cout << "\x1b[0m";
                tui::DatMau(tui::MauNoiBat);
            }
            else {
                tui::DatMau(tui::MauNoiBat);
                std::cout << DanhSachMuc[ChiSo];
                int Sp = Pad - (int)DanhSachMuc[ChiSo].size();
                if (Sp > 0) {
                    std::cout << std::string(Sp, ' ');
                }
            }
            };
        for (size_t i = 0; i < DanhSachMuc.size(); ++i) {
            PaintOne((int)i, (int)i == ChiSoHienTai);
        }
        tui::InHuongDanCuoiTrang(4, FooterY, "[Up/Down] Chon  -  [Enter] Xac nhan  -  [Esc] Quay lai");
        while (true) {
            auto SuKienPhim = tui::DocPhim();
            int ChiSoTruoc = ChiSoHienTai;
            if (SuKienPhim.Phim == tui::PhimLen) {
                ChiSoHienTai = (ChiSoHienTai + (int)DanhSachMuc.size() - 1) % (int)DanhSachMuc.size();
            }
            else if (SuKienPhim.Phim == tui::PhimXuong) {
                ChiSoHienTai = (ChiSoHienTai + 1) % (int)DanhSachMuc.size();
            }
            else if (SuKienPhim.Phim == tui::PhimEsc) {
                return (int)DanhSachMuc.size() - 1;
            }
            else if (SuKienPhim.Phim == tui::PhimEnter) {
                return ChiSoHienTai;
            }
            if (ChiSoHienTai != ChiSoTruoc) {
                PaintOne(ChiSoTruoc, false);
                PaintOne(ChiSoHienTai, true);
            }
        }
    }

    //================ Input helpers =================//
    inline void LamSachBoDemNhap() {
        std::cin.clear();
        if (std::cin.rdbuf()->in_avail() > 0) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    inline std::string DocDongTaiViTri(int X, int Y, int DoDaiToiDa = 64) {
        tui::DiChuyenConTro(X, Y);
        tui::HienConTro();
        std::string ChuoiNhap;
        std::getline(std::cin, ChuoiNhap);
        tui::AnConTro();
        if ((int)ChuoiNhap.size() > DoDaiToiDa) {
            ChuoiNhap.resize((size_t)DoDaiToiDa);
        }
        return CatKhoangTrangHaiDau(ChuoiNhap);
    }
    inline int DocSoNguyenTuyChonTaiViTri(int X, int Y, int GiaTriNhoNhat, int GiaTriLonNhat, int* GiaTriKetQua) {
        std::string ChuoiNhap = DocDongTaiViTri(X, Y, 16);
        if (ChuoiNhap.empty()) {
            return 0;
        }
        try {
            int GiaTriCanThem = std::stoi(ChuoiNhap);
            if (GiaTriCanThem < GiaTriNhoNhat || GiaTriCanThem > GiaTriLonNhat) {
                throw std::out_of_range("range");
            }
            if (GiaTriKetQua) {
                *GiaTriKetQua = GiaTriCanThem;
            }
            return 1;
        }
        catch (...) {
            tui::DiChuyenConTro(X, Y);
            std::cout << std::string(24, ' ');
            tui::DiChuyenConTro(X, Y);
            tui::DatMau(tui::MauCanhBao);
            std::cout << "(so " << GiaTriNhoNhat << "-" << GiaTriLonNhat << ")";
            tui::DatLaiMau();
            return 0;
        }
    }
    inline int ChonRadioTaiViTri(int X, int Y, const std::vector<std::string>& DanhSachLuaChon, int LuaChonBanDau) {
        int LuaChon = LuaChonBanDau;
        while (true) {
            tui::DiChuyenConTro(X, Y);
            for (size_t i = 0; i < DanhSachLuaChon.size(); ++i) {
                bool On = ((int)i == LuaChon);
                std::cout << (On ? "(*) " : "( ) ") << DanhSachLuaChon[i] << "   ";
            }
            auto SuKienPhimMoi = tui::DocPhim();
            if (SuKienPhimMoi.Phim == tui::PhimTrai) {
                LuaChon = (LuaChon + (int)DanhSachLuaChon.size() - 1) % (int)DanhSachLuaChon.size();
            }
            else if (SuKienPhimMoi.Phim == tui::PhimPhai) {
                LuaChon = (LuaChon + 1) % (int)DanhSachLuaChon.size();
            }
            else if (SuKienPhimMoi.Phim == tui::PhimEnter) {
                return LuaChon;
            }
            else if (SuKienPhimMoi.Phim == tui::PhimEsc) {
                return LuaChon;
            }
        }
    }
    inline void XoaDongTaiViTri(int X, int Y, int ChieuRong) {
        tui::DiChuyenConTro(X, Y);
        std::cout << std::string(ChieuRong, ' ');
    }
    inline int DocDongChoPhepThoatKhiRong(int X, int Y, int DoDaiToiDa, std::string& KetQuaDauRa) {
        KetQuaDauRa.clear();
        tui::DiChuyenConTro(X, Y);
        tui::HienConTro();
        while (true) {
            tui::KeyEvent SuKienPhimMoi = tui::DocPhim();
            if (SuKienPhimMoi.Phim == tui::PhimEsc && KetQuaDauRa.empty()) {
                tui::AnConTro();
                return -1;
            }
            if (SuKienPhimMoi.Phim == tui::PhimEnter) {
                tui::AnConTro();
                return 1;
            }
            if (SuKienPhimMoi.KyTu == 8 || SuKienPhimMoi.KyTu == 127) {
                if (!KetQuaDauRa.empty()) {
                    KetQuaDauRa.pop_back();
                    int Cx = X + (int)KetQuaDauRa.size();
                    tui::DiChuyenConTro(Cx, Y);
                    std::cout << ' ';
                    tui::DiChuyenConTro(Cx, Y);
                }
                continue;
            }
            char Ch = 0;
            if (SuKienPhimMoi.KyTu != 0 && SuKienPhimMoi.Phim != tui::PhimEsc && SuKienPhimMoi.Phim != tui::PhimEnter) {
                Ch = (char)SuKienPhimMoi.KyTu;
            }
            if (Ch >= 32 && Ch < 127) {
                if ((int)KetQuaDauRa.size() < DoDaiToiDa) {
                    KetQuaDauRa.push_back(Ch);
                    std::cout << Ch;
                }
            }
        }
    }
    inline int DocSoNguyenTuyChonHoacThoat(int X, int Y, int GiaTriNhoNhat, int GiaTriLonNhat, int* GiaTriKetQua) {
        std::string ChuoiNhap;
        int KetQuaNhap = DocDongChoPhepThoatKhiRong(X, Y, 16, ChuoiNhap);
        if (KetQuaNhap == -1) {
            return -1;
        }
        if (ChuoiNhap.empty()) {
            return 0;
        }
        try {
            int GiaTriCanThem = std::stoi(ChuoiNhap);
            if (GiaTriCanThem < GiaTriNhoNhat || GiaTriCanThem > GiaTriLonNhat) {
                throw std::out_of_range("range");
            }
            if (GiaTriKetQua) {
                *GiaTriKetQua = GiaTriCanThem;
            }
            return 1;
        }
        catch (...) {
            tui::DiChuyenConTro(X, Y);
            std::cout << std::string(24, ' ');
            tui::DiChuyenConTro(X, Y);
            tui::DatMau(tui::MauCanhBao);
            std::cout << "(so " << GiaTriNhoNhat << "-" << GiaTriLonNhat << ")";
            tui::DatLaiMau();
            return 0;
        }
    }

    //================ Helper functions =================//
    inline const DauSach* TimDauSachConstTheoISBN(const std::vector<DauSach*>& GiaTriThuNhat,
        const std::string& ISBNCanXuLy) {
        for (auto* DuLieuSach : GiaTriThuNhat) {
            if (DuLieuSach && DuLieuSach->ISBN == ISBNCanXuLy) {
                return DuLieuSach;
            }
        }
        return NULL;
    }

    //================ Quản lý độc giả ================//
    //================= Thêm độc giả ==================//
    inline void FormThemDocGiaTUI(DocGiaNode*& Root) {
        const int ChieuRong = 118, ChieuCao = 18, X0 = 4, Y0 = 3;
        const int FooterY = 1 + ChieuCao - 2;
        tui::XoaManHinh();
        tui::VeKhung(2, 1, ChieuRong, ChieuCao, "QUAN LY DOC GIA  >  THEM DOC GIA");
        int MaTheCanXuLy = TaoMaTheKhongTrung(Root);
        int Y = Y0 + 1;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Ma the           : ";
        tui::DiChuyenConTro(X0 + 19, Y);
        std::cout << MaTheCanXuLy;
        Y += 2;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Ho va Ten Dem    : ";
        int HoX = X0 + 19, HoY = Y;
        Y += 2;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Ten              : ";
        int TenX = X0 + 19, TenY = Y;
        Y += 2;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Gioi tinh        : ";
        int GtX = X0 + 19, GtY = Y;
        Y += 2;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Trang thai       : ";
        int TtX = X0 + 19, TtY = Y;
        Y += 2;
        tui::InHuongDanCuoiTrang(4, FooterY, "[Enter] Chon/Luu  -  [Esc] Quay lai  -  (Left/Right) doi tuy chon");
        LamSachBoDemNhap();
        // --- Nhập Họ và Tên đệm ---
        std::string HoNhap;
        while (true) {
            int KetQuaNhap = DocDongChoPhepThoatKhiRong(HoX, HoY, 60, HoNhap);
            if (KetQuaNhap == -1) {
                return;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << std::string(90, ' ');
            if (!CatKhoangTrangHaiDau(HoNhap).empty() && KiemTraTenHopLe(HoNhap)) {
                break;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            if (CatKhoangTrangHaiDau(HoNhap).empty()) {
                std::cout << "Ho va Ten Dem khong duoc de trong.";
            }
            else {
                std::cout << "Ho va Ten Dem khong duoc chua so hoac ky tu dac biet.";
            }
            tui::DatLaiMau();
            XoaDongTaiViTri(HoX, HoY, 60);
        }
        // --- Nhập Tên ---
        std::string TenNhap;
        while (true) {
            int KetQuaNhap = DocDongChoPhepThoatKhiRong(TenX, TenY, 60, TenNhap);
            if (KetQuaNhap == -1) {
                return;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << std::string(90, ' ');
            if (!CatKhoangTrangHaiDau(TenNhap).empty() && KiemTraTenHopLe(TenNhap)) {
                break;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            if (CatKhoangTrangHaiDau(TenNhap).empty()) {
                std::cout << "Ten khong duoc de trong.";
            }
            else {
                std::cout << "Ten khong duoc chua so hoac ky tu dac biet.";
            }
            tui::DatLaiMau();
            XoaDongTaiViTri(TenX, TenY, 60);
        }
        tui::DiChuyenConTro(X0, FooterY - 2);
        std::cout << std::string(90, ' ');
        int SelGT = ChonRadioTaiViTri(GtX, GtY, { "Nam", "Nu" }, 0);
        std::string PhaiNhap = (SelGT == 1 ? "Nu" : "Nam");
        int SelTT = ChonRadioTaiViTri(TtX, TtY, { "Hoat dong", "Khoa" }, 0);
        int TrangThaiNhap = (SelTT == 0 ? 1 : 0);
        DocGia DocGiaCanXuLy;
        DocGiaCanXuLy.MaThe = MaTheCanXuLy;
        SaoChepChuoi(DocGiaCanXuLy.Ho, 50, ChuanHoaChuoi(HoNhap));
        SaoChepChuoi(DocGiaCanXuLy.Ten, 30, ChuanHoaChuoi(TenNhap));
        SaoChepChuoi(DocGiaCanXuLy.Phai, 5, PhaiNhap);
        DocGiaCanXuLy.TrangThaiThe = TrangThaiNhap;
        DocGiaCanXuLy.MuonTraHead = NULL;
        ThemDocGiaVaoCay(Root, DocGiaCanXuLy);
        tui::DiChuyenConTro(X0, FooterY - 2);
        tui::DatMau(tui::MauThanhCong);
        std::cout << "Da them doc gia ma the " << MaTheCanXuLy << ".";
        tui::DatLaiMau();
        tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
    }
    //================== Xóa độc giả ==================//
    inline void FormXoaDocGiaTUI(DocGiaNode*& Root) {
        const int ChieuRong = 118, ChieuCao = 12, X0 = 4, Y0 = 3;
        const int FooterY = 1 + ChieuCao - 2;
        tui::XoaManHinh();
        tui::VeKhung(2, 1, ChieuRong, ChieuCao, "QUAN LY DOC GIA  >  XOA DOC GIA");
        int Y = Y0 + 1;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Nhap ma the muon xoa : ";
        int MaX = X0 + 24, MaY = Y;
        tui::InHuongDanCuoiTrang(4, FooterY, "[Enter] Xac nhan  -  [Esc] Quay lai");
        LamSachBoDemNhap();
        int MaTheCanXuLy = -1;
        while (true) {
            std::string SMa;
            int KetQuaDoc = DocDongChoPhepThoatKhiRong(MaX, MaY, 12, SMa);
            if (KetQuaDoc == -1) {
                return;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << std::string(90, ' ');
            if (SMa.empty()) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Ma the khong duoc de trong.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(MaX, MaY);
                std::cout << std::string(12, ' ');
                continue;
            }
            if (!KiemTraToanChuSo(SMa)) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: Ma the chi duoc chua so.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(MaX, MaY);
                std::cout << std::string(12, ' ');
                continue;
            }
            try {
                MaTheCanXuLy = std::stoi(SMa);
            }
            catch (...) {
                MaTheCanXuLy = -1;
            }
            break;
        }
        if (MaTheCanXuLy <= 0 || TimDocGiaTheoMaThe(Root, MaTheCanXuLy) == NULL) {
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            std::cout << "Ma the khong hop le hoac khong ton tai.";
            tui::DatLaiMau();
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        bool Ok = XoaDocGiaNeuKhongMuonSach(Root, MaTheCanXuLy);
        tui::DiChuyenConTro(X0, FooterY - 2);
        if (Ok) {
            tui::DatMau(tui::MauThanhCong);
            std::cout << "Da xoa doc gia.";
        }
        else {
            tui::DatMau(tui::MauCanhBao);
            std::cout << "Khong the xoa: doc gia dang muon sach.";
        }
        tui::DatLaiMau();
        tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
    }
    //============== Sửa thông tin độc giả ===========//
    inline void FormSuaDocGiaTUI(DocGiaNode*& Root) {
        const int ChieuRong = 118, ChieuCao = 22, X0 = 4, Y0 = 3;
        const int FooterY = 1 + ChieuCao - 2;
        tui::XoaManHinh();
        tui::VeKhung(2, 1, ChieuRong, 10, "QUAN LY DOC GIA  >  CAP NHAT DOC GIA");
        int YAsk = Y0 + 1;
        tui::DiChuyenConTro(X0, YAsk);
        std::cout << "Nhap ma the can cap nhat : ";
        int MaX = X0 + 28, MaY = YAsk;
        tui::InHuongDanCuoiTrang(4, 10 - 1, "[Enter] Xac nhan  -  [Esc] Quay lai");
        LamSachBoDemNhap();
        int MaTheCanXuLy = -1;
        while (true) {
            std::string SMa;
            int KetQuaDocMaThe = DocDongChoPhepThoatKhiRong(MaX, MaY, 12, SMa);
            if (KetQuaDocMaThe == -1) {
                return;
            }
            SMa = CatKhoangTrangHaiDau(SMa);
            tui::DiChuyenConTro(X0, 8);
            std::cout << std::string(90, ' ');
            if (SMa.empty()) {
                tui::DiChuyenConTro(X0, 8);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Ma the khong duoc de trong.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(MaX, MaY);
                std::cout << std::string(12, ' ');
                continue;
            }
            if (!KiemTraToanChuSo(SMa)) {
                tui::DiChuyenConTro(X0, 8);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: Ma the chi duoc chua so.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(MaX, MaY);
                std::cout << std::string(12, ' ');
                continue;
            }
            try {
                MaTheCanXuLy = std::stoi(SMa);
            }
            catch (...) {
                MaTheCanXuLy = -1;
            }
            break;
        }
        DocGiaNode* ConTroHienTai = TimDocGiaTheoMaThe(Root, MaTheCanXuLy);
        if (!ConTroHienTai) {
            const int FooterHintY = 9;
            const int MsgY = FooterHintY - 1;
            const int ErrY = MsgY - 1;
            tui::DiChuyenConTro(X0, ErrY);
            tui::DatMau(tui::MauCanhBao);
            std::cout << "Khong tim thay ma the " << MaTheCanXuLy << ".";
            tui::DatLaiMau();
            tui::NhanPhimBatKyDeQuayLai(4, MsgY);
            return;
        }
        tui::XoaManHinh();
        tui::VeKhung(2, 1, ChieuRong, ChieuCao, "QUAN LY DOC GIA  >  CAP NHAT DOC GIA");
        int Y = Y0 + 1;
        tui::DiChuyenConTro(X0, Y++);
        std::cout << "(Bo trong neu khong thay doi)";
        Y++;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Ho va Ten Dem    : ";
        int HoX = X0 + 19, HoY = Y;
        Y += 2;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Ten              : ";
        int TenX = X0 + 19, TenY = Y;
        Y += 2;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Gioi tinh        : ";
        int GtX = X0 + 19, GtY = Y;
        Y += 2;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Trang thai       : ";
        int TtX = X0 + 19, TtY = Y;
        Y += 2;
        tui::InHuongDanCuoiTrang(4, FooterY, "[Enter] Chon/Luu  -  [Esc] Quay lai  -  (Left/Right) doi");
        LamSachBoDemNhap();
        // --- Nhập Họ ---
        std::string InHo;
        while (true) {
            int KetQuaNhap = DocDongChoPhepThoatKhiRong(HoX, HoY, 60, InHo);
            if (KetQuaNhap == -1) {
                return;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << std::string(90, ' ');
            if (InHo.empty() || KiemTraTenHopLe(InHo)) {
                break;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            std::cout << "Ho khong duoc chua so hoac ky tu dac biet.";
            tui::DatLaiMau();
            XoaDongTaiViTri(HoX, HoY, 60);
        }
        // --- Nhập Tên ---
        std::string InTen;
        while (true) {
            int KetQuaNhap = DocDongChoPhepThoatKhiRong(TenX, TenY, 60, InTen);
            if (KetQuaNhap == -1) {
                return;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << std::string(90, ' ');
            if (InTen.empty() || KiemTraTenHopLe(InTen)) {
                break;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            std::cout << "Ten khong duoc chua so hoac ky tu dac biet.";
            tui::DatLaiMau();
            XoaDongTaiViTri(TenX, TenY, 60);
        }
        tui::DiChuyenConTro(X0, FooterY - 2);
        std::cout << std::string(90, ' ');
        int SelGT = (std::strcmp(ConTroHienTai->ThongTin.Phai, "Nu") == 0) ? 1 : 0;
        SelGT = ChonRadioTaiViTri(GtX, GtY, { "Nam", "Nu" }, SelGT);
        std::string NewPhai = (SelGT == 1 ? "Nu" : "Nam");
        int SelTT = (ConTroHienTai->ThongTin.TrangThaiThe == 1) ? 0 : 1;
        SelTT = ChonRadioTaiViTri(TtX, TtY, { "Hoat dong", "Khoa" }, SelTT);
        int NewTrangThai = (SelTT == 0 ? 1 : 0);
        // Xác nhận
        int ConfirmY = Y;
        tui::DiChuyenConTro(X0, ConfirmY);
        std::cout << "Xac nhan cap nhat: ";
        int Ok = ChonRadioTaiViTri(X0 + 22, ConfirmY, { "Co", "Khong" }, 0);
        if (Ok != 0) {
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << "Da huy cap nhat doc gia.";
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        // Áp dụng
        if (!InHo.empty()) {
            SaoChepChuoi(ConTroHienTai->ThongTin.Ho, 50, ChuanHoaChuoi(InHo));
        }
        if (!InTen.empty()) {
            SaoChepChuoi(ConTroHienTai->ThongTin.Ten, 30, ChuanHoaChuoi(InTen));
        }
        SaoChepChuoi(ConTroHienTai->ThongTin.Phai, 5, NewPhai);
        ConTroHienTai->ThongTin.TrangThaiThe = NewTrangThai;
        // Thông báo kết quả
        tui::DiChuyenConTro(X0, FooterY - 2);
        std::cout << "Da cap nhat doc gia.";
        tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
    }
    //==================== In danh sách độc giả ====================//
    static const int DoRongCotMATHE = 9, DoRongCotHODEM = 32, DoRongCotTEN = 12, DoRongCotGIOITINH = 9,
        DoRongCotTRANGTHAI = 11;
    inline std::string CanLeChuoi(const std::string& ChuoiNhap, int ChieuRong) {
        std::string ThoiGianHienTai = ChuoiNhap;
        if ((int)ThoiGianHienTai.size() > ChieuRong) {
            ThoiGianHienTai = ThoiGianHienTai.substr(0, ChieuRong);
        }
        return ThoiGianHienTai + std::string(ChieuRong - (int)ThoiGianHienTai.size(), ' ');
    }
    inline int VeTieuDeBangDocGia(const std::string& /*title*/, int /*w*/ = 118, int /*h*/ = 24) {
        int Y = 5;
        tui::DatMau(tui::MauNoiBat);
        tui::DiChuyenConTro(4, Y++);
        std::cout << CanLeChuoi("Ma the", DoRongCotMATHE) << " | " << CanLeChuoi("Ho va Ten Dem", DoRongCotHODEM) << " | "
            << CanLeChuoi("Ten", DoRongCotTEN) << " | " << CanLeChuoi("Gioi tinh", DoRongCotGIOITINH) << " | "
            << CanLeChuoi("Trang thai", DoRongCotTRANGTHAI);
        auto Dash = [](int SoPhanTu) {
            return std::string(SoPhanTu, '-');
            };
        tui::DiChuyenConTro(4, Y++);
        std::cout << Dash(DoRongCotMATHE) << "-+-" << Dash(DoRongCotHODEM) << "-+-" << Dash(DoRongCotTEN) << "-+-"
            << Dash(DoRongCotGIOITINH) << "-+-" << Dash(DoRongCotTRANGTHAI);
        return Y;
    }
    inline void InBangDocGiaTUI(DocGia* DanhSachDong[], int SoPhanTu, const std::string& TieuDe) {
        const int ChieuRong = 118, ChieuCao = 24;
        const int FooterY = 1 + ChieuCao - 2;
        const int PAGE = 15;
        const int TongSoLuong = SoPhanTu;
        const int TongSoTrang = (TongSoLuong == 0 ? 1 : (TongSoLuong + PAGE - 1) / PAGE);
        int Trang = 0;
        tui::XoaManHinh();
        tui::VeKhung(2, 1, ChieuRong, ChieuCao, TieuDe);
        tui::InHuongDanCuoiTrang(4, FooterY, "[Up/Down] Trang truoc/sau   -   [Esc] Quay lai");
        tui::DatMau(tui::MauNoiBat);
        const int DataY = VeTieuDeBangDocGia(TieuDe, ChieuRong, ChieuCao);
        const int DataH = FooterY - DataY - 1;
        const int DataW = ChieuRong - 8;
        auto PaintPageHint = [&](int ChiSoHienTai) {
            tui::DiChuyenConTro(4, 3);
            std::cout << "(Trang " << (ChiSoHienTai + 1) << "/" << TongSoTrang << ")" << std::string(20, ' ');
            };
        auto PaintPage = [&](int ChiSoHienTai) {
            PaintPageHint(ChiSoHienTai);
            tui::XoaVung(4, DataY, DataW, DataH);
            int ChiSoBatDau = ChiSoHienTai * PAGE;
            int ChiSoKetThuc = std::min(TongSoLuong, ChiSoBatDau + PAGE);
            int Y = DataY;
            for (int i = ChiSoBatDau; i < ChiSoKetThuc; ++i) {
                const DocGia* DocGiaCanXuLy = DanhSachDong[i];
                tui::DiChuyenConTro(4, Y++);
                std::cout << CanLeChuoi(std::to_string(DocGiaCanXuLy->MaThe), DoRongCotMATHE) << " | "
                    << CanLeChuoi(DocGiaCanXuLy->Ho, DoRongCotHODEM) << " | "
                    << CanLeChuoi(DocGiaCanXuLy->Ten, DoRongCotTEN) << " | "
                    << CanLeChuoi(DocGiaCanXuLy->Phai, DoRongCotGIOITINH) << " | "
                    << CanLeChuoi((DocGiaCanXuLy->TrangThaiThe == 1 ? "Hoat dong" : "Khoa"), DoRongCotTRANGTHAI);
            }
            };
        PaintPage(Trang);
        while (true) {
            tui::KeyEvent SuKienPhimMoi = tui::DocPhim();
            if (SuKienPhimMoi.Phim == tui::PhimEsc) {
                return;
            }
            if (SuKienPhimMoi.Phim == tui::PhimLen) {
                if (Trang > 0) {
                    --Trang;
                    PaintPage(Trang);
                }
            }
            if (SuKienPhimMoi.Phim == tui::PhimXuong) {
                if (Trang + 1 < TongSoTrang) {
                    ++Trang;
                    PaintPage(Trang);
                }
            }
        }
    }
    // IN ĐỘC GIẢ THEO TÊN + HỌ (Sử dụng mảng tĩnh + QuickSort)
    inline void InDocGiaTheoTenHoTUI(DocGiaNode* Root) {
        static DocGia* MangDuLieu[MaxDocGia];
        int SoPhanTu = 0;
        DuyetLNRChuyenThanhMang(Root, MangDuLieu, SoPhanTu);
        if (SoPhanTu > 0) {
            QuickSortDocGiaTheoTenHo(MangDuLieu, 0, SoPhanTu - 1);
        }
        InBangDocGiaTUI(MangDuLieu, SoPhanTu, "QUAN LY DOC GIA  >  IN DANH SACH  (sap theo Ten + Ho)");
    }
    // IN ĐỘC GIẢ THEO MÃ THẺ (Sử dụng mảng tĩnh + QuickSort)
    inline void InDocGiaTheoMaTheTUI(DocGiaNode* Root) {
        static DocGia* MangDuLieu[MaxDocGia];
        int SoPhanTu = 0;
        DuyetLNRChuyenThanhMang(Root, MangDuLieu, SoPhanTu);
        if (SoPhanTu > 0) {
            QuickSortDocGiaTheoMaThe(MangDuLieu, 0, SoPhanTu - 1);
        }
        InBangDocGiaTUI(MangDuLieu, SoPhanTu, "QUAN LY DOC GIA  >  IN DANH SACH  (sap theo Ma the)");
    }

    //============= Quản lý đầu sách ====================//
    //================ Thêm đầu sách ===================//
    inline void FormThemDauSachTUI(DanhSachDauSach& DanhSachDauSach) {
        const int ChieuRong = 118, ChieuCao = 20, X0 = 4, Y0 = 3;
        const int FooterY = 1 + ChieuCao - 2;
        tui::XoaManHinh();
        tui::VeKhung(2, 1, ChieuRong, ChieuCao, "QUAN LY DAU SACH  >  THEM DAU SACH");
        std::string ISBNCanXuLy = TaoISBNKhongTrung(DanhSachDauSach);
        int Y = Y0 + 1;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "ISBN          : ";
        tui::DiChuyenConTro(X0 + 16, Y);
        std::cout << ISBNCanXuLy;
        Y += 2;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Ten sach      : ";
        int TenX = X0 + 16, TenY = Y;
        Y += 2;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "So trang      : ";
        int StX = X0 + 16, StY = Y;
        Y += 2;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Tac gia       : ";
        int TgX = X0 + 16, TgY = Y;
        Y += 2;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Nam xuat ban  : ";
        int NamX = X0 + 16, NamY = Y;
        Y += 2;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "The loai      : ";
        int TlX = X0 + 16, TlY = Y;
        Y += 2;
        int X1 = X0 + 68;
        int Y2 = Y0 + 1;
        tui::DiChuyenConTro(X1, Y2);
        std::cout << "So luong ban sao : ";
        int SlX = X1 + 19, SlY = Y2;
        Y2 += 2;
        tui::InHuongDanCuoiTrang(4, FooterY, "[Enter] Luu  -  [Esc] Quay lai");
        LamSachBoDemNhap();
        // 1. Nhập Tên Sách
        std::string TenNhap;
        while (true) {
            int KetQuaNhap = DocDongChoPhepThoatKhiRong(TenX, TenY, 45, TenNhap);
            if (KetQuaNhap == -1) {
                return;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << std::string(90, ' ');
            if (CatKhoangTrangHaiDau(TenNhap).empty()) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Ten sach khong duoc de trong.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(TenX, TenY);
                std::cout << std::string(45, ' ');
                continue;
            }
            break;
        }
        // 2. Nhập Số Trang
        int SoTrangNhap = 0;
        while (true) {
            std::string ChuoiNhap;
            int KetQuaNhap = DocDongChoPhepThoatKhiRong(StX, StY, 10, ChuoiNhap);
            if (KetQuaNhap == -1) {
                return;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << std::string(90, ' ');
            if (ChuoiNhap.empty()) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "So trang khong duoc de trong.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(StX, StY);
                std::cout << std::string(10, ' ');
                continue;
            }
            if (!KiemTraToanChuSo(ChuoiNhap)) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: So trang chi duoc chua so.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(StX, StY);
                std::cout << std::string(10, ' ');
                continue;
            }
            try {
                SoTrangNhap = std::stoi(ChuoiNhap);
                if (SoTrangNhap < 1 || SoTrangNhap > 5000) {
                    throw std::out_of_range("range");
                }
                break;
            }
            catch (...) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: So trang phai tu 1 den 5000.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(StX, StY);
                std::cout << std::string(10, ' ');
            }
        }
        // 3. Nhập Tác Giả
        std::string TacGiaNhap;
        while (true) {
            int KetQuaNhap = DocDongChoPhepThoatKhiRong(TgX, TgY, 45, TacGiaNhap);
            if (KetQuaNhap == -1) {
                return;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << std::string(90, ' ');
            if (CatKhoangTrangHaiDau(TacGiaNhap).empty()) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Tac gia khong duoc de trong.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(TgX, TgY);
                std::cout << std::string(45, ' ');
                continue;
            }
            if (!KiemTraTenHopLe(TacGiaNhap)) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: Tac gia khong duoc chua so hoac ky tu dac biet.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(TgX, TgY);
                std::cout << std::string(45, ' ');
                continue;
            }
            break;
        }
        // 4. Nhập năm xuất bản
        std::time_t ThoiGianHienTai = std::time(NULL);
        std::tm Lt{};
#ifdef _WIN32
        localtime_s(&Lt, &ThoiGianHienTai);
#else
        std::tm* ConTroHienTai = std::localtime(&ThoiGianHienTai);
        if (ConTroHienTai) {
            Lt = *ConTroHienTai;
        }
#endif
        int NamHienTai = LayNgayHienTai().Nam;
        int NamXuatBanNhap = 0;
        while (true) {
            std::string ChuoiNhap;
            int KetQuaNhap = DocDongChoPhepThoatKhiRong(NamX, NamY, 10, ChuoiNhap);
            if (KetQuaNhap == -1) {
                return;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << std::string(90, ' ');
            if (ChuoiNhap.empty()) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Nam xuat ban khong duoc de trong.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(NamX, NamY);
                std::cout << std::string(10, ' ');
                continue;
            }
            if (!KiemTraToanChuSo(ChuoiNhap)) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: Nam xuat ban khong duoc chua ky tu dac biet.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(NamX, NamY);
                std::cout << std::string(10, ' ');
                continue;
            }
            try {
                int GiaTriCanThem = std::stoi(ChuoiNhap);
                if (GiaTriCanThem < 1500 || GiaTriCanThem > NamHienTai) {
                    tui::DiChuyenConTro(X0, FooterY - 2);
                    tui::DatMau(tui::MauCanhBao);
                    std::cout << "Loi: Nam xuat ban kho hop le. ";
                    tui::DatLaiMau();
                    tui::DiChuyenConTro(NamX, NamY);
                    std::cout << std::string(10, ' ');
                    continue;
                }
                NamXuatBanNhap = GiaTriCanThem;
                break;
            }
            catch (...) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: Nam xuat ban kho hop le .";
                tui::DatLaiMau();
                tui::DiChuyenConTro(NamX, NamY);
                std::cout << std::string(10, ' ');
            }
        }
        // 5. Nhập Thể Loại
        std::string TheLoaiNhap;
        while (true) {
            int KetQuaNhap = DocDongChoPhepThoatKhiRong(TlX, TlY, 48, TheLoaiNhap);
            if (KetQuaNhap == -1) {
                return;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << std::string(90, ' ');
            if (CatKhoangTrangHaiDau(TheLoaiNhap).empty()) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "The loai khong duoc de trong.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(TlX, TlY);
                std::cout << std::string(48, ' ');
                continue;
            }
            if (!KiemTraTenHopLe(TheLoaiNhap)) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: The loai khong duoc chua so hoac ky tu dac biet.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(TlX, TlY);
                std::cout << std::string(48, ' ');
                continue;
            }
            break;
        }
        // 6. Nhập Số Lượng Bản Sao
        int SoLuongCanXuLy = 0;
        while (true) {
            std::string ChuoiNhap;
            int KetQuaNhap = DocDongChoPhepThoatKhiRong(SlX, SlY, 10, ChuoiNhap);
            if (KetQuaNhap == -1) {
                return;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << std::string(90, ' ');
            if (ChuoiNhap.empty()) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "So luong ban sao khong duoc de trong.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(SlX, SlY);
                std::cout << std::string(10, ' ');
                continue;
            }
            if (!KiemTraToanChuSo(ChuoiNhap)) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: So luong chi duoc chua so.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(SlX, SlY);
                std::cout << std::string(10, ' ');
                continue;
            }
            try {
                SoLuongCanXuLy = std::stoi(ChuoiNhap);
                if (SoLuongCanXuLy < 1 || SoLuongCanXuLy > 5000) {
                    throw std::out_of_range("range");
                }
                break;
            }
            catch (...) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: So luong phai tu 1 den 5000.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(SlX, SlY);
                std::cout << std::string(10, ' ');
            }
        }        
        // 9.  Lưu dữ liệu
        DauSach* Item = new DauSach();
        SaoChepChuoi(Item->ISBN, 15, ISBNCanXuLy);
        SaoChepChuoi(Item->TenSach, 100, ChuanHoaChuoi(TenNhap));
        SaoChepChuoi(Item->TacGia, 60, ChuanHoaChuoi(TacGiaNhap));
        SaoChepChuoi(Item->TheLoai, 40, ChuanHoaChuoi(TheLoaiNhap));
        Item->SoTrang = SoTrangNhap;
        Item->NamXuatBan = NamXuatBanNhap;
        Item->DanhMucSachHead = NULL;
        Item->SoLuongBanSao = 0;
        Item->SoLuotMuon = 0;
        TaoBanSaoTuDong(Item, SoLuongCanXuLy);
        if (ChenDauSachTheoTen(DanhSachDauSach, Item)) {
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauThanhCong);
            std::cout << "Da them dau sach [" << ISBNCanXuLy << "].";
        }
        else {
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            std::cout << "Loi: Danh sach da day (" << MaxDauSach << " cuon).";
            delete Item; // Nhớ xóa nếu thêm thất bại
        }
        tui::DatLaiMau();
        tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
    }
    //================ Xóa đầu sách =================//
    inline void FormXoaDauSachTUI(DanhSachDauSach& DanhSachDauSach, DocGiaNode* Root) {
        const int ChieuRong = 90, ChieuCao = 16, X0 = 4, Y0 = 3;
        const int FooterY = 1 + ChieuCao - 2;
        tui::XoaManHinh();
        tui::VeKhung(2, 1, ChieuRong, ChieuCao, "QUAN LY DAU SACH  >  XOA DAU SACH");
        int Y = Y0 + 1;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Nhap ISBN: ";
        int ISBNX = X0 + 12, ISBNY = Y;
        Y += 1;
        tui::InHuongDanCuoiTrang(4, FooterY, "[Enter] Xac nhan  -  [Esc] Quay lai");
        LamSachBoDemNhap();
        std::string ISBNCanXuLy;
        if (DocDongChoPhepThoatKhiRong(ISBNX, ISBNY, 20, ISBNCanXuLy) == -1) {
            return;
        }
        DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy);
        if (!DuLieuSach) {
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            std::cout << "Khong tim thay ISBN.";
            tui::DatLaiMau();
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        int Tong = DemTongSoBanSao(DuLieuSach);
        int DangMuon = DemSoSachDangMuonTheoISBN(Root, ISBNCanXuLy);
        tui::DiChuyenConTro(X0, Y++);
        std::cout << "Tong so ban sao : " << Tong;
        tui::DiChuyenConTro(X0, Y++);
        std::cout << "Dang cho muon   : " << DangMuon;
        if (DangMuon > 0) {
            tui::DiChuyenConTro(X0, Y + 1);
            tui::DatMau(tui::MauCanhBao);
            std::cout << "Khong the xoa: co ban sao dang cho muon.";
            tui::DatLaiMau();
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        tui::DiChuyenConTro(X0, Y + 1);
        std::cout << "Xac nhan xoa dau sach: ";
        int LuaChon = ChonRadioTaiViTri(X0 + 26, Y + 1, { "Xoa", "Huy" }, 1);
        if (LuaChon != 0) {
            tui::DiChuyenConTro(X0, Y + 3);
            std::cout << "Da huy.";
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        std::string Err;
        if (XoaDauSach(DanhSachDauSach, ISBNCanXuLy, &Err)) {
            tui::DiChuyenConTro(X0, Y + 3);
            tui::DatMau(tui::MauThanhCong);
            std::cout << "Da xoa dau sach.";
        }
        else {
            tui::DiChuyenConTro(X0, Y + 3);
            tui::DatMau(tui::MauCanhBao);
            std::cout << "Loi: " << Err;
        }
        tui::DatLaiMau();
        tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
    }
    //========= Cập nhật thông tin đầu sách ==========//
    inline void FormCapNhatDauSachTUI(DanhSachDauSach& DanhSachDauSach) {
        const int ChieuRong = 118, ChieuCao = 26, X0 = 4, Y0 = 3;
        const int FooterY = 1 + ChieuCao - 2;
        tui::XoaManHinh();
        tui::VeKhung(2, 1, ChieuRong, ChieuCao, "QUAN LY DAU SACH  >  CAP NHAT THONG TIN");
        int Y = Y0 + 1;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Nhap ISBN        : ";
        int ISBNX = X0 + 18, ISBNY = Y;
        Y += 2;
        tui::DiChuyenConTro(X0, Y++);
        std::cout << "(Bo trong neu khong doi)";
        Y++;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "TEN SACH         : ";
        int TenX = X0 + 18, TenY = Y;
        Y += 2;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "TAC GIA          : ";
        int TgX = X0 + 18, TgY = Y;
        Y += 2;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "NAM XUAT BAN     : ";
        int NamX = X0 + 18, NamY = Y;
        Y += 2;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "SO TRANG         : ";
        int StX = X0 + 18, StY = Y;
        Y += 2;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "SO LUONG BAN SAO : ";
        int SlX = X0 + 18, SlY = Y;
        Y += 2;
        tui::InHuongDanCuoiTrang(4, FooterY, "[Enter] Luu  -  [Esc] Quay lai");
        LamSachBoDemNhap();
        std::string ISBNCanXuLy;
        if (DocDongChoPhepThoatKhiRong(ISBNX, ISBNY, 20, ISBNCanXuLy) == -1) {
            return;
        }
        DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy);
        if (!DuLieuSach) {
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            std::cout << "Khong tim thay ISBN.";
            tui::DatLaiMau();
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        std::string InTen;
        if (DocDongChoPhepThoatKhiRong(TenX, TenY, 60, InTen) == -1) {
            return;
        }
        // 1. Nhập Tác Giả
        std::string InTG;
        while (true) {
            int KetQuaNhap = DocDongChoPhepThoatKhiRong(TgX, TgY, 60, InTG);
            if (KetQuaNhap == -1) {
                return;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << std::string(90, ' ');
            if (InTG.empty()) {
                break;
            }
            if (!KiemTraTenHopLe(InTG)) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: Tac gia khong duoc chua so hoac ky tu dac biet.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(TgX, TgY);
                std::cout << std::string(60, ' ');
                continue;
            }
            break;
        }
        // 2. Nhập năm xuât bản
        int NamHienTai = LayNgayHienTai().Nam;
        int InNam = 0;
        while (true) {
            std::string ChuoiNhap;
            int KetQuaNhap = DocDongChoPhepThoatKhiRong(NamX, NamY, 10, ChuoiNhap);
            if (KetQuaNhap == -1) {
                return;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << std::string(90, ' ');
            if (ChuoiNhap.empty()) {
                InNam = 0;
                break;
            }
            if (!KiemTraToanChuSo(ChuoiNhap)) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: Nam xuat ban khong duoc chua ky tu dac biet.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(NamX, NamY);
                std::cout << std::string(10, ' ');
                continue;
            }
            try {
                int GiaTriCanThem = std::stoi(ChuoiNhap);
                if (GiaTriCanThem < 1500 || GiaTriCanThem > NamHienTai) {
                    tui::DiChuyenConTro(X0, FooterY - 2);
                    tui::DatMau(tui::MauCanhBao);
                    std::cout << "Loi: Nam xuat ban kho hop le .";
                    tui::DatLaiMau();
                    tui::DiChuyenConTro(NamX, NamY);
                    std::cout << std::string(10, ' ');
                    continue;
                }
                InNam = GiaTriCanThem;
                break;
            }
            catch (...) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: Nam xuat ban kho hop le .";
                tui::DatLaiMau();
                tui::DiChuyenConTro(NamX, NamY);
                std::cout << std::string(10, ' ');
            }
        }
        // 3. Nhập Số Trang
        int InST = 0;
        while (true) {
            std::string ChuoiNhap;
            int KetQuaNhap = DocDongChoPhepThoatKhiRong(StX, StY, 10, ChuoiNhap);
            if (KetQuaNhap == -1) {
                return;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << std::string(90, ' ');
            if (ChuoiNhap.empty()) {
                InST = 0;
                break;
            }
            if (!KiemTraToanChuSo(ChuoiNhap)) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: Chi duoc nhap so (khong chu/ky tu la).";
                tui::DatLaiMau();
                tui::DiChuyenConTro(StX, StY);
                std::cout << std::string(10, ' ');
                continue;
            }
            try {
                int GiaTriCanThem = std::stoi(ChuoiNhap);
                if (GiaTriCanThem < 1 || GiaTriCanThem > 5000) {
                    throw std::out_of_range("range");
                }
                InST = GiaTriCanThem;
                break;
            }
            catch (...) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: So trang phai tu 1 den 5000.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(StX, StY);
                std::cout << std::string(10, ' ');
            }
        }
        // 4. Nhập Số Lượng
        int SlTarget = 0;
        int HasSL = 0;
        // Tính số sách đang được mượn
        int DaMuon = DemSoSachDangMuon(DuLieuSach);
        while (true) {
            std::string ChuoiNhap;
            int KetQuaNhap = DocDongChoPhepThoatKhiRong(SlX, SlY, 10, ChuoiNhap);
            if (KetQuaNhap == -1) {
                return;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << std::string(90, ' ');
            if (ChuoiNhap.empty()) {
                HasSL = 0;
                break;
            }
            if (!KiemTraToanChuSo(ChuoiNhap)) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: Chi duoc nhap so (khong chu/ky tu la).";
                tui::DatLaiMau();
                tui::DiChuyenConTro(SlX, SlY);
                std::cout << std::string(10, ' ');
                continue;
            }
            try {
                int GiaTriCanThem = std::stoi(ChuoiNhap);
                if (GiaTriCanThem < 1 || GiaTriCanThem > 5000) {
                    throw std::out_of_range("range");
                }
                if (GiaTriCanThem < DaMuon) { // Nếu số lượng mới nhỏ hơn số lượng đang mượn thì báo lỗi
                    tui::DiChuyenConTro(X0, FooterY - 2);
                    tui::DatMau(tui::MauCanhBao);
                    std::cout << "Loi: Khong the giam vi co " << DaMuon << " cuon dang duoc muon.";
                    tui::DatLaiMau();
                    tui::DiChuyenConTro(SlX, SlY);
                    std::cout << std::string(10, ' ');
                    continue;
                }
                SlTarget = GiaTriCanThem;
                HasSL = 1;
                break;
            }
            catch (...) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: So luong phai tu 1 den 5000.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(SlX, SlY);
                std::cout << std::string(10, ' ');
            }
        }        
        // 7. Xác nhận cập nhật
        int ConfirmY = SlY + 2;
        if (ConfirmY > FooterY - 3) {
            ConfirmY = FooterY - 3;
        }
        tui::DiChuyenConTro(X0, ConfirmY);
        std::cout << "Xac nhan cap nhat: ";
        int Ok = ChonRadioTaiViTri(X0 + 22, ConfirmY, { "Co", "Khong" }, 0);
        // 8. Lưu thay đổi
        if (Ok != 0) {
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << "Da huy cap nhat.";
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        if (!InTen.empty()) {
            SaoChepChuoi(DuLieuSach->TenSach, 100, ChuanHoaChuoi(InTen));
        }
        if (!InTG.empty()) {
            SaoChepChuoi(DuLieuSach->TacGia, 60, ChuanHoaChuoi(InTG));
        }
        if (InNam > 0) {
            DuLieuSach->NamXuatBan = InNam;
        }
        if (InST > 0) {
            DuLieuSach->SoTrang = InST;
        }
        // Thay đổi số lượng
        if (HasSL == 1) {
            if (SlTarget > DuLieuSach->SoLuongBanSao) {
                TaoBanSaoTuDong(
                    DuLieuSach,SlTarget - DuLieuSach->SoLuongBanSao
                );
            }
            else if (SlTarget < DuLieuSach->SoLuongBanSao) {
                (void)GiamBanSaoTuCuoi(
                    DuLieuSach,DuLieuSach->SoLuongBanSao - SlTarget
                );
            }
        }
        if (DanhSachDauSach.SoLuong > 1){
            QuickSortTheoTenSach(DanhSachDauSach.Nodes,0,DanhSachDauSach.SoLuong - 1);
        }
        // 9. Thông báo kết quả
        tui::DiChuyenConTro(X0, FooterY - 2);
        std::cout << "Da cap nhat dau sach.";
        tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
    }
    //================ Danh sách theo thể loại =================//
    inline void InDauSachTheoTheLoaiTUI(const DanhSachDauSach& DanhSachDauSach) {
        const int ChieuRong = 118, ChieuCao = 30, X0 = 4;
        const int FooterY = 1 + ChieuCao - 2;
        const int YMax = 1 + ChieuCao - 3;
        const int MAXLINES = 5000;
        if (DanhSachDauSach.SoLuong == 0) {
            tui::XoaManHinh();
            tui::VeKhung(2, 1, ChieuRong, ChieuCao, "QUAN LY DAU SACH  >  DANH SACH THEO THE LOAI");
            tui::DatMau(tui::MauNoiBat);
            tui::DiChuyenConTro(X0, 5);
            std::cout << "(Khong co dau sach.)";
            tui::InHuongDanCuoiTrang(4, FooterY, "[Esc] Quay lai");
            while (true) {
                tui::KeyEvent SuKienPhim = tui::DocPhim();
                if (SuKienPhim.Phim == tui::PhimEsc) {
                    return;
                }
            }
        }
        static DauSach* MangTam[MaxDauSach];
        int SoPhanTu = 0;
        LayDanhSachSapXepTheoTheLoai(DanhSachDauSach, MangTam, SoPhanTu);
        static std::string CacDongKetQua[MAXLINES];
        int TongSoDong = 0;
        std::string CurrentTheLoai = "";
        for (int i = 0; i < SoPhanTu; i++) {
            DauSach* DuLieuSach = MangTam[i];
            if (DuLieuSach == NULL) {
                continue;
            }
            if (DuLieuSach->TheLoai != CurrentTheLoai) {
                CurrentTheLoai = DuLieuSach->TheLoai;
                int CountGroup = 0;
                for (int k = i; k < SoPhanTu; k++) {
                    if (MangTam[k]->TheLoai == CurrentTheLoai) {
                        CountGroup++;
                    }
                    else {
                        break;
                    }
                }
                if (TongSoDong > 0 && TongSoDong < MAXLINES) {
                    CacDongKetQua[TongSoDong++] = "";
                }
                if (TongSoDong < MAXLINES) {
                    CacDongKetQua[TongSoDong++] =
                        "The loai: " + CurrentTheLoai + " (So dau sach: " + std::to_string(CountGroup) + ")";
                }
            }
            std::string TenNhap = DuLieuSach->TenSach;
            if ((int)TenNhap.size() > 45) {
                TenNhap = TenNhap.substr(0, 45);
            }
            std::string TacGiaTam = DuLieuSach->TacGia;
            if ((int)TacGiaTam.size() > 24) {
                TacGiaTam = TacGiaTam.substr(0, 24);
            }
            int SoBanSao = DemTongSoBanSao(DuLieuSach);
            std::string DongDuLieu =
                std::string("  - [") + DuLieuSach->ISBN + "] " +
                TenNhap + " | " + TacGiaTam + " | " +
                std::to_string(DuLieuSach->NamXuatBan) +
                " | So ban sao: " + std::to_string(SoBanSao);
            if (TongSoDong < MAXLINES) {
                CacDongKetQua[TongSoDong++] = DongDuLieu;
            }
        }
        const int StartY = 5;
        const int CAP = YMax - StartY + 1;
        const int TongSoLuong = TongSoDong;
        const int TongSoTrang = (TongSoLuong + CAP - 1) / CAP;
        int Trang = 0;
        tui::XoaManHinh();
        tui::VeKhung(2, 1, ChieuRong, ChieuCao, "QUAN LY DAU SACH  >  DANH SACH THEO THE LOAI");
        tui::InHuongDanCuoiTrang(4, FooterY, "[Up/Down] Trang truoc/sau   -   [Esc] Quay lai");
        tui::DatMau(tui::MauNoiBat);
        auto PaintPageHint = [&](int ConTroHienTai) {
            tui::DiChuyenConTro(4, 3);
            std::cout << "(Trang " << (ConTroHienTai + 1) << "/" << std::max(1, TongSoTrang) << ")" << std::string(20, ' ');
            };
        auto PaintPage = [&](int ConTroHienTai) {
            PaintPageHint(ConTroHienTai);
            tui::XoaVung(4, StartY, ChieuRong - 8, CAP);
            int Y = StartY;
            int ChiSoDauTrang = ConTroHienTai * CAP;
            int ChiSoCuoiTrang = std::min(TongSoLuong, ChiSoDauTrang + CAP);
            for (int i = ChiSoDauTrang; i < ChiSoCuoiTrang; ++i) {
                tui::DiChuyenConTro(X0, Y++);
                std::cout << CacDongKetQua[i];
            }
            };
        PaintPage(Trang);
        while (true) {
            tui::KeyEvent SuKienPhimMoi = tui::DocPhim();
            if (SuKienPhimMoi.Phim == tui::PhimEsc) {
                return;
            }
            if (SuKienPhimMoi.Phim == tui::PhimLen) {
                if (Trang > 0) {
                    --Trang;
                    PaintPage(Trang);
                }
            }
            if (SuKienPhimMoi.Phim == tui::PhimXuong) {
                if (Trang + 1 < TongSoTrang) {
                    ++Trang;
                    PaintPage(Trang);
                }
            }
        }
    }
    //================ Tìm đầu sách theo tên =================//
    inline void TimDauSachTheoTenTUI(const DanhSachDauSach& DanhSachDauSach) {
        const int ChieuRong = 118, ChieuCao = 30, X0 = 4;
        const int FooterY = 1 + ChieuCao - 2;
        const int YMax = 1 + ChieuCao - 3;
        tui::XoaManHinh();
        tui::VeKhung(2, 1, ChieuRong, ChieuCao, "QUAN LY DAU SACH  >  TIM THEO TEN");
        tui::DatMau(tui::MauNoiBat);
        int Y = 4;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Nhap tu khoa ten sach: ";
        int QX = X0 + 24, QY = Y;
        Y += 2;
        tui::InHuongDanCuoiTrang(4, FooterY, "[Enter] Tim  -  [Esc] Quay lai");
        LamSachBoDemNhap();
        std::string TuKhoaTimKiem;
        if (DocDongChoPhepThoatKhiRong(QX, QY, 64, TuKhoaTimKiem) == -1) {
            return;
        }
        if (TuKhoaTimKiem.empty()) {
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            std::cout << "Tu khoa khong duoc rong.";
            tui::DatLaiMau();
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        DauSach* DaTimThay[MaxDauSach]; // Mảng chứa kết quả tìm thấy
        int SoLuongTimThay = 0;         // Số lượng tìm thấy
        TimDauSachTheoTen(DanhSachDauSach, TuKhoaTimKiem, DaTimThay, SoLuongTimThay);
        static std::string CacDongKetQua[5000];
        int TongSoDong = 0;
        auto FmtLine = [&](const std::string& ChuoiNhap) -> std::string {
            if ((int)ChuoiNhap.size() <= ChieuRong - 8) {
                return ChuoiNhap;
            }
            return ChuoiNhap.substr(0, ChieuRong - 8);
            };
        auto TrangThaiStr = [&](int TrangThaiTam) -> std::string {
            if (TrangThaiTam == 0) {
                return "CHO MUON";
            }
            if (TrangThaiTam == 1) {
                return "DA MUON";
            }
            return "?";
            };
        if (SoLuongTimThay == 0) {
            tui::DiChuyenConTro(X0, 6);
            std::cout << "Khong tim thay dau sach phu hop.";
            tui::InHuongDanCuoiTrang(4, FooterY, "[Esc] Quay lai");
            while (true) {
                tui::KeyEvent SuKienPhim = tui::DocPhim();
                if (SuKienPhim.Phim == tui::PhimEsc) {
                    return;
                }
            }
        }
        for (int i = 0; i < SoLuongTimThay; ++i) {
            if (TongSoDong >= 4990) {
                break;
            }
            DauSach* DuLieuSach = DaTimThay[i];
            CacDongKetQua[TongSoDong++] =
                FmtLine(std::to_string(i + 1) + ". ISBN: " + DuLieuSach->ISBN + " | Ten: " + DuLieuSach->TenSach);
            CacDongKetQua[TongSoDong++] =
                FmtLine(std::string("    Tac gia: ") + DuLieuSach->TacGia +
                    " | Nam: " + std::to_string(DuLieuSach->NamXuatBan) + " | The loai: " + DuLieuSach->TheLoai);
            std::string ChiSoHienTai = "    Ma ban sao: ";
            int CountInLine = 0;
            for (DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead; ConTroHienTai != NULL;
                ConTroHienTai = ConTroHienTai->Next) {
                std::string Token =
                    ConTroHienTai->MaSach + std::string(" (") + TrangThaiStr(ConTroHienTai->TrangThai) + ")";
                if ((int)(ChiSoHienTai.size() + Token.size() + 2) > ChieuRong - 8) {
                    if (TongSoDong < 5000) {
                        CacDongKetQua[TongSoDong++] = FmtLine(ChiSoHienTai);
                    }
                    ChiSoHienTai = "                  " + Token;
                    CountInLine = 1;
                }
                else {
                    if (CountInLine > 0) {
                        ChiSoHienTai += ", ";
                    }
                    ChiSoHienTai += Token;
                    CountInLine++;
                }
            }
            if (TongSoDong < 5000) {
                CacDongKetQua[TongSoDong++] = FmtLine(ChiSoHienTai);
            }
            if (TongSoDong < 5000) {
                CacDongKetQua[TongSoDong++] = "";
            }
        }
        const int StartY = 5;
        const int CAP = YMax - StartY + 1;
        const int TongSoLuong = TongSoDong;
        const int TongSoTrang = (TongSoLuong + CAP - 1) / CAP;
        int Trang = 0;
        tui::XoaManHinh();
        tui::VeKhung(2, 1, ChieuRong, ChieuCao, "QUAN LY DAU SACH  >  TIM THEO TEN");
        tui::InHuongDanCuoiTrang(4, FooterY, "[Up/Down] Trang  -  [Esc] Quay lai");
        tui::DatMau(tui::MauNoiBat);
        auto PaintPageHint = [&](int ConTroHienTai) {
            tui::DiChuyenConTro(4, 3);
            std::cout << "(Trang " << (ConTroHienTai + 1) << "/" << std::max(1, TongSoTrang) << ")" << std::string(20, ' ');
            };
        auto PaintPage = [&](int ConTroHienTai) {
            PaintPageHint(ConTroHienTai);
            tui::XoaVung(4, StartY, ChieuRong - 8, CAP);
            int Y2 = StartY;
            int ChiSoDauTrang = ConTroHienTai * CAP;
            int ChiSoCuoiTrang = std::min(TongSoLuong, ChiSoDauTrang + CAP);
            for (int i = ChiSoDauTrang; i < ChiSoCuoiTrang; ++i) {
                tui::DiChuyenConTro(X0, Y2++);
                std::cout << CacDongKetQua[i];
            }
            };
        PaintPage(Trang);
        while (true) {
            tui::KeyEvent SuKienPhimMoi = tui::DocPhim();
            if (SuKienPhimMoi.Phim == tui::PhimEsc) {
                return;
            }
            if (SuKienPhimMoi.Phim == tui::PhimLen) {
                if (Trang > 0) {
                    --Trang;
                    PaintPage(Trang);
                }
            }
            if (SuKienPhimMoi.Phim == tui::PhimXuong) {
                if (Trang + 1 < TongSoTrang) {
                    ++Trang;
                    PaintPage(Trang);
                }
            }
        }
    }
    //================ Mượn/trả sách ==============//
    //================ Mượn sách ==============//
    inline void FormMuonSachTUI(DanhSachDauSach& DanhSachDauSach, DocGiaNode*& Root) {
        const int ChieuRong = 118, ChieuCao = 24, X0 = 4, Y0 = 3;
        const int FooterY = 1 + ChieuCao - 2;
        tui::XoaManHinh();
        tui::VeKhung(2, 1, ChieuRong, ChieuCao, "MUON / TRA  >  MUON SACH");
        int Y = Y0 + 1;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Nhap MA THE                : ";
        int MaX = X0 + 28, MaY = Y;
        Y += 2;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Nhap ISBN                  : ";
        int ISBNX = X0 + 28, ISBNY = Y;
        Y += 2;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Nhap ngay muon (dd/mm/yyyy): ";
        int NgayX = X0 + 28, NgayY = Y;
        Y += 2;
        tui::InHuongDanCuoiTrang(4, FooterY, "[Enter] Xac nhan  -  [Esc] Quay lai");
        LamSachBoDemNhap();
        // 1. Nhập Mã Thẻ
        int MaTheCanXuLy = -1;
        while (true) {
            std::string ChuoiNhap;
            int KetQuaNhap = DocDongChoPhepThoatKhiRong(MaX, MaY, 12, ChuoiNhap);
            if (KetQuaNhap == -1) {
                return;
            }
            MaTheCanXuLy = -1;
            if (!ChuoiNhap.empty()) {
                try {
                    MaTheCanXuLy = std::stoi(ChuoiNhap);
                }
                catch (...) {
                    MaTheCanXuLy = -1;
                }
            }
            if (MaTheCanXuLy > 0) {
                break;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            std::cout << "Ma the khong hop le. Moi nhap lai.";
            tui::DatLaiMau();
            tui::DiChuyenConTro(MaX, MaY);
            std::cout << std::string(20, ' ');
        }
        DocGiaNode* PNode = TimDocGiaTheoMaThe(Root, MaTheCanXuLy);
        if (PNode == NULL) {
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            std::cout << "Khong tim thay doc gia.";
            tui::DatLaiMau();
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        auto PAD = [](const std::string& ChuoiNhap, int W) -> std::string {
            return (int)ChuoiNhap.size() >= W ? ChuoiNhap.substr(0, W)
                : ChuoiNhap + std::string(W - (int)ChuoiNhap.size(), ' ');
            };
        auto LPAD = [](const std::string& ChuoiNhap, int W) -> std::string {
            return (int)ChuoiNhap.size() >= W ? ChuoiNhap.substr(0, W)
                : std::string(W - (int)ChuoiNhap.size(), ' ') + ChuoiNhap;
            };
        auto DASH = [](int SoPhanTu) -> std::string {
            return std::string(SoPhanTu, '-');
            };
        auto FmtDate = [](const NgayThangNam& NgayCanXuLy) -> std::string {
            char GiaTriThuHai[16];
            std::snprintf(
                GiaTriThuHai, sizeof(GiaTriThuHai), "%02d/%02d/%04d", NgayCanXuLy.Ngay, NgayCanXuLy.Thang, NgayCanXuLy.Nam);
            return std::string(GiaTriThuHai);
            };
        // 2. Hiển thị danh sách đang mượn
        const int DoRongCotSTT = 4, DoRongCotMS = 13, DoRongCotISBN = 11, DoRongCotTEN = 60, DoRongCotNGAY = 10;
        int TableY = Y0 + 7;
        tui::DiChuyenConTro(X0, TableY++);
        std::cout << PAD("STT", DoRongCotSTT) << " | " << PAD("MaSach", DoRongCotMS) << " | " << PAD("ISBN", DoRongCotISBN)
            << " | " << PAD("Ten sach", DoRongCotTEN) << " | " << PAD("Ngay muon", DoRongCotNGAY);
        tui::DiChuyenConTro(X0, TableY++);
        std::cout << DASH(DoRongCotSTT) << "-+-" << DASH(DoRongCotMS) << "-+-" << DASH(DoRongCotISBN) << "-+-"
            << DASH(DoRongCotTEN) << "-+-" << DASH(DoRongCotNGAY);
        int Stt = 0;
        for (MuonTraNode* PhieuMuonTraHienTai = PNode->ThongTin.MuonTraHead; PhieuMuonTraHienTai;
            PhieuMuonTraHienTai = PhieuMuonTraHienTai->Next) {
            if (PhieuMuonTraHienTai->TrangThai != 0) {
                continue;
            }
            std::string ISBNThuNhat = LayISBNTuMaSach(PhieuMuonTraHienTai->MaSach);
            std::string Ten0;
            DauSach* Ds0 = TimDauSachTheoISBN(DanhSachDauSach, ISBNThuNhat);
            if (Ds0) {
                Ten0 = Ds0->TenSach;
            }
            if (TableY >= FooterY - 3) {
                break;
            }
            tui::DiChuyenConTro(X0, TableY++);
            std::cout << LPAD(std::to_string(++Stt), DoRongCotSTT) << " | " << PAD(PhieuMuonTraHienTai->MaSach, DoRongCotMS)
                << " | " << PAD(ISBNThuNhat, DoRongCotISBN) << " | " << PAD(Ten0, DoRongCotTEN) << " | "
                << PAD(FmtDate(PhieuMuonTraHienTai->NgayMuon), DoRongCotNGAY);
        }
        if (Stt == 0) {
            tui::DiChuyenConTro(X0, TableY++);
            std::cout << "(Khong co sach dang muon.)";
        }        
        // 3. Nhập ISBN
        std::string ISBNCanXuLy;
        if (DocDongChoPhepThoatKhiRong(ISBNX, ISBNY, 20, ISBNCanXuLy) == -1) {
            return;
        }
        DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy);
        if (DuLieuSach == NULL) {
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            std::cout << "Khong tim thay dau sach voi ISBN da nhap.";
            tui::DatLaiMau();
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        // 4. Nhập ngày mượn
        NgayThangNam NgayMuon{};
        while (true) {
            std::string ChuoiNhap;
            int KetQuaNhap = DocDongChoPhepThoatKhiRong(NgayX, NgayY, 16, ChuoiNhap);
            if (KetQuaNhap == -1) {
                return;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << std::string(90, ' ');
            NgayMuon = PhanTichNgayDDMMYYYY(ChuoiNhap);
            if (!KiemTraNgayHopLe(NgayMuon)) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Ngay khong hop le (dd/mm/yyyy). Moi nhap lai!";
                tui::DatLaiMau();
                tui::DiChuyenConTro(NgayX, NgayY);
                std::cout << std::string(20, ' ');
                continue;
            }
            if (NgayMuon.Nam < 1500 || SoSanhNgay(NgayMuon,LayNgayHienTai()) > 0) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: Ngay muon khong duoc lon hon ngay hien tai.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(NgayX, NgayY);
                std::cout << std::string(20, ' ');
                continue;
            }
            break;
        }
        // 5. Xác nhận mượn
        const int ConfirmY = FooterY - 3;
        tui::DiChuyenConTro(X0, ConfirmY);
        std::cout << "Xac nhan muon sach: ";
        int Ok = ChonRadioTaiViTri(X0 + 22, ConfirmY, { "Co", "Khong" }, 0);
        if (Ok != 0) {
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << "Da huy muon sach.";
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        // 6. Gọi trực tiếp hàm nghiệp vụ mượn sách
        std::string MaSachDaMuon;
        std::string ThongBaoLoi;
        if (!MuonSach(
            PNode->ThongTin,
            *DuLieuSach,
            NgayMuon,
            &MaSachDaMuon,
            &ThongBaoLoi
        )){
            tui::DiChuyenConTro(X0,FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            std::cout << ThongBaoLoi;
            tui::DatLaiMau();
            tui::NhanPhimBatKyDeQuayLai(4,FooterY - 1);
            return;
        }
        tui::DiChuyenConTro(X0,FooterY - 2);
        tui::DatMau(tui::MauThanhCong);
        std::cout << "Da MUON thanh cong: " << DuLieuSach->ISBN << " | " << DuLieuSach->TenSach << " | MaSach: " << MaSachDaMuon;
        tui::DatLaiMau();
        tui::NhanPhimBatKyDeQuayLai(4,FooterY - 1);
    }
    //================ Trả sách ==============//
    inline void FormTraSachTUI(DanhSachDauSach& DanhSachDauSach, DocGiaNode*& Root) {
        const int ChieuRong = 118, ChieuCao = 24, X0 = 4, Y0 = 3;
        const int FooterY = 1 + ChieuCao - 2;
        const int YMax = 1 + ChieuCao - 3;
        const int DoRongCotSTT = 4, DoRongCotMS = 13, DoRongCotISBN = 11, DoRongCotTEN = 42, DoRongCotNGAY = 10;
        auto PAD = [](const std::string& ChuoiNhap, int ChieuRong) -> std::string {
            if ((int)ChuoiNhap.size() >= ChieuRong) {
                return ChuoiNhap.substr(0, ChieuRong);
            }
            return ChuoiNhap + std::string(ChieuRong - (int)ChuoiNhap.size(), ' ');
            };
        auto DASH = [](int SoPhanTu) -> std::string {
            return std::string(SoPhanTu, '-');
            };
        auto FmtDate = [](const NgayThangNam& NgayCanXuLy) -> std::string {
            char BoDem[16];
            std::snprintf(BoDem, sizeof(BoDem), "%02d/%02d/%04d", NgayCanXuLy.Ngay, NgayCanXuLy.Thang, NgayCanXuLy.Nam);
            return std::string(BoDem);
            };
        tui::XoaManHinh();
        tui::VeKhung(2, 1, ChieuRong, ChieuCao, "MUON / TRA  >  TRA SACH");
        int Y = Y0 + 1;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Nhap MA THE: ";
        int MaX = X0 + 14, MaY = Y;
        tui::InHuongDanCuoiTrang(4, FooterY, "[Enter] Xac nhan  -  [Esc] Quay lai");
        LamSachBoDemNhap();
        // 1. Nhập và Tìm độc giả
        std::string SMa;
        if (DocDongChoPhepThoatKhiRong(MaX, MaY, 12, SMa) == -1) {
            return;
        }
        int MaTheCanXuLy = -1;
        try {
            MaTheCanXuLy = std::stoi(SMa);
        }
        catch (...) {
            MaTheCanXuLy = -1;
        }
        DocGiaNode* PNode = TimDocGiaTheoMaThe(Root, MaTheCanXuLy);
        if (MaTheCanXuLy <= 0 || PNode == NULL) {
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            std::cout << "Ma the khong hop le hoac khong ton tai.";
            tui::DatLaiMau();
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        MuonTraNode* DanhSachDong[100];
        int SoDong = 0;
        for (MuonTraNode* ConTroHienTai = PNode->ThongTin.MuonTraHead; ConTroHienTai; ConTroHienTai = ConTroHienTai->Next) {
            if (ConTroHienTai->TrangThai == 0) {
                if (SoDong < 100) {
                    DanhSachDong[SoDong++] = ConTroHienTai;
                }
            }
        }
        Y += 2;
        tui::DiChuyenConTro(X0, Y++);
        std::cout << "Danh sach DANG MUON:";
        if (SoDong == 0) {
            tui::DiChuyenConTro(X0, Y++);
            std::cout << "(Doc gia khong co sach dang muon.)";
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        std::string Header = PAD("STT", DoRongCotSTT) + " | " + PAD("MaSach", DoRongCotMS) + " | " +
            PAD("ISBN", DoRongCotISBN) + " | " + PAD("Ten sach", DoRongCotTEN) + " | " +
            PAD("Ngay muon", DoRongCotNGAY);
        std::string Sep = DASH(DoRongCotSTT) + "-+-" + DASH(DoRongCotMS) + "-+-" + DASH(DoRongCotISBN) + "-+-" +
            DASH(DoRongCotTEN) + "-+-" + DASH(DoRongCotNGAY);
        tui::DiChuyenConTro(X0, Y++);
        std::cout << Header;
        tui::DiChuyenConTro(X0, Y++);
        std::cout << Sep;
        //  2.Duyệt mảng tĩnh
        for (int i = 0; i < SoDong; ++i) {
            MuonTraNode* PhieuMuonTraHienTai = DanhSachDong[i];
            std::string ISBNCanXuLy = LayISBNTuMaSach(PhieuMuonTraHienTai->MaSach);
            DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy);
            std::string TenNhap = DuLieuSach ? DuLieuSach->TenSach : "";
            if ((int)TenNhap.size() > DoRongCotTEN) {
                TenNhap = TenNhap.substr(0, DoRongCotTEN);
            }
            std::string DongDuLieu = PAD(std::to_string(i + 1), DoRongCotSTT) + " | " +
                PAD(PhieuMuonTraHienTai->MaSach, DoRongCotMS) + " | " +
                PAD(ISBNCanXuLy, DoRongCotISBN) + " | " + PAD(TenNhap, DoRongCotTEN) + " | " +
                PAD(FmtDate(PhieuMuonTraHienTai->NgayMuon), DoRongCotNGAY);
            tui::DiChuyenConTro(X0, Y++);
            std::cout << DongDuLieu;
            if (Y > YMax) {
                tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
                tui::XoaManHinh();
                tui::VeKhung(2, 1, ChieuRong, ChieuCao, "MUON / TRA  >  TRA SACH");
                Y = 5;
                tui::DiChuyenConTro(X0, Y++);
                std::cout << Header;
                tui::DiChuyenConTro(X0, Y++);
                std::cout << Sep;
            }
        }
        // 3. Chọn sách trả
        int PromptY = std::min(FooterY - 6, Y + 1);
        tui::DiChuyenConTro(X0, PromptY);
        std::cout << "Nhap STT de tra (hoac bo trong de nhap ma sach de tra): ";
        int ChooseX = X0 + 55, ChooseY = PromptY;
        LamSachBoDemNhap();
        std::string SChon;
        if (DocDongChoPhepThoatKhiRong(ChooseX, ChooseY, 8, SChon) == -1) {
            return;
        }
        MuonTraNode* DoiTuongCanXuLy = NULL;
        if (!SChon.empty()) {
            int ChiSo = -1;
            try {
                ChiSo = std::stoi(SChon);
            }
            catch (...) {
                ChiSo = -1;
            }
            if (1 <= ChiSo && ChiSo <= SoDong) {
                DoiTuongCanXuLy = DanhSachDong[ChiSo - 1];
            }
        }
        if (DoiTuongCanXuLy == NULL) {
            tui::DiChuyenConTro(X0, ChooseY + 1);
            std::cout << "Nhap MaSach can tra: ";
            int MsX = X0 + 23, MsY = ChooseY + 1;
            std::string MaSachTam;
            if (DocDongChoPhepThoatKhiRong(MsX, MsY, 20, MaSachTam) == -1) {
                return;
            }
            for (int i = 0; i < SoDong; i++) {
                if (DanhSachDong[i]->MaSach == MaSachTam) {
                    DoiTuongCanXuLy = DanhSachDong[i];
                    break;
                }
            }
        }
        if (DoiTuongCanXuLy == NULL) {
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            std::cout << "Lua chon khong hop le.";
            tui::DatLaiMau();
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        // 4. Nhập ngày trả và kiểm tra logic
        NgayThangNam NgayTra{};
        tui::DiChuyenConTro(X0, FooterY - 2);
        std::cout << std::string(90, ' ');
        while (true) {
            tui::DiChuyenConTro(X0, ChooseY + 3);
            std::cout << "Nhap ngay tra (dd/mm/yyyy): ";
            int NtX = X0 + 30, NtY = ChooseY + 3;
            std::string ChuoiNhap;
            int KetQuaNhap = DocDongChoPhepThoatKhiRong(NtX, NtY, 16, ChuoiNhap);
            if (KetQuaNhap == -1) {
                return;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << std::string(90, ' ');
            NgayTra = PhanTichNgayDDMMYYYY(ChuoiNhap);
            if (!KiemTraNgayHopLe(NgayTra)) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Dinh dang ngay khong dung. Moi nhap lai!";
                tui::DatLaiMau();
                tui::DiChuyenConTro(NtX, NtY);
                std::cout << std::string(16, ' ');
                continue;
            }
            if (NgayTra.Nam < 1500 || SoSanhNgay(NgayTra,LayNgayHienTai()) > 0) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: Ngay tra khong duoc lon hon ngay hien tai.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(NtX, NtY);
                std::cout << std::string(16, ' ');
                continue;
            }
            if (TinhSoNgayChenhLech(NgayTra, DoiTuongCanXuLy->NgayMuon) < 0) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: Ngay tra khong duoc nho hon Ngay muon (" << DoiTuongCanXuLy->NgayMuon.Ngay << "/"
                    << DoiTuongCanXuLy->NgayMuon.Thang << "/" << DoiTuongCanXuLy->NgayMuon.Nam << ")";
                tui::DatLaiMau();
                tui::DiChuyenConTro(NtX, NtY);
                std::cout << std::string(16, ' ');
                continue;
            }
            break;
        }
        // 5. Gọi trực tiếp hàm nghiệp vụ trả sách
        int TongSoNgay = 0;
        int SoNgayTre = 0;
        std::string ThongBaoLoi;
        if (!TraSach(
            PNode->ThongTin,
            DanhSachDauSach,
            DoiTuongCanXuLy,
            NgayTra,
            &TongSoNgay,
            &SoNgayTre,
            &ThongBaoLoi
        ))
        {
            tui::DiChuyenConTro(X0,FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            std::cout << ThongBaoLoi;
            tui::DatLaiMau();
            tui::NhanPhimBatKyDeQuayLai(4,FooterY - 1);
            return;
        }
        tui::DiChuyenConTro(X0,FooterY - 2);
        std::cout
            << "Da TRA sach: "
            << DoiTuongCanXuLy->MaSach
            << " | So ngay muon: "
            << TongSoNgay
            << (
                SoNgayTre > 0
                ? " | Tre han: " +
                std::to_string(SoNgayTre) +
                " ngay"
                : ""
                )
            << ".";
        tui::NhanPhimBatKyDeQuayLai(4,FooterY - 1);
    }
    // =========== Liệt Kê Sách Đang Mượn Của Độc Giả ================//
    inline void FormInSachDangMuonTUI(const DanhSachDauSach& DanhSachDauSach, DocGiaNode* Root) {
        const int ChieuRong = 118, ChieuCao = 24, X0 = 4, Y0 = 3;
        const int FooterY = 1 + ChieuCao - 2, YMax = 1 + ChieuCao - 3;
        const int DoRongCotSTT = 4, DoRongCotMS = 13, DoRongCotISBN = 11, DoRongCotTEN = 42, DoRongCotNGAY = 10,
            DoRongCotSONG = 7;
        auto PAD = [](const std::string& ChuoiNhap, int ChieuRong) -> std::string {
            if ((int)ChuoiNhap.size() >= ChieuRong) {
                return ChuoiNhap.substr(0, ChieuRong);
            }
            return ChuoiNhap + std::string(ChieuRong - (int)ChuoiNhap.size(), ' ');
            };
        auto DASH = [](int SoPhanTu) -> std::string {
            return std::string(SoPhanTu, '-');
            };
        auto FmtDate = [](const NgayThangNam& NgayCanXuLy) -> std::string {
            char BoDem[16];
            std::snprintf(BoDem, sizeof(BoDem), "%02d/%02d/%04d", NgayCanXuLy.Ngay, NgayCanXuLy.Thang, NgayCanXuLy.Nam);
            return std::string(BoDem);
            };
        tui::XoaManHinh();
        tui::VeKhung(2, 1, ChieuRong, ChieuCao, "MUON / TRA  >  DANH SACH DANG MUON CUA DOC GIA");
        int Y = Y0 + 1;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Nhap MA THE: ";
        int TheX = X0 + 14, TheY = Y;
        Y += 2;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Nhap ngay hien tai (dd/mm/yyyy): ";
        int DateX = X0 + 34, DateY = Y;
        Y += 2;
        tui::InHuongDanCuoiTrang(4, FooterY, "[Enter] Xac nhan  -  [Esc] Quay lai");
        LamSachBoDemNhap();
        // 1. Nhập Mã Thẻ
        std::string SThe;
        if (DocDongChoPhepThoatKhiRong(TheX, TheY, 12, SThe) == -1) {
            return;
        }
        int MaTheCanXuLy = -1;
        try {
            MaTheCanXuLy = std::stoi(SThe);
        }
        catch (...) {
            MaTheCanXuLy = -1;
        }
        DocGiaNode* PNode = TimDocGiaTheoMaThe(Root, MaTheCanXuLy);
        if (MaTheCanXuLy <= 0 || PNode == nullptr) {
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            std::cout << "Ma the khong hop le hoac khong ton tai.";
            tui::DatLaiMau();
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        // 2. Nhập Ngày Hiện Tại
        NgayThangNam NgayHienTai{};
        int NamHienTai = LayNgayHienTai().Nam;
        while (true) {
            std::string ChuoiNhap;
            int KetQuaNhap = DocDongChoPhepThoatKhiRong(DateX, DateY, 16, ChuoiNhap);
            if (KetQuaNhap == -1) {
                return;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << std::string(110, ' ');
            NgayHienTai = PhanTichNgayDDMMYYYY(ChuoiNhap);
            if (!KiemTraNgayHopLe(NgayHienTai)) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Ngay khong hop le. Moi nhap lai!";
                tui::DatLaiMau();
                tui::DiChuyenConTro(DateX, DateY);
                std::cout << std::string(20, ' ');
                continue;
            }
            if (NgayHienTai.Nam < 1500 || NgayHienTai.Nam > NamHienTai) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: Nam khong hop le";
                tui::DatLaiMau();
                tui::DiChuyenConTro(DateX, DateY);
                std::cout << std::string(20, ' ');
                continue;
            }
            break;
        }
        // 3. Thu thập dữ liệu
        struct Row {
            std::string MaSach, ISBN, Ten;
            NgayThangNam NgayMuon;
            int TongSoNgay;
        };
        Row DanhSachDong[100];
        int SoDong = 0;
        for (MuonTraNode* ConTroHienTai = PNode->ThongTin.MuonTraHead; ConTroHienTai; ConTroHienTai = ConTroHienTai->Next) {
            if (ConTroHienTai->TrangThai == 0) {
                int Diff = TinhSoNgayChenhLech(NgayHienTai, ConTroHienTai->NgayMuon);
                if (Diff >= 0 && SoDong < 100) {
                    Row& KetQuaNhap = DanhSachDong[SoDong++]; // Lấy tham chiếu đến phần tử mảng để gán
                    KetQuaNhap.MaSach = ConTroHienTai->MaSach;
                    KetQuaNhap.ISBN = LayISBNTuMaSach(ConTroHienTai->MaSach);
                    DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, KetQuaNhap.ISBN);
                    if (DuLieuSach) {
                        KetQuaNhap.Ten = DuLieuSach->TenSach;
                    }
                    else {
                        KetQuaNhap.Ten = "";
                    }
                    KetQuaNhap.NgayMuon = ConTroHienTai->NgayMuon;
                    KetQuaNhap.TongSoNgay = Diff;
                }
            }
        }
        // 4. Hiển thị thông tin độc giả
        tui::DiChuyenConTro(X0, Y++);
        {
            std::string Name = PNode->ThongTin.Ho + std::string(" ") + PNode->ThongTin.Ten;
            std::cout << "Doc gia: " << PNode->ThongTin.MaThe << " | " << Name;
        }
        Y++;
        if (SoDong == 0) {
            tui::DiChuyenConTro(X0, Y++);
            std::cout << "(Khong co sach dang muon nao truoc hoac trong ngay " << FmtDate(NgayHienTai) << ".)";
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        // 5. Hiển thị bảng
        std::string Header = PAD("STT", DoRongCotSTT) + " | " + PAD("MaSach", DoRongCotMS) + " | " +
            PAD("ISBN", DoRongCotISBN) + " | " + PAD("Ten sach", DoRongCotTEN) + " | " +
            PAD("Ngay muon", DoRongCotNGAY) + " | " + PAD("So ngay", DoRongCotSONG);
        std::string Sep = DASH(DoRongCotSTT) + "-+-" + DASH(DoRongCotMS) + "-+-" + DASH(DoRongCotISBN) + "-+-" +
            DASH(DoRongCotTEN) + "-+-" + DASH(DoRongCotNGAY) + "-+-" + DASH(DoRongCotSONG);
        tui::DiChuyenConTro(X0, Y++);
        std::cout << Header;
        tui::DiChuyenConTro(X0, Y++);
        std::cout << Sep;
        for (int i = 0; i < SoDong; ++i) {
            Row& KetQuaNhap = DanhSachDong[i];
            std::string TenCut =
                (int)KetQuaNhap.Ten.size() > DoRongCotTEN ? KetQuaNhap.Ten.substr(0, DoRongCotTEN) : KetQuaNhap.Ten;
            std::string DongDuLieu = PAD(std::to_string(i + 1), DoRongCotSTT) + " | " +
                PAD(KetQuaNhap.MaSach, DoRongCotMS) + " | " + PAD(KetQuaNhap.ISBN, DoRongCotISBN) +
                " | " + PAD(TenCut, DoRongCotTEN) + " | " +
                PAD(FmtDate(KetQuaNhap.NgayMuon), DoRongCotNGAY) + " | " +
                PAD(std::to_string(KetQuaNhap.TongSoNgay), DoRongCotSONG);
            tui::DiChuyenConTro(X0, Y++);
            std::cout << DongDuLieu;
            if (Y > YMax) {
                tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
                tui::XoaManHinh();
                tui::VeKhung(2, 1, ChieuRong, ChieuCao, "MUON / TRA  >  DANH SACH DANG MUON CUA DOC GIA");
                Y = 5;
                tui::DiChuyenConTro(X0, Y++);
                std::cout << Header;
                tui::DiChuyenConTro(X0, Y++);
                std::cout << Sep;
            }
        }
        tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
    }
    //================ Thống kê =================//
    /// ========= Thống Kê Top 10 ===============//
    inline void ThongKeTop10LuotMuonTUI(const DanhSachDauSach& DanhSachDauSach) {
        const int ChieuRong = 118, ChieuCao = 24;
        const int FooterY = 1 + ChieuCao - 2;
        const int DoRongCotSTT = 4, DoRongCotISBN = 12, DoRongCotTEN = 47, DoRongCotCNT = 14;
        auto PAD = [](const std::string& ChuoiNhap, int W) -> std::string {
            if ((int)ChuoiNhap.size() >= W) {
                return ChuoiNhap.substr(0, W);
            }
            return ChuoiNhap + std::string(W - (int)ChuoiNhap.size(), ' ');
            };
        auto LPAD = [](const std::string& ChuoiNhap, int W) -> std::string {
            if ((int)ChuoiNhap.size() >= W) {
                return ChuoiNhap.substr(0, W);
            }
            return std::string(W - (int)ChuoiNhap.size(), ' ') + ChuoiNhap;
            };
        auto DASH = [](int SoPhanTu) -> std::string {
            return std::string(SoPhanTu, '-');
            };
        DauSach* Top10[10];
        int SoLuongTimThay = 0;
        ThongKeTop10TheoLuotMuon(DanhSachDauSach, Top10, SoLuongTimThay);
        tui::XoaManHinh();
        tui::VeKhung(2, 1, ChieuRong, ChieuCao, "THONG KE > TOP 10 SACH MUON NHIEU NHAT");
        int Y = 5;
        tui::DiChuyenConTro(4, Y++);
        std::cout << PAD("STT", DoRongCotSTT) << " | " << PAD("ISBN", DoRongCotISBN) << " | "
            << PAD("Ten sach", DoRongCotTEN) << " | " << PAD("So luot muon", DoRongCotCNT);
        tui::DiChuyenConTro(4, Y++);
        std::cout << DASH(DoRongCotSTT) << "-+-" << DASH(DoRongCotISBN) << "-+-" << DASH(DoRongCotTEN) << "-+-"
            << DASH(DoRongCotCNT);
        if (SoLuongTimThay == 0) {
            tui::DiChuyenConTro(4, Y++);
            std::cout << "(Chua co sach nao duoc muon.)";
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        for (int i = 0; i < SoLuongTimThay; ++i) {
            DauSach* DuLieuSach = Top10[i];
            std::string TenCut = DuLieuSach->TenSach;
            if ((int)TenCut.size() > DoRongCotTEN) {
                TenCut = TenCut.substr(0, DoRongCotTEN);
            }
            tui::DiChuyenConTro(4, Y++);
            std::cout << PAD(std::to_string(i + 1) + ".", DoRongCotSTT) << " | " << PAD(DuLieuSach->ISBN, DoRongCotISBN)
                << " | " << PAD(TenCut, DoRongCotTEN) << " | "
                << LPAD(std::to_string(DuLieuSach->SoLuotMuon), DoRongCotCNT);
        }
        tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
    }
    // ================= Thống Kê Quá Hạn ===================//
    inline void ThongKeDanhSachQuaHanTUI(const DanhSachDauSach& DanhSachDauSach, DocGiaNode* Root) {
        const int ChieuRong = 118, ChieuCao = 24, X0 = 4, Y0 = 3;
        const int FooterY = 1 + ChieuCao - 2;
        const int DoRongCotSTT = 4, DoRongCotMATHE = 9, DoRongCotMS = 13, DoRongCotTEN = 47, DoRongCotNGAY = 10,
            DoRongCotTRE = 5;
        auto PAD = [](const std::string& ChuoiNhap, int W) -> std::string {
            return (int)ChuoiNhap.size() >= W ? ChuoiNhap.substr(0, W)
                : ChuoiNhap + std::string(W - (int)ChuoiNhap.size(), ' ');
            };
        auto LPAD = [](const std::string& ChuoiNhap, int W) -> std::string {
            return (int)ChuoiNhap.size() >= W ? ChuoiNhap.substr(0, W)
                : std::string(W - (int)ChuoiNhap.size(), ' ') + ChuoiNhap;
            };
        auto DASH = [](int SoPhanTu) -> std::string {
            return std::string(SoPhanTu, '-');
            };
        auto FmtDate = [](const NgayThangNam& NgayCanXuLy) -> std::string {
            char GiaTriThuHai[16];
            std::snprintf(
                GiaTriThuHai, sizeof(GiaTriThuHai), "%02d/%02d/%04d", NgayCanXuLy.Ngay, NgayCanXuLy.Thang, NgayCanXuLy.Nam);
            return std::string(GiaTriThuHai);
            };
        tui::XoaManHinh();
        tui::VeKhung(2, 1, ChieuRong, ChieuCao, "THONG KE > DANH SACH MUON QUA HAN");
        int Y = Y0 + 1;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Nhap ngay hien tai (dd/mm/yyyy): ";
        int DateX = X0 + 34, DateY = Y;
        Y += 2;
        tui::InHuongDanCuoiTrang(4, FooterY, "[Up/Down] Trang truoc/sau  -  [Esc] Quay lai");
        LamSachBoDemNhap();
        NgayThangNam NgayHienTai{};
        int NamHienTai = LayNgayHienTai().Nam;
        while (true) {
            std::string ChuoiNhap;
            int KetQuaNhap = DocDongChoPhepThoatKhiRong(DateX, DateY, 16, ChuoiNhap);
            if (KetQuaNhap == -1) {
                return;
            }
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << std::string(90, ' ');
            NgayHienTai = PhanTichNgayDDMMYYYY(ChuoiNhap);
            if (!KiemTraNgayHopLe(NgayHienTai)) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Ngay khong hop le. Moi nhap lai!";
                tui::DatLaiMau();
                tui::DiChuyenConTro(DateX, DateY);
                std::cout << std::string(20, ' ');
                continue;
            }
            if (NgayHienTai.Nam < 1500 || NgayHienTai.Nam > NamHienTai) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: Nam khong hop le";
                tui::DatLaiMau();
                tui::DiChuyenConTro(DateX, DateY);
                std::cout << std::string(20, ' ');
                continue;
            }
            break;
        }
        const int MAXROWS = 500;
        ThongKeQuaHan DanhSachDong[MAXROWS];
        int TongSoLuong = 0;
        LapDanhSachQuaHan(Root, DanhSachDauSach, NgayHienTai, DanhSachDong, TongSoLuong, MAXROWS);
        const int PAGE = 14;
        int TongSoTrang = (TongSoLuong == 0 ? 1 : (TongSoLuong + PAGE - 1) / PAGE);
        int Trang = 0;
        const int HeaderY = Y0 + 3;
        const int TableY = HeaderY + 2;
        const int DataH = PAGE;
        auto PaintHeader = [&](int TrangHienTai) {
            tui::DiChuyenConTro(X0, Y0 + 1);
            std::cout << "Nhap ngay hien tai (dd/mm/yyyy): " << FmtDate(NgayHienTai) << "   (Trang " << (TrangHienTai + 1)
                << "/" << TongSoTrang << ")" << std::string(10, ' ');
            tui::DiChuyenConTro(X0, HeaderY);
            std::cout << PAD("STT", DoRongCotSTT) << " | " << PAD("MaThe", DoRongCotMATHE) << " | "
                << PAD("MaSach", DoRongCotMS) << " | " << PAD("Ten sach", DoRongCotTEN) << " | "
                << PAD("Ngay muon", DoRongCotNGAY) << " | " << PAD("Tre", DoRongCotTRE);
            tui::DiChuyenConTro(X0, HeaderY + 1);
            std::cout << DASH(DoRongCotSTT) << "-+-" << DASH(DoRongCotMATHE) << "-+-" << DASH(DoRongCotMS) << "-+-"
                << DASH(DoRongCotTEN) << "-+-" << DASH(DoRongCotNGAY) << "-+-" << DASH(DoRongCotTRE);
            };
        auto PaintPage = [&](int TrangHienTai) {
            PaintHeader(TrangHienTai);
            tui::XoaVung(X0, TableY, ChieuRong - 8, DataH);
            if (TongSoLuong == 0) {
                tui::DiChuyenConTro(X0, TableY);
                std::cout << "(Khong co phieu qua han.)";
                return;
            }
            int ChiSoDauTrang = TrangHienTai * PAGE;
            int ChiSoCuoiTrang = std::min(TongSoLuong, ChiSoDauTrang + PAGE);
            int YOut = TableY;
            for (int i = ChiSoDauTrang; i < ChiSoCuoiTrang; ++i) {
                const ThongKeQuaHan& KetQuaNhap = DanhSachDong[i];
                std::string TenSachThongKe = KetQuaNhap.TenSach;
                std::string TenCut =
                    (int)TenSachThongKe.size() > DoRongCotTEN ? TenSachThongKe.substr(0, DoRongCotTEN) : TenSachThongKe;
                tui::DiChuyenConTro(X0, YOut++);
                std::cout << PAD(std::to_string(i + 1), DoRongCotSTT) << " | "
                    << PAD(std::to_string(KetQuaNhap.MaThe), DoRongCotMATHE) << " | "
                    << PAD(KetQuaNhap.MaSach, DoRongCotMS) << " | " << PAD(TenCut, DoRongCotTEN) << " | "
                    << PAD(FmtDate(KetQuaNhap.NgayMuon), DoRongCotNGAY) << " | "
                    << LPAD(std::to_string(KetQuaNhap.SoNgayTre), DoRongCotTRE);
            }
            };
        PaintPage(Trang);
        while (true) {
            tui::KeyEvent SuKienPhimMoi = tui::DocPhim();
            if (SuKienPhimMoi.Phim == tui::PhimEsc) {
                return;
            }
            if (SuKienPhimMoi.Phim == tui::PhimLen) {
                if (Trang > 0) {
                    --Trang;
                    PaintPage(Trang);
                }
            }
            if (SuKienPhimMoi.Phim == tui::PhimXuong) {
                if (Trang + 1 < TongSoTrang) {
                    ++Trang;
                    PaintPage(Trang);
                }
            }
        }
    }
    //==================== Menus ====================//
    //========= Submenu Quản lý độc giả ===========//
    inline void MenuConDocGia(DanhSachDauSach& DanhSachDauSach, DocGiaNode*& Root) {
        (void)DanhSachDauSach;
        const std::string TieuDe = "QUAN LY DOC GIA";
        while (true) {
            int Ch = MenuMuiTen(8,
                0,
                TieuDe,
                { "1. Them doc gia",
                 "2. Xoa doc gia theo ma the",
                 "3. Cap nhat doc gia theo ma the",
                 "4. Danh sach doc gia theo ten",
                 "5. Danh sach doc gia theo ma the",
                 "0. Quay lai" });
            if (Ch < 0) {
                return;
            }
            if (Ch == 5) {
                return;
            }
            switch (Ch) {
            case 0:
                FormThemDocGiaTUI(Root);
                break;
            case 1:
                FormXoaDocGiaTUI(Root);
                break;
            case 2:
                FormSuaDocGiaTUI(Root);
                break;
            case 3:
                InDocGiaTheoTenHoTUI(Root);
                break;
            case 4:
                InDocGiaTheoMaTheTUI(Root);
                break;
            default:
                break;
            }
        }
    }
    //========= Submenu Quản lý đầu sách ===========//
    inline void MenuConDauSach(DanhSachDauSach& DanhSachDauSach, DocGiaNode*& Root) {
        const std::string TieuDe = "QUAN LY DAU SACH";
        while (true) {
            int Ch = MenuMuiTen(8,
                0,
                TieuDe,
                { "1. Them dau sach",
                 "2. Xoa dau sach",
                 "3. Cap nhat thong tin dau sach",
                 "4. Danh sach dau sach theo the loai",
                 "5. Tim dau sach theo ten",
                 "0. Quay lai" });
            if (Ch < 0) {
                return;
            }
            if (Ch == 5) {
                return;
            }
            switch (Ch) {
            case 0:
                FormThemDauSachTUI(DanhSachDauSach);
                break;
            case 1:
                FormXoaDauSachTUI(DanhSachDauSach, Root);
                break;
            case 2:
                FormCapNhatDauSachTUI(DanhSachDauSach);
                break;
            case 3:
                InDauSachTheoTheLoaiTUI(DanhSachDauSach);
                break;
            case 4:
                TimDauSachTheoTenTUI(DanhSachDauSach);
                break;
            default:
                break;
            }
        }
    }
    //========= Submenu Mượn / Trả sách ===========//
    inline void MenuConMuonTra(DanhSachDauSach& DanhSachDauSach, DocGiaNode*& Root) {
        const std::string TieuDe = "MUON / TRA";
        while (true) {
            int Ch = MenuMuiTen(
                8, 0, TieuDe, { "1. Muon sach", "2. Tra sach", "3. Danh sach sach dang muon cua doc gia", "0. Quay lai" });
            if (Ch < 0) {
                return;
            }
            if (Ch == 3) {
                return;
            }
            switch (Ch) {
            case 0:
                FormMuonSachTUI(DanhSachDauSach, Root);
                break;
            case 1:
                FormTraSachTUI(DanhSachDauSach, Root);
                break;
            case 2:
                FormInSachDangMuonTUI(DanhSachDauSach, Root);
                break;
            default:
                break;
            }
        }
    }
    //========= Submenu Thống kê ===========//
    inline void MenuConThongKe(DanhSachDauSach& DanhSachDauSach, DocGiaNode*& Root) {
        const std::string TieuDe = "THONG KE";
        while (true) {
            int Ch =
                MenuMuiTen(8, 0, TieuDe, { "1. Top 10 sach muon nhieu nhat", "2. Danh sach muon qua han", "0. Quay lai" });
            if (Ch < 0) {
                return;
            }
            if (Ch == 2) {
                return;
            }
            switch (Ch) {
            case 0:
                ThongKeTop10LuotMuonTUI(DanhSachDauSach);
                break;
            case 1:
                ThongKeDanhSachQuaHanTUI(DanhSachDauSach, Root);
                break;
            default:
                break;
            }
        }
    }
    //========= Menu chính ===========//
    inline void MenuChinhTUI(DanhSachDauSach& DanhSachDauSach, DocGiaNode*& Root) {
        const std::string TieuDe = "QUAN LY THU VIEN - MENU CHINH";
        while (true) {
            int Ch = MenuMuiTen(
                8, 0, TieuDe, { "1. Quan ly doc gia", "2. Quan ly dau sach", "3. Muon / Tra", "4. Thong ke", "0. Thoat" });
            if (Ch < 0) {
                return;
            }
            if (Ch == 4) {
                return;
            }
            switch (Ch) {
            case 0:
                MenuConDocGia(DanhSachDauSach, Root);
                break;
            case 1:
                MenuConDauSach(DanhSachDauSach, Root);
                break;
            case 2:
                MenuConMuonTra(DanhSachDauSach, Root);
                break;
            case 3:
                MenuConThongKe(DanhSachDauSach, Root);
                break;
            default:
                break;
            }
        }
    }
} // namespace menutui

