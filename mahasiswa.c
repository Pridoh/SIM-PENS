/*
 * mahasiswa.c - Pengelolaan data mahasiswa (Anggota 2)
 * Memakai array paralel dari data.h: nim, nama, jurusan, angkatan, ipMhs.
 * Indeks yang sama di semua array = mahasiswa yang sama.
 */
#include <stdio.h>
#include <string.h>
#include "data.h"

/* Cek apakah 'jarum' ada di dalam 'tumpukan' (tidak peduli huruf besar/kecil) */
static int mengandung(const char tumpukan[], const char jarum[]) {
    char a[64], b[64];
    keHurufKecil(tumpukan, a);
    keHurufKecil(jarum, b);
    return strstr(a, b) != NULL;
}

/* Mengembalikan indeks mahasiswa dengan NIM tertentu, atau -1 jika tidak ada */
int cariIndexMhs(const char nimCari[]) {
    int i;
    for (i = 0; i < jumlahMahasiswa; i++) {
        if (strcmp(nim[i], nimCari) == 0) return i;
    }
    return -1;
}

/* Menampilkan profil satu mahasiswa berdasarkan indeks */
void profilMahasiswa(int idx) {
    if (idx < 0 || idx >= jumlahMahasiswa) {
        printf("Indeks tidak valid.\n");
        return;
    }
    printf("\n+-------------------------------------------+\n");
    printf("|            PROFIL MAHASISWA               |\n");
    printf("+-------------------------------------------+\n");
    printf("  NIM      : %s\n", nim[idx]);
    printf("  Nama     : %s\n", nama[idx]);
    printf("  Jurusan  : %s\n", jurusan[idx]);
    printf("  Angkatan : %d\n", angkatan[idx]);
    printf("  IP       : %.2f\n", ipMhs[idx]);
    printf("+-------------------------------------------+\n");
}

/* 1 + 10. Tambah data mahasiswa dengan validasi NIM unik */
void tambahMahasiswa(void) {
    char nimBaru[20], namaBaru[50], jurusanBaru[50];
    int angkatanBaru;

    if (jumlahMahasiswa >= MAX_MHS) {
        printf("Data penuh! Maksimal %d mahasiswa.\n", MAX_MHS);
        return;
    }

    bacaStringWajib("NIM      : ", nimBaru, sizeof(nimBaru));
    if (cariIndexMhs(nimBaru) != -1) {
        printf("Gagal: NIM %s sudah terdaftar (duplikat).\n", nimBaru);
        return;
    }
    bacaStringWajib("Nama     : ", namaBaru, sizeof(namaBaru));
    bacaStringWajib("Jurusan  : ", jurusanBaru, sizeof(jurusanBaru));
    angkatanBaru = bacaInt("Angkatan (2000-2100) : ", 2000, 2100);

    /* simpan ke posisi jumlahMahasiswa di setiap array */
    strcpy(nim[jumlahMahasiswa], nimBaru);
    strcpy(nama[jumlahMahasiswa], namaBaru);
    strcpy(jurusan[jumlahMahasiswa], jurusanBaru);
    angkatan[jumlahMahasiswa] = angkatanBaru;
    ipMhs[jumlahMahasiswa] = 0.0f;       /* diisi nanti oleh hitungIP() */
    jumlahMahasiswa++;

    printf("Data mahasiswa berhasil ditambahkan.\n");
}

/* 2. Tampil seluruh mahasiswa dalam tabel rapi */
void tampilMahasiswa(void) {
    int i;
    if (jumlahMahasiswa == 0) {
        printf("Belum ada data mahasiswa.\n");
        return;
    }
    printf("\n");
    printf("+-----+-----------------+------------------------------+--------------------------+----------+------+\n");
    printf("| %-3s | %-15s | %-28s | %-24s | %-8s | %-4s |\n",
           "No", "NIM", "Nama", "Jurusan", "Angkatan", "IP");
    printf("+-----+-----------------+------------------------------+--------------------------+----------+------+\n");
    for (i = 0; i < jumlahMahasiswa; i++) {
        printf("| %-3d | %-15.15s | %-28.28s | %-24.24s | %-8d | %.2f |\n",
               i + 1, nim[i], nama[i], jurusan[i], angkatan[i], ipMhs[i]);
    }
    printf("+-----+-----------------+------------------------------+--------------------------+----------+------+\n");
    printf("Total: %d mahasiswa\n", jumlahMahasiswa);
}

