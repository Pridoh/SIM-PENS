#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "data.h"

/* Definisi variabel global (sesuai data.h) */
char  nim[MAX_MHS][20];
char  nama[MAX_MHS][50];
char  jurusan[MAX_MHS][50];
int   angkatan[MAX_MHS];
float ipMhs[MAX_MHS];
int   jumlahMahasiswa = 0;

char  kodeMatkul[MAX_MATKUL][15];
char  namaMatkul[MAX_MATKUL][50];
int   sks[MAX_MATKUL];
int   jumlahMatkul = 0;

/* Baca satu baris string dengan aman (buang newline) */
void bacaString(const char prompt[], char hasil[], int maks) {
    int len;
    printf("%s", prompt);
    if (fgets(hasil, maks, stdin) == NULL) {
        hasil[0] = '\0';
        return;
    }
    len = strlen(hasil);
    if (len > 0 && hasil[len - 1] == '\n') {
        hasil[len - 1] = '\0';
    } else {
        int c;                                   /* buang sisa input yang kepanjangan */
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

void bacaStringWajib(const char prompt[], char hasil[], int maks) {
    do {
        bacaString(prompt, hasil, maks);
        if (strlen(hasil) == 0) printf("Input tidak boleh kosong.\n");
    } while (strlen(hasil) == 0);
}

int bacaInt(const char prompt[], int min, int max) {
    char buf[30];
    int x;
    for (;;) {
        bacaString(prompt, buf, sizeof(buf));
        if (sscanf(buf, "%d", &x) == 1 && x >= min && x <= max) return x;
        printf("Input tidak valid (%d-%d).\n", min, max);
    }
}

/* Enter (kosong) = pakai nilai lama */
int bacaIntEdit(const char prompt[], int lama, int min, int max) {
    char buf[30];
    int x;
    for (;;) {
        bacaString(prompt, buf, sizeof(buf));
        if (strlen(buf) == 0) return lama;
        if (sscanf(buf, "%d", &x) == 1 && x >= min && x <= max) return x;
        printf("Input tidak valid (%d-%d).\n", min, max);
    }
}

int konfirmasi(const char prompt[]) {
    char buf[10];
    bacaString(prompt, buf, sizeof(buf));
    return buf[0] == 'y' || buf[0] == 'Y';
}

void keHurufKecil(const char sumber[], char tujuan[]) {
    int i;
    for (i = 0; sumber[i] != '\0'; i++) tujuan[i] = tolower((unsigned char)sumber[i]);
    tujuan[i] = '\0';
}

static void menuMahasiswa(void) {
    int p;
    do {
        printf("\n=== KELOLA DATA MAHASISWA ===\n");
        printf("1. Tambah\n2. Tampil\n3. Cari\n4. Ubah\n5. Hapus\n0. Kembali\n");
        p = bacaInt("Pilihan: ", 0, 5);
        switch (p) {
            case 1: tambahMahasiswa(); break;
            case 2: tampilMahasiswa(); break;
            case 3: cariMahasiswa();   break;
            case 4: ubahMahasiswa();   break;
            case 5: hapusMahasiswa();  break;
        }
    } while (p != 0);
}

static void menuMatkul(void) {
    int p;
    do {
        printf("\n=== KELOLA DATA MATA KULIAH ===\n");
        printf("1. Tambah\n2. Tampil\n3. Cari\n4. Ubah\n5. Hapus\n0. Kembali\n");
        p = bacaInt("Pilihan: ", 0, 5);
        switch (p) {
            case 1: tambahMatkul(); break;
            case 2: tampilMatkul(); break;
            case 3: cariMatkul();   break;
            case 4: ubahMatkul();   break;
            case 5: hapusMatkul();  break;
        }
    } while (p != 0);
}

int main(void) {
    int p;
    do {
        printf("\n=== SISTEM AKADEMIK ===\n");
        printf("1. Kelola Data Mahasiswa\n2. Kelola Data Mata Kuliah\n0. Keluar\n");
        p = bacaInt("Pilihan: ", 0, 2);
        switch (p) {
            case 1: menuMahasiswa(); break;
            case 2: menuMatkul();    break;
            case 0: printf("Terima kasih.\n"); break;
        }
    } while (p != 0);
    return 0;
}
