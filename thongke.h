#pragma once
#include "cautruc.h"
#include "dausach.h"
#include "docgia.h"

// ======================= DỮ LIỆU THỐNG KÊ =======================
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

// ======================= TOP 10 SÁCH MƯỢN NHIỀU NHẤT =======================
inline void ThongKeTop10TheoLuotMuon(const DanhSachDauSach& DanhSachDauSach, DauSach* DanhSachKetQua[], int& SoLuongKetQua) {
    DauSach* DanhSachTam[MaxDauSach];
    int SoLuongTam = 0;
    for (int i = 0; i < DanhSachDauSach.SoLuong; i++) {
        DauSach* DuLieuSach = DanhSachDauSach.Nodes[i];
        if (DuLieuSach != NULL && DuLieuSach->SoLuotMuon > 0) {
            DanhSachTam[SoLuongTam] = DuLieuSach;
            SoLuongTam++;
        }
    }
    for (int i = 0; i < SoLuongTam - 1; i++) {
        for (int j = i + 1; j < SoLuongTam; j++) {
            bool CanHoanDoi = false;
            if (DanhSachTam[j]->SoLuotMuon > DanhSachTam[i]->SoLuotMuon) {
                CanHoanDoi = true;
            }
            else if (DanhSachTam[j]->SoLuotMuon == DanhSachTam[i]->SoLuotMuon) {
                if (std::strcmp(DanhSachTam[j]->TenSach, DanhSachTam[i]->TenSach) < 0) {
                    CanHoanDoi = true;
                }
            }
            if (CanHoanDoi) {
                DauSach* DuLieuTam = DanhSachTam[i];
                DanhSachTam[i] = DanhSachTam[j];
                DanhSachTam[j] = DuLieuTam;
            }
        }
    }
    SoLuongKetQua = SoLuongTam;
    if (SoLuongKetQua > 10) {
        SoLuongKetQua = 10;
    }
    for (int i = 0; i < SoLuongKetQua; i++) {
        DanhSachKetQua[i] = DanhSachTam[i];
    }
}

// =================== THỐNG KÊ ĐỘC GIẢ QUÁ HẠN ===================
// Duyệt cây độc giả và thu thập các phiếu mượn quá hạn
inline void DuyetCayThongKeQuaHan(DocGiaNode* Root,
    const DanhSachDauSach& DanhSachDauSach,
    const NgayThangNam& NgayHienTai,
    ThongKeQuaHan DanhSachQuaHan[],
    int& SoLuongTimThay,
    int SoDongToiDa) {
    if (Root == NULL || SoLuongTimThay >= SoDongToiDa) {
        return;
    }
    DuyetCayThongKeQuaHan(Root->Left, DanhSachDauSach, NgayHienTai, DanhSachQuaHan, SoLuongTimThay, SoDongToiDa);
    DocGia* DocGiaCanXuLy = &Root->ThongTin;
    for (MuonTraNode* ConTroHienTai = DocGiaCanXuLy->MuonTraHead; ConTroHienTai != NULL;
        ConTroHienTai = ConTroHienTai->Next) {
        if (ConTroHienTai->TrangThai != 0) {
            continue;
        }
        int Tre = TinhSoNgayTre(ConTroHienTai->NgayMuon, NgayHienTai);
        if (Tre > 0) {
            if (SoLuongTimThay >= SoDongToiDa) {
                return;
            } // Đầy mảng thì dừng
            char ISBNCanXuLy[15];
            LayISBNTuMaSach(ConTroHienTai->MaSach, ISBNCanXuLy, 15);
            const DauSach* DuLieuSach = TimDauSachTheoISBN(DanhSachDauSach, ISBNCanXuLy);
            DanhSachQuaHan[SoLuongTimThay].MaThe = DocGiaCanXuLy->MaThe;
            std::snprintf(DanhSachQuaHan[SoLuongTimThay].HoTen, 100, "%s %s", DocGiaCanXuLy->Ho, DocGiaCanXuLy->Ten);
            SaoChepChuoi(DanhSachQuaHan[SoLuongTimThay].MaSach, MaxMaSach, ConTroHienTai->MaSach);
            SaoChepChuoi(DanhSachQuaHan[SoLuongTimThay].ISBN, 15, ISBNCanXuLy);
            if (DuLieuSach != NULL) {
                SaoChepChuoi(DanhSachQuaHan[SoLuongTimThay].TenSach, 100, DuLieuSach->TenSach);
            }
            else {
                DanhSachQuaHan[SoLuongTimThay].TenSach[0] = '\0';
            }
            DanhSachQuaHan[SoLuongTimThay].NgayMuon = ConTroHienTai->NgayMuon;
            DanhSachQuaHan[SoLuongTimThay].SoNgayTre = Tre;
            SoLuongTimThay++; // Tăng biến đếm
        }
    }
    DuyetCayThongKeQuaHan(Root->Right, DanhSachDauSach, NgayHienTai, DanhSachQuaHan, SoLuongTimThay, SoDongToiDa);
}
// Lập và sắp xếp danh sách độc giả đang mượn sách quá hạn
inline void LapDanhSachQuaHan(DocGiaNode* Root,
    const DanhSachDauSach& DanhSachDauSach,
    const NgayThangNam& NgayHienTai,
    ThongKeQuaHan DanhSachQuaHan[],
    int& SoLuongKetQua,
    int SoDongToiDa = 500) {
    SoLuongKetQua = 0;
    DuyetCayThongKeQuaHan(Root, DanhSachDauSach, NgayHienTai, DanhSachQuaHan, SoLuongKetQua, SoDongToiDa);
    for (int i = 0; i < SoLuongKetQua - 1; i++) {
        for (int j = i + 1; j < SoLuongKetQua; j++) {
            bool Swap = false;
            if (DanhSachQuaHan[j].SoNgayTre > DanhSachQuaHan[i].SoNgayTre) {
                Swap = true;
            }
            else if (DanhSachQuaHan[j].SoNgayTre == DanhSachQuaHan[i].SoNgayTre) {
                if (DanhSachQuaHan[j].MaThe < DanhSachQuaHan[i].MaThe) {
                    Swap = true;
                }
                else if (DanhSachQuaHan[j].MaThe == DanhSachQuaHan[i].MaThe) {
                    if (std::strcmp(DanhSachQuaHan[j].MaSach, DanhSachQuaHan[i].MaSach) < 0) {
                        Swap = true;
                    }
                }
            }
            if (Swap) {
                ThongKeQuaHan DuLieuTam = DanhSachQuaHan[i];
                DanhSachQuaHan[i] = DanhSachQuaHan[j];
                DanhSachQuaHan[j] = DuLieuTam;
            }
        }
    }
}