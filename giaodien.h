#pragma once
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <limits>
#include "cautruc.h"
#include "dausach.h"
#include "docgia.h"
#include "muontra.h"
#include "thongke.h"
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <conio.h>
#endif

namespace tui {
    enum : int { MauThanhCong = 10, MauCanhBao = 12, MauNoiBat = 14 };
    enum : int { PhimKhongXacDinh = 0, PhimTrai = 1, PhimPhai = 2, PhimLen = 3, PhimXuong = 4, PhimEnter = 5, PhimEsc = 6 };
    struct KeyEvent {
        int Phim;
        int KyTu;
        KeyEvent() : Phim(PhimKhongXacDinh), KyTu(0) {
        }
    };
#ifdef _WIN32
    inline void DiChuyenConTro(int X, int Y) {
        HANDLE ChieuCao = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD KyTuDoc;
        KyTuDoc.X = (SHORT)((X > 0) ? (X - 1) : 0);
        KyTuDoc.Y = (SHORT)((Y > 0) ? (Y - 1) : 0);
        SetConsoleCursorPosition(ChieuCao, KyTuDoc);
    }
    inline void XoaManHinh() {
        HANDLE ChieuCao = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO Csbi;
        DWORD SoLuongTimThay;
        DWORD CellCount;
        COORD Home = { 0, 0 };
        if (!GetConsoleScreenBufferInfo(ChieuCao, &Csbi)) {
            std::system("cls");
            return;
        }
        CellCount = (DWORD)Csbi.dwSize.X * (DWORD)Csbi.dwSize.Y;
        FillConsoleOutputCharacter(ChieuCao, (TCHAR)' ', CellCount, Home, &SoLuongTimThay);
        FillConsoleOutputAttribute(ChieuCao, Csbi.wAttributes, CellCount, Home, &SoLuongTimThay);
        SetConsoleCursorPosition(ChieuCao, Home);
    }
    inline void HienConTro() {
        HANDLE ChieuCao = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO Info;
        if (GetConsoleCursorInfo(ChieuCao, &Info)) {
            Info.bVisible = TRUE;
            SetConsoleCursorInfo(ChieuCao, &Info);
        }
    }
    inline void AnConTro() {
        HANDLE ChieuCao = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO Info;
        if (GetConsoleCursorInfo(ChieuCao, &Info)) {
            Info.bVisible = FALSE;
            SetConsoleCursorInfo(ChieuCao, &Info);
        }
    }
    inline void DatMau(int ColorCode) {
        HANDLE ChieuCao = GetStdHandle(STD_OUTPUT_HANDLE);
        static WORD DefaultAttr = 0;
        static bool Cached = false;
        if (!Cached) {
            CONSOLE_SCREEN_BUFFER_INFO Csbi;
            if (GetConsoleScreenBufferInfo(ChieuCao, &Csbi)) {
                DefaultAttr = Csbi.wAttributes;
                Cached = true;
            }
        }
        WORD Attr = DefaultAttr;
        switch (ColorCode) {
        case MauThanhCong:
            Attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case MauCanhBao:
            Attr = FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case MauNoiBat:
            Attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        default:
            Attr = DefaultAttr;
            break;
        }
        SetConsoleTextAttribute(ChieuCao, Attr);
    }
    inline void DatLaiMau() {
        HANDLE ChieuCao = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO Csbi;
        if (GetConsoleScreenBufferInfo(ChieuCao, &Csbi)) {
            SetConsoleTextAttribute(ChieuCao, Csbi.wAttributes);
        }
    }
    inline KeyEvent DocPhim() {
        KeyEvent SuKienPhim;
        int KyTuDoc = _getch();
        if (KyTuDoc == 0 || KyTuDoc == 224) {
            int DauPhanCachThuHai = _getch();
            switch (DauPhanCachThuHai) {
            case 72:
                SuKienPhim.Phim = PhimLen;
                break;
            case 80:
                SuKienPhim.Phim = PhimXuong;
                break;
            case 75:
                SuKienPhim.Phim = PhimTrai;
                break;
            case 77:
                SuKienPhim.Phim = PhimPhai;
                break;
            default:
                SuKienPhim.Phim = PhimKhongXacDinh;
                break;
            }
            SuKienPhim.KyTu = DauPhanCachThuHai;
        }
        else if (KyTuDoc == 13) {
            SuKienPhim.Phim = PhimEnter;
            SuKienPhim.KyTu = KyTuDoc;
        }
        else if (KyTuDoc == 27) {
            SuKienPhim.Phim = PhimEsc;
            SuKienPhim.KyTu = KyTuDoc;
        }
        else {
            switch (KyTuDoc) {
            case 'w':
            case 'W':
                SuKienPhim.Phim = PhimLen;
                break;
            case 's':
            case 'S':
                SuKienPhim.Phim = PhimXuong;
                break;
            case 'a':
            case 'A':
                SuKienPhim.Phim = PhimTrai;
                break;
            case 'd':
            case 'D':
                SuKienPhim.Phim = PhimPhai;
                break;
            default:
                SuKienPhim.Phim = PhimKhongXacDinh;
                break;
            }
            SuKienPhim.KyTu = KyTuDoc;
        }
        return SuKienPhim;
    }
#else
    inline void DiChuyenConTro(int X, int Y) {
        if (X < 1) {
            X = 1;
        }
        if (Y < 1) {
            Y = 1;
        }
        std::printf("\x1b[%d;%dH", Y, X);
        std::fflush(stdout);
    }
    inline void XoaManHinh() {
        std::printf("\x1b[2J\x1b[H");
        std::fflush(stdout);
    }
    inline void HienConTro() {
        std::printf("\x1b[?25h");
        std::fflush(stdout);
    }
    inline void AnConTro() {
        std::printf("\x1b[?25l");
        std::fflush(stdout);
    }
    inline void DatMau(int ColorCode) {
        int Ansi = 37;
        switch (ColorCode) {
        case MauThanhCong:
            Ansi = 32;
            break;
        case MauCanhBao:
            Ansi = 31;
            break;
        case MauNoiBat:
            Ansi = 33;
            break;
        default:
            Ansi = 37;
            break;
        }
        std::printf("\x1b[%dm", Ansi);
        std::fflush(stdout);
    }
    inline void DatLaiMau() {
        std::printf("\x1b[0m");
        std::fflush(stdout);
    }
    inline KeyEvent DocPhim() {
        KeyEvent SuKienPhim;
        int KyTuDoc = std::getchar();
        if (KyTuDoc == 27) {
            int DauPhanCachThuNhat = std::getchar();
            if (DauPhanCachThuNhat == '[') {
                int DauPhanCachThuHai = std::getchar();
                switch (DauPhanCachThuHai) {
                case 'A':
                    SuKienPhim.Phim = PhimLen;
                    break;
                case 'B':
                    SuKienPhim.Phim = PhimXuong;
                    break;
                case 'C':
                    SuKienPhim.Phim = PhimPhai;
                    break;
                case 'D':
                    SuKienPhim.Phim = PhimTrai;
                    break;
                default:
                    SuKienPhim.Phim = PhimEsc;
                    break;
                }
                SuKienPhim.KyTu = DauPhanCachThuHai;
            }
            else {
                SuKienPhim.Phim = PhimEsc;
                SuKienPhim.KyTu = DauPhanCachThuNhat;
            }
        }
        else if (KyTuDoc == '\n' || KyTuDoc == '\r') {
            SuKienPhim.Phim = PhimEnter;
            SuKienPhim.KyTu = KyTuDoc;
        }
        else {
            switch (KyTuDoc) {
            case 'w':
            case 'W':
                SuKienPhim.Phim = PhimLen;
                break;
            case 's':
            case 'S':
                SuKienPhim.Phim = PhimXuong;
                break;
            case 'a':
            case 'A':
                SuKienPhim.Phim = PhimTrai;
                break;
            case 'd':
            case 'D':
                SuKienPhim.Phim = PhimPhai;
                break;
            default:
                SuKienPhim.Phim = PhimKhongXacDinh;
                break;
            }
            SuKienPhim.KyTu = KyTuDoc;
        }
        return SuKienPhim;
    }
#endif
    inline void VeDuongNgang(int X, int Y, int ChieuRong) {
        if (ChieuRong <= 0) {
            return;
        }
        DiChuyenConTro(X, Y);
        for (int i = 0; i < ChieuRong; ++i) {
            std::cout << "-";
        }
    }
    inline void VeDuongDoc(int X, int Y, int ChieuCao) {
        for (int i = 0; i < ChieuCao; ++i) {
            DiChuyenConTro(X, Y + i);
            std::cout << "|";
        }
    }
    inline void VeKhung(int X, int Y, int ChieuRong, int ChieuCao, const std::string& TieuDe) {
        tui::DatMau(tui::MauNoiBat);
        if (ChieuRong < 4) {
            ChieuRong = 4;
        }
        if (ChieuCao < 3) {
            ChieuCao = 3;
        }
        DiChuyenConTro(X, Y);
        std::cout << "+";
        DiChuyenConTro(X + ChieuRong - 1, Y);
        std::cout << "+";
        DiChuyenConTro(X, Y + ChieuCao - 1);
        std::cout << "+";
        DiChuyenConTro(X + ChieuRong - 1, Y + ChieuCao - 1);
        std::cout << "+";
        VeDuongNgang(X + 1, Y, ChieuRong - 2);
        VeDuongNgang(X + 1, Y + ChieuCao - 1, ChieuRong - 2);
        VeDuongDoc(X, Y + 1, ChieuCao - 2);
        VeDuongDoc(X + ChieuRong - 1, Y + 1, ChieuCao - 2);
        if (!TieuDe.empty() && ChieuRong > 4) {
            std::string ThoiGianHienTai = TieuDe;
            if ((int)ThoiGianHienTai.size() > ChieuRong - 4) {
                ThoiGianHienTai = ThoiGianHienTai.substr(0, ChieuRong - 4);
            }
            DiChuyenConTro(X + 2, Y);
            std::cout << " " << ThoiGianHienTai << " ";
        }
    }
    inline void InHuongDanCuoiTrang(int X, int Y, const std::string& NoiDung) {
        DiChuyenConTro(X, Y);
        DatMau(MauNoiBat);
        std::cout << NoiDung;
        DatLaiMau();
    }
    inline void NhanPhimBatKyDeQuayLai(int X, int Y) {
        DiChuyenConTro(X, Y);
        DatMau(MauNoiBat);
        std::cout << "[Esc] Quay lai";
        DatLaiMau();
        while (true) {
            KeyEvent SuKienPhim = DocPhim();
            if (SuKienPhim.Phim == PhimEsc) {
                break;
            }
        }
    }
    inline void XoaVung(int X, int Y, int ChieuRong, int ChieuCao) {
        for (int KetQuaNhap = 0; KetQuaNhap < ChieuCao; ++KetQuaNhap) {
            DiChuyenConTro(X, Y + KetQuaNhap);
            std::cout << std::string(ChieuRong, ' ');
        }
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
            if (Ch >= 32 && Ch <= 126) {
                if ((int)KetQuaDauRa.size() < DoDaiToiDa) {
                    KetQuaDauRa.push_back(Ch);
                    std::cout << Ch;
                }
            }
        }
    }
}

