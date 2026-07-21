#pragma once
#include "cautruc.h"
#include "dsdocgia.h"

// =================== CÁC HÀM THỐNG KÊ ===================
// Đếm thống kê tổng số bản sao (node) trong DMS của một đầu sách.
inline int DemTongSoBanSao(const DauSach* DuLieuSach) {
    if (DuLieuSach == NULL) {
        return 0;
    }
    int Dem = 0;
    for (const DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead; ConTroHienTai != NULL;
        ConTroHienTai = ConTroHienTai->Next) {
        Dem++;
    }
    return Dem;
}
// Đếm thống kê  bản sao đang được mượn.
inline int DemSoSachDangMuon(const DauSach* DuLieuSach) {
    if (DuLieuSach == NULL) {
        return 0;
    }
    int Dem = 0;
    for (const DanhMucSachNode* ConTroHienTai = DuLieuSach->DanhMucSachHead; ConTroHienTai != NULL;
        ConTroHienTai = ConTroHienTai->Next) {
        if (ConTroHienTai->TrangThai == 1) {
            Dem++;
        }
    }
    return Dem;
}

// =================== THỐNG KÊ TOP 10 SÁCH MƯỢN NHIỀU NHẤT ===================
inline void ThongKeTop10TheoLuotMuon(const DanhSachDauSach& DanhSachDauSach,DauSach* DanhSachKetQua[],int& SoLuongKetQua){
    DauSach* DanhSachTam[MaxDauSach];
    int SoLuongTam = 0;
    // Lấy toàn bộ đầu sách đã từng được mượn
    for (int i = 0; i < DanhSachDauSach.SoLuong; i++){
        DauSach* DuLieuSach = DanhSachDauSach.Nodes[i];
        if (DuLieuSach != NULL && DuLieuSach->SoLuotMuon > 0){
            DanhSachTam[SoLuongTam] = DuLieuSach;
            SoLuongTam++;
        }
    }

    // Sắp xếp giảm dần theo số lượt mượn
    for (int i = 0; i < SoLuongTam - 1; i++){
        for (int j = i + 1; j < SoLuongTam; j++){
            bool CanHoanDoi = false;
            if (DanhSachTam[j]->SoLuotMuon > DanhSachTam[i]->SoLuotMuon){
                CanHoanDoi = true;
            }
            else if (DanhSachTam[j]->SoLuotMuon == DanhSachTam[i]->SoLuotMuon){
                if (std::strcmp(DanhSachTam[j]->TenSach,DanhSachTam[i]->TenSach) < 0){
                    CanHoanDoi = true;
                }
            }
            if (CanHoanDoi){
                DauSach* DuLieuTam = DanhSachTam[i];
                DanhSachTam[i] = DanhSachTam[j];
                DanhSachTam[j] = DuLieuTam;
            }
        }
    }
    // Chỉ lấy tối đa 10 đầu sách
    SoLuongKetQua = SoLuongTam;
    if (SoLuongKetQua > 10){
        SoLuongKetQua = 10;
    }
    // Sao chép kết quả sang mảng Top 10
    for (int i = 0; i < SoLuongKetQua; i++){
        DanhSachKetQua[i] = DanhSachTam[i];
    }
}

