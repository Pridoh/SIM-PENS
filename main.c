/*
 * main.c - Program utama SIM-PENS (Anggota 1)
 * Berisi: definisi array global, alat bantu input/tampilan, dan alur menu awal.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "data.h"

/* ===================== DEFINISI ARRAY GLOBAL ===================== */
char username[MAX_USER][30];
char password[MAX_USER][30];
char role[MAX_USER][15];
int  statusBlokir[MAX_USER];
int  gagalLogin[MAX_USER];
int  jumlahUser = 0;
int  userAktif = -1;

char  nim[MAX_MHS][20];
char  nama[MAX_MHS][50];
char  jurusan[MAX_MHS][50];
int   angkatan[MAX_MHS];
float ipMhs[MAX_MHS];
int   jumlahMahasiswa = 0;

char kodeMatkul[MAX_MATKUL][15];
char namaMatkul[MAX_MATKUL][50];
int  sks[MAX_MATKUL];
int  jumlahMatkul = 0;

char  nimNilai[MAX_NILAI][20];
char  kodeNilai[MAX_NILAI][15];
float kehadiran[MAX_NILAI];
float tugas[MAX_NILAI];
float uts[MAX_NILAI];
float uas[MAX_NILAI];
float nilaiAkhir[MAX_NILAI];
char  nilaiHuruf[MAX_NILAI][3];
float bobot[MAX_NILAI];
int   jumlahNilai = 0;

/* ===================== ALAT BANTU TAMPILAN ===================== */
void bersihLayar(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void garis(int panjang, char karakter) {
    int i;
    for (i = 0; i < panjang; i++) {
        putchar(karakter);
    }
    putchar('\n');
}

void judul(const char teks[]) {
    garis(60, '=');
    printf("  %s\n", teks);
    garis(60, '=');
}

void jeda(void) {
    char sisa[10];
    bacaString("\nTekan Enter untuk melanjutkan...", sisa, 10);
}

/* ===================== ALAT BANTU INPUT ===================== */
void bacaString(const char prompt[], char hasil[], int maks) {
    char buf[256];
    int len, awal, akhir, j, k;

    printf("%s", prompt);
    if (gets(buf) == NULL) {
        printf("\nInput berakhir. Program ditutup.\n");
        exit(0);
    }
    len = (int)strlen(buf);

    /* hapus spasi di awal dan akhir */
    awal = 0;
    while (awal < len && isspace((unsigned char)buf[awal])) {
        awal++;
    }
    akhir = len - 1;
    while (akhir >= awal && isspace((unsigned char)buf[akhir])) {
        akhir--;
    }

    j = 0;
    for (k = awal; k <= akhir && j < maks - 1; k++) {
        hasil[j++] = buf[k];
    }
    hasil[j] = '\0';
}

void bacaStringWajib(const char prompt[], char hasil[], int maks) {
    do {
        bacaString(prompt, hasil, maks);
        if (hasil[0] == '\0') {
            printf("  [!] Input tidak boleh kosong.\n");
        }
    } while (hasil[0] == '\0');
}

int bacaInt(const char prompt[], int min, int max) {
    char buf[40];
    char sisa;
    int nilai;

    while (1) {
        bacaString(prompt, buf, 40);
        if (sscanf(buf, "%d %c", &nilai, &sisa) == 1) {
            if (nilai >= min && nilai <= max) {
                return nilai;
            }
        }
        printf("  [!] Masukkan angka bulat antara %d - %d.\n", min, max);
    }
}

int bacaIntEdit(const char prompt[], int lama, int min, int max) {
    char buf[40];
    char sisa;
    int nilai;

    while (1) {
        bacaString(prompt, buf, 40);
        if (buf[0] == '\0') {
            return lama;
        }
        if (sscanf(buf, "%d %c", &nilai, &sisa) == 1) {
            if (nilai >= min && nilai <= max) {
                return nilai;
            }
        }
        printf("  [!] Masukkan angka bulat antara %d - %d (atau Enter untuk tetap).\n", min, max);
    }
}

/* Mengubah koma desimal menjadi titik agar "85,5" juga diterima */
static void komaKeTitik(char s[]) {
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] == ',') {
            s[i] = '.';
        }
    }
}

float bacaFloat(const char prompt[], float min, float max) {
    char buf[40];
    char sisa;
    float nilai;

    while (1) {
        bacaString(prompt, buf, 40);
        komaKeTitik(buf);
        if (sscanf(buf, "%f %c", &nilai, &sisa) == 1) {
            if (nilai >= min && nilai <= max) {
                return nilai;
            }
        }
        printf("  [!] Masukkan angka antara %.0f - %.0f.\n", min, max);
    }
}

