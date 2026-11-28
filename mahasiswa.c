#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "data.h"

/* Cek apakah 'jarum' ada di dalam 'tumpukan' (tidak peduli huruf besar/kecil) */
static int mengandung(const char *tumpukan, const char *jarum) {
    int n = strlen(tumpukan), m = strlen(jarum);
    int i, j;
    if (m == 0) return 0;
    for (i = 0; i + m <= n; i++) {
        for (j = 0; j < m; j++) {
            if (tolower((unsigned char)tumpukan[i + j]) !=
                tolower((unsigned char)jarum[j])) break;
        }
        if (j == m) return 1;
    }
    return 0;
}

/* Mengembalikan indeks mahasiswa dengan NIM tertentu, atau -1 jika tidak ada */
int cariIndexMhs(const char nimCari[]) {
    int i;
    for (i = 0; i < jumlahMhs; i++) {
        if (strcmp(dataMhs[i].nim, nimCari) == 0) return i;
    }
    return -1;
}

/* Menampilkan profil satu mahasiswa berdasarkan indeks */
void profilMahasiswa(int idx) {
    if (idx < 0 || idx >= jumlahMhs) {
        printf("Indeks tidak valid.\n");
        return;
    }
    printf("\n+-------------------------------------------+\n");
    printf("|            PROFIL MAHASISWA               |\n");
    printf("+-------------------------------------------+\n");
    printf("  NIM         : %s\n", dataMhs[idx].nim);
    printf("  Nama        : %s\n", dataMhs[idx].nama);
    printf("  Program Studi: %s\n", dataMhs[idx].prodi);
    printf("  IPK         : %.2f\n", dataMhs[idx].ipk);
    printf("+-------------------------------------------+\n");
}

/* 1 + 10. Tambah data mahasiswa dengan validasi NIM unik */
void tambahMahasiswa(void) {
    char nim[15];
    Mahasiswa m;

    if (jumlahMhs >= MAX_MHS) {
        printf("Data penuh! Maksimal %d mahasiswa.\n", MAX_MHS);
        return;
    }

    bacaString("NIM           : ", nim, sizeof(nim));
    if (strlen(nim) == 0) {
        printf("NIM tidak boleh kosong.\n");
        return;
    }
    if (cariIndexMhs(nim) != -1) {
        printf("Gagal: NIM %s sudah terdaftar (duplikat).\n", nim);
        return;
    }
    strcpy(m.nim, nim);

    bacaString("Nama          : ", m.nama, sizeof(m.nama));
    if (strlen(m.nama) == 0) {
        printf("Nama tidak boleh kosong.\n");
        return;
    }
    bacaString("Program Studi : ", m.prodi, sizeof(m.prodi));

    do {
        m.ipk = bacaFloat("IPK (0.00-4.00): ");
        if (m.ipk < 0.0f || m.ipk > 4.0f)
            printf("IPK harus antara 0.00 sampai 4.00.\n");
    } while (m.ipk < 0.0f || m.ipk > 4.0f);

    dataMhs[jumlahMhs++] = m;
    printf("Data mahasiswa berhasil ditambahkan.\n");
}

/* 2. Tampil seluruh mahasiswa dalam tabel rapi */
void tampilMahasiswa(void) {
    int i;
    if (jumlahMhs == 0) {
        printf("Belum ada data mahasiswa.\n");
        return;
    }
    printf("\n");
    printf("+-----+--------------+--------------------------------+----------------------+------+\n");
    printf("| No  | NIM          | Nama                           | Program Studi        | IPK  |\n");
    printf("+-----+--------------+--------------------------------+----------------------+------+\n");
    for (i = 0; i < jumlahMhs; i++) {
        printf("| %-3d | %-12s | %-30s | %-20s | %.2f |\n",
               i + 1, dataMhs[i].nim, dataMhs[i].nama,
               dataMhs[i].prodi, dataMhs[i].ipk);
    }
    printf("+-----+--------------+--------------------------------+----------------------+------+\n");
    printf("Total: %d mahasiswa\n", jumlahMhs);
}

/* 3. Cari mahasiswa berdasarkan NIM atau nama */
void cariMahasiswa(void) {
    int pilih, i, ketemu = 0;
    char kunci[50];

    printf("Cari berdasarkan:\n 1. NIM\n 2. Nama\n");
    pilih = bacaInt("Pilihan: ");

    if (pilih == 1) {
        bacaString("Masukkan NIM: ", kunci, sizeof(kunci));
        i = cariIndexMhs(kunci);
        if (i == -1) printf("Mahasiswa dengan NIM %s tidak ditemukan.\n", kunci);
        else profilMahasiswa(i);
    } else if (pilih == 2) {
        bacaString("Masukkan nama (boleh sebagian): ", kunci, sizeof(kunci));
        for (i = 0; i < jumlahMhs; i++) {
            if (mengandung(dataMhs[i].nama, kunci)) {
                profilMahasiswa(i);
                ketemu++;
            }
        }
        if (!ketemu) printf("Tidak ada mahasiswa dengan nama \"%s\".\n", kunci);
        else printf("Ditemukan %d data.\n", ketemu);
    } else {
        printf("Pilihan tidak valid.\n");
    }
}

/* 4. Ubah data mahasiswa (kosongkan input = tidak diubah) */
void ubahMahasiswa(void) {
    char nim[15], buf[50];
    int idx;
    float ipkBaru;

    bacaString("Masukkan NIM yang akan diubah: ", nim, sizeof(nim));
    idx = cariIndexMhs(nim);
    if (idx == -1) {
        printf("NIM %s tidak ditemukan.\n", nim);
        return;
    }

    printf("Data saat ini:");
    profilMahasiswa(idx);
    printf("(Kosongkan input lalu Enter jika tidak ingin mengubah. NIM tidak dapat diubah.)\n");

    bacaString("Nama baru          : ", buf, sizeof(dataMhs[idx].nama));
    if (strlen(buf) > 0) strcpy(dataMhs[idx].nama, buf);

    bacaString("Program Studi baru : ", buf, sizeof(dataMhs[idx].prodi));
    if (strlen(buf) > 0) strcpy(dataMhs[idx].prodi, buf);

    bacaString("IPK baru (0-4)     : ", buf, sizeof(buf));
    if (strlen(buf) > 0) {
        if (sscanf(buf, "%f", &ipkBaru) == 1 && ipkBaru >= 0.0f && ipkBaru <= 4.0f)
            dataMhs[idx].ipk = ipkBaru;
        else
            printf("IPK tidak valid, IPK tidak diubah.\n");
    }
    printf("Data berhasil diperbarui.\n");
}

/* 5. Hapus mahasiswa + geser indeks array */
void hapusMahasiswa(void) {
    char nim[15], yakin[10];
    int idx, i;

    bacaString("Masukkan NIM yang akan dihapus: ", nim, sizeof(nim));
    idx = cariIndexMhs(nim);
    if (idx == -1) {
        printf("NIM %s tidak ditemukan.\n", nim);
        return;
    }

    profilMahasiswa(idx);
    bacaString("Yakin hapus? (y/n): ", yakin, sizeof(yakin));
    if (yakin[0] != 'y' && yakin[0] != 'Y') {
        printf("Penghapusan dibatalkan.\n");
        return;
    }

    /* geser elemen setelah idx satu posisi ke kiri */
    for (i = idx; i < jumlahMhs - 1; i++) {
        dataMhs[i] = dataMhs[i + 1];
    }
    jumlahMhs--;
    printf("Data mahasiswa berhasil dihapus.\n");
}