// =================== THỐNG KÊ ĐỘC GIẢ QUÁ HẠN ===================
// Hàm phụ: Tìm đầu sách trong mảng tĩnh (const)
inline const DauSach* TimDauSachChoThongKe(const DanhSachDauSach& DanhSachDauSach, const std::string& ISBNCanXuLy) {
    // Vòng lặp duyệt từ đầu (0) đến cuối danh sách (n)
    for (int i = 0; i < DanhSachDauSach.SoLuong; i++) {
        // So sánh ISBN của sách tại vị trí i với ISBN cần tìm
        if (DanhSachDauSach.Nodes[i]->ISBN == ISBNCanXuLy) {
            // Nếu trùng khớp -> Trả về con trỏ sách tại vị trí đó ngay lập tức
            return DanhSachDauSach.Nodes[i];
        }
    }
    // Nếu chạy hết vòng lặp mà không return (tức là không tìm thấy)
    return NULL;
}
// Hàm phụ: Duyệt cây (DFS) để thu thập dữ liệu vào mảng outRows
inline void DuyetCayThongKeQuaHan(DocGiaNode* Root,
    const DanhSachDauSach& DanhSachDauSach,
    const NgayThangNam& NgayHienTai,
    ThongKeQuaHan DanhSachQuaHan[],
    int& SoLuongTimThay,
    int SoDongToiDa) {
    if (Root == NULL || SoLuongTimThay >= SoDongToiDa) {
        return;
    }
    // Duyệt trái (L)
    DuyetCayThongKeQuaHan(Root->Left, DanhSachDauSach, NgayHienTai, DanhSachQuaHan, SoLuongTimThay, SoDongToiDa);
    // Xử lý nút hiện tại (N)
    DocGia* DocGiaCanXuLy = &Root->ThongTin;
    for (MuonTraNode* ConTroHienTai = DocGiaCanXuLy->MuonTraHead; ConTroHienTai != NULL;
        ConTroHienTai = ConTroHienTai->Next) {
        // Chỉ xét sách ĐANG MƯỢN
        if (ConTroHienTai->TrangThai != 0) {
            continue;
        }
        int TongSoNgay = TinhSoNgayChenhLech(NgayHienTai, ConTroHienTai->NgayMuon);
        int Tre = TongSoNgay - HanMuonNgay;
        // Nếu quá hạn
        if (Tre > 0) {
            if (SoLuongTimThay >= SoDongToiDa) {
                return;
            } // Đầy mảng thì dừng
            std::string ISBNCanXuLy = LayISBNTuMaSach(ConTroHienTai->MaSach);
            const DauSach* DuLieuSach = TimDauSachChoThongKe(DanhSachDauSach, ISBNCanXuLy);
            // Điền dữ liệu vào mảng tại vị trí count
            DanhSachQuaHan[SoLuongTimThay].MaThe = DocGiaCanXuLy->MaThe;
            SaoChepChuoi(DanhSachQuaHan[SoLuongTimThay].HoTen,
                100,
                CatKhoangTrangHaiDau(std::string(DocGiaCanXuLy->Ho) + " " + DocGiaCanXuLy->Ten));
            SaoChepChuoi(DanhSachQuaHan[SoLuongTimThay].MaSach, MaxMaSach, ConTroHienTai->MaSach);
            SaoChepChuoi(DanhSachQuaHan[SoLuongTimThay].ISBN, 15, ISBNCanXuLy);
            SaoChepChuoi(DanhSachQuaHan[SoLuongTimThay].TenSach,
                100,
                DuLieuSach != NULL ? std::string(DuLieuSach->TenSach) : std::string(""));
            DanhSachQuaHan[SoLuongTimThay].NgayMuon = ConTroHienTai->NgayMuon;
            DanhSachQuaHan[SoLuongTimThay].SoNgayTre = Tre;
            SoLuongTimThay++; // Tăng biến đếm
        }
    }
    // Duyệt phải (R)
    DuyetCayThongKeQuaHan(Root->Right, DanhSachDauSach, NgayHienTai, DanhSachQuaHan, SoLuongTimThay, SoDongToiDa);
}
// Hàm chính: Thống kê quá hạn
inline void LapDanhSachQuaHan(DocGiaNode* Root,
    const DanhSachDauSach& DanhSachDauSach,
    const NgayThangNam& NgayHienTai,
    ThongKeQuaHan DanhSachQuaHan[],
    int& SoLuongKetQua,
    int SoDongToiDa = 500) {
    SoLuongKetQua = 0;
    // 1. Thu thập dữ liệu
    DuyetCayThongKeQuaHan(Root, DanhSachDauSach, NgayHienTai, DanhSachQuaHan, SoLuongKetQua, SoDongToiDa);
    // 2. Sắp xếp thủ công (Bubble Sort)
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
