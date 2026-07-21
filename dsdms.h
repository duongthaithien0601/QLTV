#pragma once
#include <string>
#include "cautruc.h"
#include "thongke.h"

// =================== TIỆN ÍCH DSLK ===================
// Tìm bản sao có TrangThai bằng 0 đầu tiên.
inline DanhMucSachNode* TimSachCoTheMuonDauTien(DauSach* DuLieuSach) {
    if (DuLieuSach == NULL) {
        return NULL;
    }
    for (DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead; ConTroHienTai != NULL;
        ConTroHienTai = ConTroHienTai->Next) {
        if (ConTroHienTai->TrangThai == 0) {
            return ConTroHienTai;
        }
    }
    return NULL;
}
// Tìm bản sao theo mã sách.
inline DanhMucSachNode* TimSachTheoMaSach(DauSach* DuLieuSach, const std::string& MaSach) {
    if (DuLieuSach == NULL) {
        return NULL;
    }
    for (DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead; ConTroHienTai != NULL;
        ConTroHienTai = ConTroHienTai->Next) {
        if (ConTroHienTai->MaSach == MaSach) {
            return ConTroHienTai;
        }
    }
    return NULL;
}

// =================== THÊM / GỠ BẢN SAO ===================
// Thêm một Node vào cuối DSLK, tự tăng SoLuongBanSao.
inline void ThemSachVaoCuoiDanhMuc(DauSach* DuLieuSach, DanhMucSachNode* NodeCanXuLy) {
    if (DuLieuSach == NULL || NodeCanXuLy == NULL) {
        return;
    }
    NodeCanXuLy->Next = NULL;
    if (DuLieuSach->DanhMucSachHead == NULL) {
        DuLieuSach->DanhMucSachHead = NodeCanXuLy;
    }
    else {
        DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead;
        while (ConTroHienTai->Next != NULL) {
            ConTroHienTai = ConTroHienTai->Next;
        }
        ConTroHienTai->Next = NodeCanXuLy;
    }
    DuLieuSach->SoLuongBanSao++;
}
// Tách (gỡ) một node khỏi DSLK.
inline bool TachSachKhoiDanhMuc(DauSach* DuLieuSach, DanhMucSachNode* DoiTuongCanXuLy) {
    if (DuLieuSach == NULL || DoiTuongCanXuLy == NULL) {
        return false;
    }
    DanhMucSachNode* ChiSoTruoc = NULL;
    DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead;
    while (ConTroHienTai != NULL) {
        if (ConTroHienTai == DoiTuongCanXuLy) {
            if (ChiSoTruoc == NULL) {
                DuLieuSach->DanhMucSachHead = ConTroHienTai->Next;
            }
            else {
                ChiSoTruoc->Next = ConTroHienTai->Next;
            }
            DuLieuSach->SoLuongBanSao--;
            return true;
        }
        ChiSoTruoc = ConTroHienTai;
        ConTroHienTai = ConTroHienTai->Next;
    }
    return false;
}
// Giải phóng toàn bộ DSLK.
inline void GiaiPhongDanhMucSach(DanhMucSachNode*& Head) {
    DanhMucSachNode* ConTroHienTai = Head;
    while (ConTroHienTai != NULL) {
        DanhMucSachNode* Nxt = ConTroHienTai->Next;
        delete ConTroHienTai;
        ConTroHienTai = Nxt;
    }
    Head = NULL;
}
// Lấy danh sách mã sách (phục vụ lưu trữ/in bảng).
inline void LayDanhSachMaSach(const DauSach* DuLieuSach,
    std::string DanhSachKetQua[],
    int& SoLuongKetQua,
    int SoPhanTuToiDa = 1000) {
    SoLuongKetQua = 0;
    if (DuLieuSach == NULL) {
        return;
    }
    for (const DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead; ConTroHienTai != NULL;
        ConTroHienTai = ConTroHienTai->Next) {
        if (SoLuongKetQua < SoPhanTuToiDa) {
            DanhSachKetQua[SoLuongKetQua++] = ConTroHienTai->MaSach;
        }
    }
}

// =================== ĐÁNH DẤU MƯỢN / TRẢ ===================
// Đánh dấu mượn sách.
inline bool DanhDauSachDaMuon(DanhMucSachNode* NodeCanXuLy) {
    if (NodeCanXuLy == NULL) {
        return false;
    }
    if (NodeCanXuLy->TrangThai != 0) {
        return false;
    }
    NodeCanXuLy->TrangThai = 1;
    return true;
}
//  Đánh dấu trả sách.
inline bool DanhDauSachDaTra(DanhMucSachNode* NodeCanXuLy) {
    if (NodeCanXuLy == NULL) {
        return false;
    }
    if (NodeCanXuLy->TrangThai != 1) {
        return false;
    }
    NodeCanXuLy->TrangThai = 0;
    return true;
}

// =================== KHÁC ===================
// Đồng bộ lại SoLuongBanSao theo DSLK 
inline void CapNhatSoLuongBanSao(DauSach* DuLieuSach) {
    if (DuLieuSach == NULL) {
        return;
    }
    DuLieuSach->SoLuongBanSao = DemTongSoBanSao(DuLieuSach);
}
// Đặt tất cả bản sao về trạng thái có thể cho mượn
// trước khi khôi phục từ danh sách mượn trả
inline void DatLaiTrangThaiTatCaBanSao(DanhSachDauSach& DanhSachDauSach){
    for (int i = 0;i < DanhSachDauSach.SoLuong;i++){
        DauSach* DuLieuSach = DanhSachDauSach.Nodes[i];
        if (DuLieuSach == NULL){
            continue;
        }
        for (DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead;ConTroHienTai != NULL;ConTroHienTai = ConTroHienTai->Next){
            ConTroHienTai->TrangThai = 0;
        }
    }
}