/* 3. Cari mahasiswa berdasarkan NIM atau nama */
void cariMahasiswa(void) {
    int pilih, i, ketemu = 0;
    char kunci[50];

    printf("Cari berdasarkan:\n 1. NIM\n 2. Nama\n");
    pilih = bacaInt("Pilihan : ", 1, 2);

    if (pilih == 1) {
        bacaStringWajib("Masukkan NIM : ", kunci, sizeof(kunci));
        i = cariIndexMhs(kunci);
        if (i == -1) printf("Mahasiswa dengan NIM %s tidak ditemukan.\n", kunci);
        else profilMahasiswa(i);
    } else {
        bacaStringWajib("Masukkan nama (boleh sebagian) : ", kunci, sizeof(kunci));
        for (i = 0; i < jumlahMahasiswa; i++) {
            if (mengandung(nama[i], kunci)) {
                profilMahasiswa(i);
                ketemu++;
            }
        }
        if (!ketemu) printf("Tidak ada mahasiswa dengan nama \"%s\".\n", kunci);
        else printf("Ditemukan %d data.\n", ketemu);
    }
}

/* 4. Ubah data mahasiswa (Enter = tidak diubah, NIM tidak bisa diubah) */
void ubahMahasiswa(void) {
    char nimCari[20], buf[50];
    int idx;

    bacaStringWajib("Masukkan NIM yang akan diubah : ", nimCari, sizeof(nimCari));
    idx = cariIndexMhs(nimCari);
    if (idx == -1) {
        printf("NIM %s tidak ditemukan.\n", nimCari);
        return;
    }

    printf("Data saat ini:");
    profilMahasiswa(idx);
    printf("(Tekan Enter jika tidak ingin mengubah. NIM tidak dapat diubah.)\n");

    bacaString("Nama baru     : ", buf, sizeof(buf));
    if (strlen(buf) > 0) strcpy(nama[idx], buf);

    bacaString("Jurusan baru  : ", buf, sizeof(buf));
    if (strlen(buf) > 0) strcpy(jurusan[idx], buf);

    angkatan[idx] = bacaIntEdit("Angkatan baru : ", angkatan[idx], 2000, 2100);

    printf("Data berhasil diperbarui.\n");
}

/* 5. Hapus mahasiswa + geser indeks semua array */
void hapusMahasiswa(void) {
    char nimCari[20];
    int idx, i;

    bacaStringWajib("Masukkan NIM yang akan dihapus : ", nimCari, sizeof(nimCari));
    idx = cariIndexMhs(nimCari);
    if (idx == -1) {
        printf("NIM %s tidak ditemukan.\n", nimCari);
        return;
    }

    profilMahasiswa(idx);
    if (!konfirmasi("Yakin hapus data ini (y/n)? ")) {
        printf("Penghapusan dibatalkan.\n");
        return;
    }

    /* geser SEMUA array paralel satu posisi ke kiri mulai dari idx */
    for (i = idx; i < jumlahMahasiswa - 1; i++) {
        strcpy(nim[i],     nim[i + 1]);
        strcpy(nama[i],    nama[i + 1]);
        strcpy(jurusan[i], jurusan[i + 1]);
        angkatan[i] = angkatan[i + 1];
        ipMhs[i]    = ipMhs[i + 1];
    }
    jumlahMahasiswa--;
    printf("Data mahasiswa berhasil dihapus.\n");
}
