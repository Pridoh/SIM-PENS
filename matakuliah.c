#include <stdio.h>
#include <string.h>
#include "data.h"

/* Mengembalikan indeks mata kuliah dengan kode tertentu, atau -1 jika tidak ada */
int cariIndexMatkul(const char kode[]) {
    int i;
    for (i = 0; i < jumlahMk; i++) {
        if (strcmp(dataMk[i].kode, kode) == 0) return i;
    }
    return -1;
}

/* 6 + 10. Tambah mata kuliah dengan validasi kode unik */
void tambahMatkul(void) {
    char kode[10];
    MataKuliah mk;

    if (jumlahMk >= MAX_MK) {
        printf("Data penuh! Maksimal %d mata kuliah.\n", MAX_MK);
        return;
    }

    bacaString("Kode Mata Kuliah : ", kode, sizeof(kode));
    if (strlen(kode) == 0) {
        printf("Kode tidak boleh kosong.\n");
        return;
    }
    if (cariIndexMatkul(kode) != -1) {
        printf("Gagal: Kode %s sudah terdaftar (duplikat).\n", kode);
        return;
    }
    strcpy(mk.kode, kode);

    bacaString("Nama Mata Kuliah : ", mk.nama, sizeof(mk.nama));
    if (strlen(mk.nama) == 0) {
        printf("Nama tidak boleh kosong.\n");
        return;
    }

    do {
        mk.sks = bacaInt("Jumlah SKS (1-6) : ");
        if (mk.sks < 1 || mk.sks > 6)
            printf("SKS harus antara 1 sampai 6.\n");
    } while (mk.sks < 1 || mk.sks > 6);

    dataMk[jumlahMk++] = mk;
    printf("Mata kuliah berhasil ditambahkan.\n");
}

/* 7. Tampil seluruh mata kuliah */
void tampilMatkul(void) {
    int i, totalSks = 0;
    if (jumlahMk == 0) {
        printf("Belum ada data mata kuliah.\n");
        return;
    }
    printf("\n");
    printf("+-----+----------+--------------------------------+-----+\n");
    printf("| No  | Kode     | Nama Mata Kuliah               | SKS |\n");
    printf("+-----+----------+--------------------------------+-----+\n");
    for (i = 0; i < jumlahMk; i++) {
        printf("| %-3d | %-8s | %-30s | %-3d |\n",
               i + 1, dataMk[i].kode, dataMk[i].nama, dataMk[i].sks);
        totalSks += dataMk[i].sks;
    }
    printf("+-----+----------+--------------------------------+-----+\n");
    printf("Total: %d mata kuliah, %d SKS\n", jumlahMk, totalSks);
}

/* 8. Ubah data mata kuliah (kosongkan input = tidak diubah) */
void ubahMatkul(void) {
    char kode[10], buf[50];
    int idx, sksBaru;

    bacaString("Masukkan kode yang akan diubah: ", kode, sizeof(kode));
    idx = cariIndexMatkul(kode);
    if (idx == -1) {
        printf("Kode %s tidak ditemukan.\n", kode);
        return;
    }

    printf("Data saat ini: %s - %s (%d SKS)\n",
           dataMk[idx].kode, dataMk[idx].nama, dataMk[idx].sks);
    printf("(Kosongkan input lalu Enter jika tidak ingin mengubah. Kode tidak dapat diubah.)\n");

    bacaString("Nama baru : ", buf, sizeof(dataMk[idx].nama));
    if (strlen(buf) > 0) strcpy(dataMk[idx].nama, buf);

    bacaString("SKS baru (1-6): ", buf, sizeof(buf));
    if (strlen(buf) > 0) {
        if (sscanf(buf, "%d", &sksBaru) == 1 && sksBaru >= 1 && sksBaru <= 6)
            dataMk[idx].sks = sksBaru;
        else
            printf("SKS tidak valid, SKS tidak diubah.\n");
    }
    printf("Data mata kuliah berhasil diperbarui.\n");
}

/* 9. Hapus mata kuliah + geser indeks array */
void hapusMatkul(void) {
    char kode[10], yakin[10];
    int idx, i;

    bacaString("Masukkan kode yang akan dihapus: ", kode, sizeof(kode));
    idx = cariIndexMatkul(kode);
    if (idx == -1) {
        printf("Kode %s tidak ditemukan.\n", kode);
        return;
    }

    printf("Akan menghapus: %s - %s (%d SKS)\n",
           dataMk[idx].kode, dataMk[idx].nama, dataMk[idx].sks);
    bacaString("Yakin hapus? (y/n): ", yakin, sizeof(yakin));
    if (yakin[0] != 'y' && yakin[0] != 'Y') {
        printf("Penghapusan dibatalkan.\n");
        return;
    }

    for (i = idx; i < jumlahMk - 1; i++) {
        dataMk[i] = dataMk[i + 1];
    }
    jumlahMk--;
    printf("Mata kuliah berhasil dihapus.\n");
}