// ======================= FORM VÀ MENU =======================
namespace giaodien {
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
    inline int MenuMuiTen(int X, int /*y*/, const std::string& TieuDe, const std::vector<std::string>& DanhSachMuc) {
        const int MENUY = 6;
        int ChiSoHienTai = 0;
        int ChieuRong = 0, ChieuCao = 0;
        VeKhungMenu(TieuDe, (int)DanhSachMuc.size(), ChieuRong, ChieuCao);
        int FooterY = 1 + ChieuCao - 2;
        int CanLeTrai = 0;
        for (auto& ChuoiNhap : DanhSachMuc) {
            CanLeTrai = std::max(CanLeTrai, (int)ChuoiNhap.size());
        }
        auto PaintOne = [&](int ChiSo, bool DangChon) {
            tui::DiChuyenConTro(X, MENUY + ChiSo);
            if (DangChon) {
                std::cout << "\x1b[7m" << DanhSachMuc[ChiSo];
                int Sp = CanLeTrai - (int)DanhSachMuc[ChiSo].size();
                if (Sp > 0) {
                    std::cout << std::string(Sp, ' ');
                }
                std::cout << "\x1b[0m";
                tui::DatMau(tui::MauNoiBat);
            }
            else {
                tui::DatMau(tui::MauNoiBat);
                std::cout << DanhSachMuc[ChiSo];
                int Sp = CanLeTrai - (int)DanhSachMuc[ChiSo].size();
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
    inline std::string CanLeTrai(const std::string& ChuoiNhap, int ChieuRong) {
        if ((int)ChuoiNhap.size() >= ChieuRong) {
            return ChuoiNhap.substr(0, ChieuRong);
        }
        return ChuoiNhap + std::string(ChieuRong - (int)ChuoiNhap.size(), ' ');
    }
    inline std::string CanLePhai(const std::string& ChuoiNhap, int ChieuRong) {
        if ((int)ChuoiNhap.size() >= ChieuRong) {
            return ChuoiNhap.substr(0, ChieuRong);
        }
        return std::string(ChieuRong - (int)ChuoiNhap.size(), ' ') + ChuoiNhap;
    }
    inline std::string TaoDuongKe(int SoKyTu) {
        return std::string(SoKyTu, '-');
    }
    inline void XoaDongTaiViTri(int X, int Y, int ChieuRong) {
        tui::DiChuyenConTro(X, Y);
        std::cout << std::string(ChieuRong, ' ');
    }
    inline int DocDongChoPhepThoatKhiRong(int X, int Y, int DoDaiToiDa, std::string& KetQuaDauRa) {
        return tui::DocDongChoPhepThoatKhiRong(X, Y, DoDaiToiDa, KetQuaDauRa);
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
        std::string ThongBaoLoi;
        bool ThemThanhCong = ThemDocGia(Root, MaTheCanXuLy, HoNhap, TenNhap, PhaiNhap, TrangThaiNhap, &ThongBaoLoi);
        tui::DiChuyenConTro(X0, FooterY - 2);
        if (ThemThanhCong) {
            tui::DatMau(tui::MauThanhCong);
            std::cout << "Da them doc gia ma the " << MaTheCanXuLy << ".";
        }
        else {
            tui::DatMau(tui::MauCanhBao);
            std::cout << ThongBaoLoi;
        }
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
        std::string ThongBaoLoi;
        bool CapNhatThanhCong = CapNhatThongTinDocGia(Root, MaTheCanXuLy, InHo, InTen, NewPhai, NewTrangThai, &ThongBaoLoi);
        tui::DiChuyenConTro(X0, FooterY - 2);
        if (CapNhatThanhCong) {
            tui::DatMau(tui::MauThanhCong);
            std::cout << "Da cap nhat doc gia.";
        }
        else {
            tui::DatMau(tui::MauCanhBao);
            std::cout << ThongBaoLoi;
        }
        tui::DatLaiMau();
        tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
    }
    //==================== In danh sách độc giả ====================//
    static const int DoRongCotMATHE = 9, DoRongCotHODEM = 32, DoRongCotTEN = 12, DoRongCotGIOITINH = 9,
        DoRongCotTRANGTHAI = 11;
    inline int VeTieuDeBangDocGia(const std::string& /*title*/, int /*w*/ = 118, int /*h*/ = 24) {
        int Y = 5;
        tui::DatMau(tui::MauNoiBat);
        tui::DiChuyenConTro(4, Y++);
        std::cout << CanLeTrai("Ma the", DoRongCotMATHE) << " | " << CanLeTrai("Ho va Ten Dem", DoRongCotHODEM) << " | "
            << CanLeTrai("Ten", DoRongCotTEN) << " | " << CanLeTrai("Gioi tinh", DoRongCotGIOITINH) << " | "
            << CanLeTrai("Trang thai", DoRongCotTRANGTHAI);
        auto TaoDuongKe = [](int SoPhanTu) {
            return std::string(SoPhanTu, '-');
            };
        tui::DiChuyenConTro(4, Y++);
        std::cout << TaoDuongKe(DoRongCotMATHE) << "-+-" << TaoDuongKe(DoRongCotHODEM) << "-+-" << TaoDuongKe(DoRongCotTEN) << "-+-"
            << TaoDuongKe(DoRongCotGIOITINH) << "-+-" << TaoDuongKe(DoRongCotTRANGTHAI);
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
                std::cout << CanLeTrai(std::to_string(DocGiaCanXuLy->MaThe), DoRongCotMATHE) << " | "
                    << CanLeTrai(DocGiaCanXuLy->Ho, DoRongCotHODEM) << " | "
                    << CanLeTrai(DocGiaCanXuLy->Ten, DoRongCotTEN) << " | "
                    << CanLeTrai(DocGiaCanXuLy->Phai, DoRongCotGIOITINH) << " | "
                    << CanLeTrai((DocGiaCanXuLy->TrangThaiThe == 1 ? "Hoat dong" : "Khoa"), DoRongCotTRANGTHAI);
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
    // Lập và hiển thị danh sách độc giả theo tên và họ
    inline void InDocGiaTheoTenHoTUI(DocGiaNode* Root){
        static DocGia* MangDuLieu[MaxDocGia];
        int SoPhanTu = 0;
        LapDanhSachDocGiaTheoTenHo(Root, MangDuLieu, SoPhanTu);
        InBangDocGiaTUI(MangDuLieu, SoPhanTu, "QUAN LY DOC GIA  >  IN DANH SACH  (sap theo Ten + Ho)");
    }
    // Lập và hiển thị danh sách độc giả theo mã thẻ
    inline void InDocGiaTheoMaTheTUI(DocGiaNode* Root){
        static DocGia* MangDuLieu[MaxDocGia];
        int SoPhanTu = 0;
        LapDanhSachDocGiaTheoMaThe(Root, MangDuLieu, SoPhanTu);
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
        std::string ThongBaoLoi;
        bool ThemThanhCong = ThemDauSachMoi(DanhSachDauSach, ISBNCanXuLy, TenNhap, SoTrangNhap, TacGiaNhap, NamXuatBanNhap, TheLoaiNhap, SoLuongCanXuLy, &ThongBaoLoi);
        tui::DiChuyenConTro(X0, FooterY - 2);
        if (ThemThanhCong) {
            tui::DatMau(tui::MauThanhCong);
            std::cout << "Da them dau sach [" << ISBNCanXuLy << "].";
        }
        else {
            tui::DatMau(tui::MauCanhBao);
            std::cout << ThongBaoLoi;
        }
        tui::DatLaiMau();
        tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
    }
    //================ Xóa đầu sách =================//
    inline void FormXoaDauSachTUI(DanhSachDauSach& DanhSachDauSach) {
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

        int TongSoBanSao = 0;
        int SoSachDangMuon = 0;
        std::string ThongBaoLoi;
        if (!LayThongTinDauSachDeXoa(DanhSachDauSach, ISBNCanXuLy, TongSoBanSao, SoSachDangMuon, &ThongBaoLoi)) {
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            std::cout << ThongBaoLoi;
            tui::DatLaiMau();
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        tui::DiChuyenConTro(X0, Y++);
        std::cout << "Tong so ban sao : " << TongSoBanSao;
        tui::DiChuyenConTro(X0, Y++);
        std::cout << "Dang cho muon   : " << SoSachDangMuon;
        if (SoSachDangMuon > 0) {
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
        ThongBaoLoi.clear();
        tui::DiChuyenConTro(X0, Y + 3);
        if (XoaDauSach(DanhSachDauSach, ISBNCanXuLy, &ThongBaoLoi)) {
            tui::DatMau(tui::MauThanhCong);
            std::cout << "Da xoa dau sach.";
        }
        else {
            tui::DatMau(tui::MauCanhBao);
            std::cout << ThongBaoLoi;
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
        DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy.c_str());
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
        int SlTarget = 0;
        int HasSL = 0;
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
                std::string ThongBaoKiemTra;
                if (!KiemTraSoLuongBanSaoMoi(DuLieuSach, GiaTriCanThem, &ThongBaoKiemTra)) {
                    tui::DiChuyenConTro(X0, FooterY - 2);
                    tui::DatMau(tui::MauCanhBao);
                    std::cout << ThongBaoKiemTra;
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
        int ConfirmY = SlY + 2;
        if (ConfirmY > FooterY - 3) {
            ConfirmY = FooterY - 3;
        }
        tui::DiChuyenConTro(X0, ConfirmY);
        std::cout << "Xac nhan cap nhat: ";
        int Ok = ChonRadioTaiViTri(X0 + 22, ConfirmY, { "Co", "Khong" }, 0);
        if (Ok != 0) {
            tui::DiChuyenConTro(X0, FooterY - 2);
            std::cout << "Da huy cap nhat.";
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }

        std::string ThongBaoLoi;
        bool CapNhatThanhCong = CapNhatThongTinDauSach(DanhSachDauSach, ISBNCanXuLy, InTen, InTG, InNam, InST, HasSL, SlTarget, &ThongBaoLoi);
        tui::DiChuyenConTro(X0, FooterY - 2);
        if (CapNhatThanhCong) {
            tui::DatMau(tui::MauThanhCong);
            std::cout << "Da cap nhat dau sach.";
        }
        else {
            tui::DatMau(tui::MauCanhBao);
            std::cout << ThongBaoLoi;
        }
        tui::DatLaiMau();
        tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
    }
    //================ Danh sách theo thể loại =================//
    inline void InDauSachTheoTheLoaiTUI(const DanhSachDauSach& DanhSachDauSach) {
        const int ChieuRong = 118, ChieuCao = 30, X0 = 4;
        const int FooterY = 1 + ChieuCao - 2;
        const int YMax = 1 + ChieuCao - 3;
        const int MAXLINES = 5000;
        static ThongTinDauSachTheoTheLoai DanhSachDaXuLy[MaxDauSach];
        int SoPhanTu = 0;
        LapDanhSachDauSachTheoTheLoai(DanhSachDauSach, DanhSachDaXuLy, SoPhanTu, MaxDauSach);
        if (SoPhanTu == 0) {
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
        static std::string CacDongKetQua[MAXLINES];
        int TongSoDong = 0;
        for (int i = 0; i < SoPhanTu; i++) {
            ThongTinDauSachTheoTheLoai& DongDaXuLy = DanhSachDaXuLy[i];
            DauSach* DuLieuSach = DongDaXuLy.DuLieuSach;

            if (DongDaXuLy.LaDauNhom == 1) {
                if (TongSoDong > 0 && TongSoDong < MAXLINES) {
                    CacDongKetQua[TongSoDong++] = "";
                }
                if (TongSoDong < MAXLINES) {
                    CacDongKetQua[TongSoDong++] =
                        std::string("The loai: ") + DuLieuSach->TheLoai +
                        " (So dau sach: " +
                        std::to_string(DongDaXuLy.SoDauSachTrongNhom) +
                        ")";
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
            std::string DongDuLieu =
                std::string("  - [") + DuLieuSach->ISBN + "] " +
                TenNhap + " | " + TacGiaTam + " | " +
                std::to_string(DuLieuSach->NamXuatBan) +
                " | So ban sao: " +
                std::to_string(DuLieuSach->SoLuongBanSao);
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
        std::string TuKhoaDaChuanHoa = ChuyenThanhChuHoa(CatKhoangTrangHaiDau(TuKhoaTimKiem));
        TimDauSachTheoTen(DanhSachDauSach, TuKhoaDaChuanHoa.c_str(), DaTimThay, SoLuongTimThay);
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
            const DanhMucSachNode* DanhSachBanSao[5000];
            int SoLuongBanSao = 0;
            LayDanhSachBanSao(DuLieuSach, DanhSachBanSao, SoLuongBanSao, 5000);
            std::string ChiSoHienTai = "    Ma ban sao: ";
            int CountInLine = 0;
            for (int ChiSoBanSao = 0; ChiSoBanSao < SoLuongBanSao; ChiSoBanSao++) {
                const DanhMucSachNode* BanSao = DanhSachBanSao[ChiSoBanSao];
                std::string Token =
                    std::string(BanSao->MaSach) +
                    " (" +
                    TrangThaiStr(BanSao->TrangThai) +
                    ")";
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
    //================ Mượn sách ==============
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
        const int DoRongCotSTT = 4, DoRongCotMS = 13, DoRongCotISBN = 11, DoRongCotTEN = 60, DoRongCotNGAY = 10;
        int TableY = Y0 + 7;
        tui::DiChuyenConTro(X0, TableY++);
        std::cout << CanLeTrai("STT", DoRongCotSTT) << " | " << CanLeTrai("MaSach", DoRongCotMS) << " | " << CanLeTrai("ISBN", DoRongCotISBN)
            << " | " << CanLeTrai("Ten sach", DoRongCotTEN) << " | " << CanLeTrai("Ngay muon", DoRongCotNGAY);
        tui::DiChuyenConTro(X0, TableY++);
        std::cout << TaoDuongKe(DoRongCotSTT) << "-+-" << TaoDuongKe(DoRongCotMS) << "-+-" << TaoDuongKe(DoRongCotISBN) << "-+-"
            << TaoDuongKe(DoRongCotTEN) << "-+-" << TaoDuongKe(DoRongCotNGAY);
        MuonTraNode* DanhSachDangMuon[100];
        int SoLuongDangMuon = 0;
        LayDanhSachPhieuDangMuon(PNode->ThongTin, DanhSachDangMuon, SoLuongDangMuon, 100);
        int Stt = 0;
        for (int i = 0; i < SoLuongDangMuon; i++) {
            MuonTraNode* PhieuMuonTraHienTai = DanhSachDangMuon[i];
            char ISBNThuNhat[15];
            LayISBNTuMaSach(PhieuMuonTraHienTai->MaSach, ISBNThuNhat, 15);
            std::string Ten0;
            DauSach* Ds0 = TimDauSachTheoISBN(DanhSachDauSach, ISBNThuNhat);
            if (Ds0) {
                Ten0 = Ds0->TenSach;
            }
            if (TableY >= FooterY - 3) {
                break;
            }
            tui::DiChuyenConTro(X0, TableY++);
            std::cout << CanLeTrai(std::to_string(++Stt), DoRongCotSTT) << " | " << CanLeTrai(PhieuMuonTraHienTai->MaSach, DoRongCotMS)
                << " | " << CanLeTrai(ISBNThuNhat, DoRongCotISBN) << " | " << CanLeTrai(Ten0, DoRongCotTEN) << " | "
                << CanLeTrai(ChuyenNgayThanhChuoi(PhieuMuonTraHienTai->NgayMuon), DoRongCotNGAY);
        }
        if (Stt == 0) {
            tui::DiChuyenConTro(X0, TableY++);
            std::cout << "(Khong co sach dang muon.)";
        }
        std::string ISBNCanXuLy;
        if (DocDongChoPhepThoatKhiRong(ISBNX, ISBNY, 20, ISBNCanXuLy) == -1) {
            return;
        }
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
            if (NgayMuon.Nam < 1500 || SoSanhNgay(NgayMuon, LayNgayHienTai()) > 0) {
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
        std::string MaSachDaMuon;
        std::string ThongBaoLoi;
        if (!MuonSachTheoMaTheVaISBN(Root, DanhSachDauSach, MaTheCanXuLy, ISBNCanXuLy, NgayMuon, &MaSachDaMuon, &ThongBaoLoi)) {
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            std::cout << ThongBaoLoi;
            tui::DatLaiMau();
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        tui::DiChuyenConTro(X0, FooterY - 2);
        tui::DatMau(tui::MauThanhCong);
        std::cout << "Da MUON thanh cong: "
            << ISBNCanXuLy
            << " | "
            << LayTenSachTheoISBN(DanhSachDauSach, ISBNCanXuLy.c_str())
            << " | MaSach: "
            << MaSachDaMuon;
        tui::DatLaiMau();
        tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
    }
    //================ Trả sách ==============
    inline void FormTraSachTUI(DanhSachDauSach& DanhSachDauSach, DocGiaNode*& Root) {
        const int ChieuRong = 118, ChieuCao = 24, X0 = 4, Y0 = 3;
        const int FooterY = 1 + ChieuCao - 2;
        const int YMax = 1 + ChieuCao - 3;
        const int DoRongCotSTT = 4, DoRongCotMS = 13, DoRongCotISBN = 11, DoRongCotTEN = 42, DoRongCotNGAY = 10;
        tui::XoaManHinh();
        tui::VeKhung(2, 1, ChieuRong, ChieuCao, "MUON / TRA  >  TRA SACH");
        int Y = Y0 + 1;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Nhap MA THE: ";
        int MaX = X0 + 14, MaY = Y;
        tui::InHuongDanCuoiTrang(4, FooterY, "[Enter] Xac nhan  -  [Esc] Quay lai");
        LamSachBoDemNhap();
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
        LayDanhSachPhieuDangMuon(PNode->ThongTin, DanhSachDong, SoDong, 100);
        Y += 2;
        tui::DiChuyenConTro(X0, Y++);
        std::cout << "Danh sach DANG MUON:";
        if (SoDong == 0) {
            tui::DiChuyenConTro(X0, Y++);
            std::cout << "(Doc gia khong co sach dang muon.)";
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        std::string Header = CanLeTrai("STT", DoRongCotSTT) + " | " + CanLeTrai("MaSach", DoRongCotMS) + " | " +
            CanLeTrai("ISBN", DoRongCotISBN) + " | " + CanLeTrai("Ten sach", DoRongCotTEN) + " | " +
            CanLeTrai("Ngay muon", DoRongCotNGAY);
        std::string Sep = TaoDuongKe(DoRongCotSTT) + "-+-" + TaoDuongKe(DoRongCotMS) + "-+-" + TaoDuongKe(DoRongCotISBN) + "-+-" +
            TaoDuongKe(DoRongCotTEN) + "-+-" + TaoDuongKe(DoRongCotNGAY);
        tui::DiChuyenConTro(X0, Y++);
        std::cout << Header;
        tui::DiChuyenConTro(X0, Y++);
        std::cout << Sep;
        for (int i = 0; i < SoDong; ++i) {
            MuonTraNode* PhieuMuonTraHienTai = DanhSachDong[i];
            char ISBNCanXuLy[15];
            LayISBNTuMaSach(PhieuMuonTraHienTai->MaSach, ISBNCanXuLy, 15);
            DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy);
            std::string TenNhap = DuLieuSach ? DuLieuSach->TenSach : "";
            if ((int)TenNhap.size() > DoRongCotTEN) {
                TenNhap = TenNhap.substr(0, DoRongCotTEN);
            }
            std::string DongDuLieu = CanLeTrai(std::to_string(i + 1), DoRongCotSTT) + " | " +
                CanLeTrai(PhieuMuonTraHienTai->MaSach, DoRongCotMS) + " | " +
                CanLeTrai(ISBNCanXuLy, DoRongCotISBN) + " | " + CanLeTrai(TenNhap, DoRongCotTEN) + " | " +
                CanLeTrai(ChuyenNgayThanhChuoi(PhieuMuonTraHienTai->NgayMuon), DoRongCotNGAY);
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
        int PromptY = std::min(FooterY - 6, Y + 1);
        tui::DiChuyenConTro(X0, PromptY);
        std::cout << "Nhap STT de tra (hoac bo trong de nhap ma sach de tra): ";
        int ChooseX = X0 + 55, ChooseY = PromptY;
        LamSachBoDemNhap();
        std::string SChon;
        if (DocDongChoPhepThoatKhiRong(ChooseX, ChooseY, 8, SChon) == -1) {
            return;
        }
        std::string MaSachCanTra;
        if (!SChon.empty()) {
            int ChiSo = -1;
            try {
                ChiSo = std::stoi(SChon);
            }
            catch (...) {
                ChiSo = -1;
            }
            if (1 <= ChiSo && ChiSo <= SoDong) {
                MaSachCanTra = DanhSachDong[ChiSo - 1]->MaSach;
            }
        }
        if (MaSachCanTra.empty()) {
            tui::DiChuyenConTro(X0, ChooseY + 1);
            std::cout << "Nhap MaSach can tra: ";
            int MsX = X0 + 23, MsY = ChooseY + 1;
            if (DocDongChoPhepThoatKhiRong(MsX, MsY, 20, MaSachCanTra) == -1) {
                return;
            }
        }
        if (MaSachCanTra.empty()) {
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            std::cout << "Lua chon khong hop le.";
            tui::DatLaiMau();
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
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
            if (NgayTra.Nam < 1500 || SoSanhNgay(NgayTra, LayNgayHienTai()) > 0) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Loi: Ngay tra khong duoc lon hon ngay hien tai.";
                tui::DatLaiMau();
                tui::DiChuyenConTro(NtX, NtY);
                std::cout << std::string(16, ' ');
                continue;
            }
            break;
        }
        int TongSoNgay = 0;
        int SoNgayTre = 0;
        std::string ThongBaoLoi;
        if (!TraSachTheoMaTheVaMaSach(Root, DanhSachDauSach, MaTheCanXuLy, MaSachCanTra, NgayTra, &TongSoNgay, &SoNgayTre, &ThongBaoLoi)) {
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            std::cout << ThongBaoLoi;
            tui::DatLaiMau();
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        tui::DiChuyenConTro(X0, FooterY - 2);
        std::cout
            << "Da TRA sach: "
            << MaSachCanTra
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
        tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
    }
    // =========== Liệt Kê Sách Đang Mượn Của Độc Giả ================//
    inline void FormInSachDangMuonTUI(const DanhSachDauSach& DanhSachDauSach, DocGiaNode* Root) {
        const int ChieuRong = 118, ChieuCao = 24, X0 = 4, Y0 = 3;
        const int FooterY = 1 + ChieuCao - 2, YMax = 1 + ChieuCao - 3;
        const int DoRongCotSTT = 4, DoRongCotMS = 13, DoRongCotISBN = 11, DoRongCotTEN = 42, DoRongCotNGAY = 10,
            DoRongCotSONG = 7;
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
        ThongTinSachDangMuon DanhSachDong[100];
        int SoDong = 0;
        DocGia* DocGiaCanHienThi = NULL;
        std::string ThongBaoLoi;
        if (!LapDanhSachSachDangMuonCuaDocGia(DanhSachDauSach, Root, MaTheCanXuLy, NgayHienTai, DanhSachDong, SoDong, DocGiaCanHienThi, 100, &ThongBaoLoi)) {
            tui::DiChuyenConTro(X0, FooterY - 2);
            tui::DatMau(tui::MauCanhBao);
            std::cout << ThongBaoLoi;
            tui::DatLaiMau();
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        tui::DiChuyenConTro(X0, Y++);
        {
            std::string Name = DocGiaCanHienThi->Ho +
                std::string(" ") +
                DocGiaCanHienThi->Ten;
            std::cout << "Doc gia: "
                << DocGiaCanHienThi->MaThe
                << " | "
                << Name;
        }
        Y++;
        if (SoDong == 0) {
            tui::DiChuyenConTro(X0, Y++);
            std::cout << "(Khong co sach dang muon nao truoc hoac trong ngay " << ChuyenNgayThanhChuoi(NgayHienTai) << ".)";
            tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
            return;
        }
        std::string Header = CanLeTrai("STT", DoRongCotSTT) + " | " + CanLeTrai("MaSach", DoRongCotMS) + " | " +
            CanLeTrai("ISBN", DoRongCotISBN) + " | " + CanLeTrai("Ten sach", DoRongCotTEN) + " | " +
            CanLeTrai("Ngay muon", DoRongCotNGAY) + " | " + CanLeTrai("So ngay", DoRongCotSONG);
        std::string Sep = TaoDuongKe(DoRongCotSTT) + "-+-" + TaoDuongKe(DoRongCotMS) + "-+-" + TaoDuongKe(DoRongCotISBN) + "-+-" +
            TaoDuongKe(DoRongCotTEN) + "-+-" + TaoDuongKe(DoRongCotNGAY) + "-+-" + TaoDuongKe(DoRongCotSONG);
        tui::DiChuyenConTro(X0, Y++);
        std::cout << Header;
        tui::DiChuyenConTro(X0, Y++);
        std::cout << Sep;
        for (int i = 0; i < SoDong; ++i) {
            ThongTinSachDangMuon& KetQuaNhap = DanhSachDong[i];
            MuonTraNode* PhieuMuon = KetQuaNhap.PhieuMuon;
            const DauSach* DuLieuSach = KetQuaNhap.DuLieuSach;
            std::string TenCut = "";
            if (DuLieuSach != NULL) {
                TenCut = DuLieuSach->TenSach;
            }
            if ((int)TenCut.size() > DoRongCotTEN) {
                TenCut = TenCut.substr(0, DoRongCotTEN);
            }
            std::string MaSach = "";
            std::string ISBN = "";
            NgayThangNam NgayMuon{ 0, 0, 0 };
            if (PhieuMuon != NULL) {
                MaSach = PhieuMuon->MaSach;
                NgayMuon = PhieuMuon->NgayMuon;
            }
            if (DuLieuSach != NULL) {
                ISBN = DuLieuSach->ISBN;
            }
            std::string DongDuLieu =
                CanLeTrai(std::to_string(i + 1), DoRongCotSTT) + " | " +
                CanLeTrai(MaSach, DoRongCotMS) + " | " +
                CanLeTrai(ISBN, DoRongCotISBN) + " | " +
                CanLeTrai(TenCut, DoRongCotTEN) + " | " +
                CanLeTrai(
                    ChuyenNgayThanhChuoi(NgayMuon),
                    DoRongCotNGAY
                ) + " | " +
                CanLeTrai(
                    std::to_string(KetQuaNhap.TongSoNgay),
                    DoRongCotSONG
                );
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
    //========= Thống Kê Top 10 ===============
    inline void ThongKeTop10LuotMuonTUI(const DanhSachDauSach& DanhSachDauSach) {
        const int ChieuRong = 118, ChieuCao = 24;
        const int FooterY = 1 + ChieuCao - 2;
        const int DoRongCotSTT = 4, DoRongCotISBN = 12, DoRongCotTEN = 47, DoRongCotCNT = 14;
        DauSach* Top10[10];
        int SoLuongTimThay = 0;
        ThongKeTop10TheoLuotMuon(DanhSachDauSach, Top10, SoLuongTimThay);
        tui::XoaManHinh();
        tui::VeKhung(2, 1, ChieuRong, ChieuCao, "THONG KE > TOP 10 SACH MUON NHIEU NHAT");
        int Y = 5;
        tui::DiChuyenConTro(4, Y++);
        std::cout << CanLeTrai("STT", DoRongCotSTT) << " | " << CanLeTrai("ISBN", DoRongCotISBN) << " | "
            << CanLeTrai("Ten sach", DoRongCotTEN) << " | " << CanLeTrai("So luot muon", DoRongCotCNT);
        tui::DiChuyenConTro(4, Y++);
        std::cout << TaoDuongKe(DoRongCotSTT) << "-+-" << TaoDuongKe(DoRongCotISBN) << "-+-" << TaoDuongKe(DoRongCotTEN) << "-+-"
            << TaoDuongKe(DoRongCotCNT);
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
            std::cout << CanLeTrai(std::to_string(i + 1) + ".", DoRongCotSTT) << " | " << CanLeTrai(DuLieuSach->ISBN, DoRongCotISBN)
                << " | " << CanLeTrai(TenCut, DoRongCotTEN) << " | " << CanLeTrai(std::to_string(DuLieuSach->SoLuotMuon), DoRongCotCNT);
        }
        tui::NhanPhimBatKyDeQuayLai(4, FooterY - 1);
    }
    //================= Thống Kê Quá Hạn ===================
    inline void ThongKeDanhSachQuaHanTUI(const DanhSachDauSach& DanhSachDauSach, DocGiaNode* Root){
        const int ChieuRong = 118;
        const int ChieuCao = 24;
        const int X0 = 4;
        const int Y0 = 3;
        const int FooterY = 1 + ChieuCao - 2;
        const int DoRongCotSTT = 4;
        const int DoRongCotMATHE = 9;
        const int DoRongCotMS = 13;
        const int DoRongCotTEN = 47;
        const int DoRongCotNGAY = 10;
        const int DoRongCotTRE = 5;
        tui::XoaManHinh();
        tui::VeKhung(2, 1, ChieuRong, ChieuCao, "THONG KE > DANH SACH MUON QUA HAN");
        int Y = Y0 + 1;
        tui::DiChuyenConTro(X0, Y);
        std::cout << "Nhap ngay hien tai (dd/mm/yyyy): ";
        int DateX = X0 + 34;
        int DateY = Y;
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
            NgayHienTai =  PhanTichNgayDDMMYYYY(ChuoiNhap);
            if (!KiemTraNgayHopLe(NgayHienTai)) {
                tui::DiChuyenConTro(X0, FooterY - 2);
                tui::DatMau(tui::MauCanhBao);
                std::cout << "Ngay khong hop le. Moi nhap lai!";
                tui::DatLaiMau();
                tui::DiChuyenConTro(DateX, DateY);
                std::cout << std::string(20, ' ');
                continue;
            }
            if (NgayHienTai.Nam < 1500 || NgayHienTai.Nam > NamHienTai){
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
        // Đếm trước để cấp phát đúng số phần tử cần dùng
        int TongSoLuong = DemSoPhieuQuaHan(Root, NgayHienTai);
        ThongKeQuaHan* DanhSachDong = NULL;
        if (TongSoLuong > 0) {
            // Chỉ cấp phát đúng bằng số phiếu quá hạn
            DanhSachDong = new ThongKeQuaHan[TongSoLuong];
            int SoLuongDaLap = 0;
            LapDanhSachQuaHan(Root, DanhSachDauSach, NgayHienTai, DanhSachDong, SoLuongDaLap, TongSoLuong);
            TongSoLuong = SoLuongDaLap;
        }
        const int PAGE = 14;
        int TongSoTrang =(TongSoLuong == 0 ? 1 : (TongSoLuong + PAGE - 1) / PAGE);
        int Trang = 0;
        const int HeaderY = Y0 + 3;
        const int TableY = HeaderY + 2;
        const int DataH = PAGE;
        auto PaintHeader = [&](int TrangHienTai) {
            tui::DiChuyenConTro(X0, Y0 + 1);
            std::cout
                << "Nhap ngay hien tai (dd/mm/yyyy): "
                << ChuyenNgayThanhChuoi(NgayHienTai)
                << "   (Trang "
                << TrangHienTai + 1
                << "/"
                << TongSoTrang
                << ")"
                << std::string(10, ' ');
            tui::DiChuyenConTro(X0, HeaderY);
            std::cout
                << CanLeTrai("STT", DoRongCotSTT)
                << " | "
                << CanLeTrai("MaThe", DoRongCotMATHE)
                << " | "
                << CanLeTrai("MaSach", DoRongCotMS)
                << " | "
                << CanLeTrai("Ten sach", DoRongCotTEN)
                << " | "
                << CanLeTrai("Ngay muon", DoRongCotNGAY)
                << " | "
                << CanLeTrai("Tre", DoRongCotTRE);
            tui::DiChuyenConTro(X0, HeaderY + 1);
            std::cout
                << TaoDuongKe(DoRongCotSTT)
                << "-+-"
                << TaoDuongKe(DoRongCotMATHE)
                << "-+-"
                << TaoDuongKe(DoRongCotMS)
                << "-+-"
                << TaoDuongKe(DoRongCotTEN)
                << "-+-"
                << TaoDuongKe(DoRongCotNGAY)
                << "-+-"
                << TaoDuongKe(DoRongCotTRE);
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
            for (int i = ChiSoDauTrang; i < ChiSoCuoiTrang; i++){
                const ThongKeQuaHan& KetQuaNhap = DanhSachDong[i];
                // Lấy tên sách trực tiếp qua địa chỉ đầu sách
                std::string TenSachThongKe = "";
                if (KetQuaNhap.DuLieuSach != NULL) {
                    TenSachThongKe = KetQuaNhap.DuLieuSach->TenSach;
                }
                std::string TenCut =((int)TenSachThongKe.size() > DoRongCotTEN) ? TenSachThongKe.substr(0, DoRongCotTEN): TenSachThongKe;
                tui::DiChuyenConTro(X0, YOut++);
                std::cout
                    << CanLeTrai(
                        std::to_string(i + 1),
                        DoRongCotSTT
                    )
                    << " | "
                    << CanLeTrai(
                        std::to_string(
                            KetQuaNhap
                            .DuLieuDocGia
                            ->MaThe
                        ),
                        DoRongCotMATHE
                    )
                    << " | "
                    << CanLeTrai(
                        KetQuaNhap
                        .PhieuMuon
                        ->MaSach,
                        DoRongCotMS
                    )
                    << " | "
                    << CanLeTrai(
                        TenCut,
                        DoRongCotTEN
                    )
                    << " | "
                    << CanLeTrai(
                        ChuyenNgayThanhChuoi(
                            KetQuaNhap
                            .PhieuMuon
                            ->NgayMuon
                        ),
                        DoRongCotNGAY
                    )
                    << " | "
                    << CanLeTrai(
                        std::to_string(
                            KetQuaNhap.SoNgayTre
                        ),
                        DoRongCotTRE
                    );
            }
            };
        PaintPage(Trang);
        while (true) {
            tui::KeyEvent SuKienPhimMoi = tui::DocPhim();
            if (SuKienPhimMoi.Phim == tui::PhimEsc){
                // Giải phóng đúng mảng đã cấp phát
                delete[] DanhSachDong;
                return;
            }
            if (SuKienPhimMoi.Phim == tui::PhimLen){
                if (Trang > 0) {
                    Trang--;
                    PaintPage(Trang);
                }
            }
            if (SuKienPhimMoi.Phim == tui::PhimXuong){
                if (Trang + 1 < TongSoTrang) {
                    Trang++;
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
    inline void MenuConDauSach(DanhSachDauSach& DanhSachDauSach) {
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
                FormXoaDauSachTUI(DanhSachDauSach);
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
                MenuConDauSach(DanhSachDauSach);
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
}