#pragma once
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
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
} // namespace tui

