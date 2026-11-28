/*
 * matakuliah.c - Pengelolaan data mata kuliah (Anggota 2)
 * Memakai array paralel dari data.h: kodeMatkul, namaMatkul, sks.
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

/* Mengembalikan indeks mata kuliah dengan kode tertentu, atau -1 jika tidak ada */
int cariIndexMatkul(const char kode[]) {
    int i;
    for (i = 0; i < jumlahMatkul; i++) {
        if (strcmp(kodeMatkul[i], kode) == 0) return i;
    }
    return -1;
}

/* 6 + 10. Tambah mata kuliah dengan validasi kode unik */
void tambahMatkul(void) {
    char kodeBaru[15], namaBaru[50];
    int sksBaru;

    if (jumlahMatkul >= MAX_MATKUL) {
        printf("Data penuh! Maksimal %d mata kuliah.\n", MAX_MATKUL);
        return;
    }

    bacaStringWajib("Kode Mata Kuliah : ", kodeBaru, sizeof(kodeBaru));
    if (cariIndexMatkul(kodeBaru) != -1) {
        printf("Gagal: Kode %s sudah terdaftar (duplikat).\n", kodeBaru);
        return;
    }
    bacaStringWajib("Nama Mata Kuliah : ", namaBaru, sizeof(namaBaru));
    sksBaru = bacaInt("Jumlah SKS (1-6)  : ", 1, 6);

    strcpy(kodeMatkul[jumlahMatkul], kodeBaru);
    strcpy(namaMatkul[jumlahMatkul], namaBaru);
    sks[jumlahMatkul] = sksBaru;
    jumlahMatkul++;

    printf("Mata kuliah berhasil ditambahkan.\n");
}

/* 7. Tampil seluruh mata kuliah */
void tampilMatkul(void) {
    int i, totalSks = 0;
    if (jumlahMatkul == 0) {
        printf("Belum ada data mata kuliah.\n");
        return;
    }
    printf("\n");
    printf("+-----+------------+--------------------------------+-----+\n");
    printf("| %-3s | %-10s | %-30s | %-3s |\n", "No", "Kode", "Nama Mata Kuliah", "SKS");
    printf("+-----+------------+--------------------------------+-----+\n");
    for (i = 0; i < jumlahMatkul; i++) {
        printf("| %-3d | %-10.10s | %-30.30s | %-3d |\n",
               i + 1, kodeMatkul[i], namaMatkul[i], sks[i]);
        totalSks += sks[i];
    }
    printf("+-----+------------+--------------------------------+-----+\n");
    printf("Total: %d mata kuliah, %d SKS\n", jumlahMatkul, totalSks);
}

/* Cari mata kuliah berdasarkan kode atau nama */
void cariMatkul(void) {
    int pilih, i, ketemu = 0;
    char kunci[50];

    printf("Cari berdasarkan:\n 1. Kode\n 2. Nama\n");
    pilih = bacaInt("Pilihan : ", 1, 2);

    if (pilih == 1) {
        bacaStringWajib("Masukkan kode : ", kunci, sizeof(kunci));
        i = cariIndexMatkul(kunci);
        if (i == -1) {
            printf("Mata kuliah dengan kode %s tidak ditemukan.\n", kunci);
        } else {
            printf("Ditemukan: %s - %s (%d SKS)\n", kodeMatkul[i], namaMatkul[i], sks[i]);
        }
    } else {
        bacaStringWajib("Masukkan nama (boleh sebagian) : ", kunci, sizeof(kunci));
        for (i = 0; i < jumlahMatkul; i++) {
            if (mengandung(namaMatkul[i], kunci)) {
                printf("%s - %s (%d SKS)\n", kodeMatkul[i], namaMatkul[i], sks[i]);
                ketemu++;
            }
        }
        if (!ketemu) printf("Tidak ada mata kuliah dengan nama \"%s\".\n", kunci);
        else printf("Ditemukan %d data.\n", ketemu);
    }
}

/* 8. Ubah data mata kuliah (Enter = tidak diubah, kode tidak bisa diubah) */
void ubahMatkul(void) {
    char kodeCari[15], buf[50];
    int idx;

    bacaStringWajib("Masukkan kode yang akan diubah : ", kodeCari, sizeof(kodeCari));
    idx = cariIndexMatkul(kodeCari);
    if (idx == -1) {
        printf("Kode %s tidak ditemukan.\n", kodeCari);
        return;
    }

    printf("Data saat ini: %s - %s (%d SKS)\n", kodeMatkul[idx], namaMatkul[idx], sks[idx]);
    printf("(Tekan Enter jika tidak ingin mengubah. Kode tidak dapat diubah.)\n");

    bacaString("Nama baru : ", buf, sizeof(buf));
    if (strlen(buf) > 0) strcpy(namaMatkul[idx], buf);

    sks[idx] = bacaIntEdit("SKS baru  : ", sks[idx], 1, 6);

    printf("Data mata kuliah berhasil diperbarui.\n");
}

/* 9. Hapus mata kuliah + geser indeks semua array */
void hapusMatkul(void) {
    char kodeCari[15];
    int idx, i;

    bacaStringWajib("Masukkan kode yang akan dihapus : ", kodeCari, sizeof(kodeCari));
    idx = cariIndexMatkul(kodeCari);
    if (idx == -1) {
        printf("Kode %s tidak ditemukan.\n", kodeCari);
        return;
    }

    printf("Akan menghapus: %s - %s (%d SKS)\n", kodeMatkul[idx], namaMatkul[idx], sks[idx]);
    if (!konfirmasi("Yakin hapus data ini (y/n)? ")) {
        printf("Penghapusan dibatalkan.\n");
        return;
    }

    for (i = idx; i < jumlahMatkul - 1; i++) {
        strcpy(kodeMatkul[i], kodeMatkul[i + 1]);
        strcpy(namaMatkul[i], namaMatkul[i + 1]);
        sks[i] = sks[i + 1];
    }
    jumlahMatkul--;
    printf("Mata kuliah berhasil dihapus.\n");
}