float bacaFloatEdit(const char prompt[], float lama, float min, float max) {
    char buf[40];
    char sisa;
    float nilai;

    while (1) {
        bacaString(prompt, buf, 40);
        if (buf[0] == '\0') {
            return lama;
        }
        komaKeTitik(buf);
        if (sscanf(buf, "%f %c", &nilai, &sisa) == 1) {
            if (nilai >= min && nilai <= max) {
                return nilai;
            }
        }
        printf("  [!] Masukkan angka antara %.0f - %.0f (atau Enter untuk tetap).\n", min, max);
    }
}

int konfirmasi(const char prompt[]) {
    char jawab[10];
    bacaString(prompt, jawab, 10);
    return (jawab[0] == 'y' || jawab[0] == 'Y');
}

void keHurufBesar(char s[]) {
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        s[i] = (char)toupper((unsigned char)s[i]);
    }
}

void keHurufKecil(const char sumber[], char tujuan[]) {
    int i;
    for (i = 0; sumber[i] != '\0'; i++) {
        tujuan[i] = (char)tolower((unsigned char)sumber[i]);
    }
    tujuan[i] = '\0';
}

/* ===================== DATA CONTOH (UNTUK DEMO) ===================== */
void isiDataContoh(void) {
    int i;

    /* Mahasiswa */
    strcpy(nim[0], "242660001"); strcpy(nama[0], "Ilham"); strcpy(jurusan[0], "Informatika"); angkatan[0] = 2024;
    strcpy(nim[1], "242660002"); strcpy(nama[1], "Andi");  strcpy(jurusan[1], "Informatika"); angkatan[1] = 2024;
    strcpy(nim[2], "242660003"); strcpy(nama[2], "Budi");  strcpy(jurusan[2], "Informatika"); angkatan[2] = 2024;
    jumlahMahasiswa = 3;
    for (i = 0; i < jumlahMahasiswa; i++) {
        tambahAkun(nim[i], nim[i], "mahasiswa");   /* username & password awal = NIM */
    }

    /* Mata kuliah */
    strcpy(kodeMatkul[0], "DSP");  strcpy(namaMatkul[0], "Dasar Pemrograman"); sks[0] = 3;
    strcpy(kodeMatkul[1], "MTK");  strcpy(namaMatkul[1], "Matematika");        sks[1] = 2;
    strcpy(kodeMatkul[2], "LGD");  strcpy(namaMatkul[2], "Logika Digital");    sks[2] = 2;
    strcpy(kodeMatkul[3], "BIG");  strcpy(namaMatkul[3], "Bahasa Inggris");    sks[3] = 2;
    jumlahMatkul = 4;

    /* Nilai: nim, kode, kehadiran, tugas, uts, uas */
    simpanNilai("242660001", "DSP", 90, 85, 80, 90);
    simpanNilai("242660001", "MTK", 85, 80, 75, 80);
    simpanNilai("242660001", "LGD", 80, 80, 70, 78);
    simpanNilai("242660001", "BIG", 95, 90, 85, 88);
    simpanNilai("242660002", "DSP", 100, 95, 90, 95);
    simpanNilai("242660002", "MTK", 90, 90, 88, 92);
    simpanNilai("242660002", "LGD", 95, 90, 85, 90);
    simpanNilai("242660002", "BIG", 90, 88, 90, 92);
    simpanNilai("242660003", "DSP", 80, 75, 70, 72);
    simpanNilai("242660003", "MTK", 70, 65, 60, 55);
    simpanNilai("242660003", "LGD", 85, 80, 78, 80);
    simpanNilai("242660003", "BIG", 75, 70, 65, 60);
}

/* ===================== PROGRAM UTAMA ===================== */
int main(void) {
    int pilih;

    /* akun dosen bawaan */
    tambahAkun("dosen", "dosen123", "dosen");

#if ISI_DATA_CONTOH
    isiDataContoh();
#endif

    do {
        bersihLayar();
        judul("SIM-PENS | Sistem Informasi Penilaian Mahasiswa");
        printf("  Mata Kuliah Dasar Pemrograman (Bahasa C)\n\n");
        printf("  1. Login\n");
        printf("  0. Keluar\n\n");
        pilih = bacaInt("Pilih menu: ", 0, 1);

        switch (pilih) {
            case 1:
                login();
                break;
            case 0:
                printf("\nTerima kasih telah menggunakan SIM-PENS.\n");
                break;
        }
    } while (pilih != 0);
}