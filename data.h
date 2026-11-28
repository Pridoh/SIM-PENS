/*
 * data.h - Header bersama SIM-PENS
 * Berisi: konstanta, deklarasi extern array paralel, dan prototype seluruh fungsi.
 * Array-nya didefinisikan (dibuat) satu kali saja di main.c.
 */
#ifndef DATA_H
#define DATA_H

/* ===================== KONSTANTA ===================== */
#define MAX_USER        100
#define MAX_MHS         100
#define MAX_MATKUL      30
#define MAX_NILAI       500
#define MAX_GAGAL_LOGIN 3

/* Bobot komponen nilai (dalam persen, total harus 100).
 * Rancangan proyek : 10 / 20 / 30 / 40
 * Jika memakai dokumen "Sistem Penilaian" (3 komponen): 0 / 30 / 30 / 40 */
#define BOBOT_HADIR     10
#define BOBOT_TUGAS     20
#define BOBOT_UTS       30
#define BOBOT_UAS       40

#define NILAI_LULUS     60.0f   /* Nilai akhir minimal untuk dinyatakan lulus (grade C) */

/* 1 = program diisi data contoh saat dijalankan (untuk demo), 0 = mulai kosong */
#define ISI_DATA_CONTOH 1

/* ===================== A. ARRAY AKUN PENGGUNA ===================== */
extern char username[MAX_USER][30];
extern char password[MAX_USER][30];
extern char role[MAX_USER][15];        /* "dosen" atau "mahasiswa" */
extern int  statusBlokir[MAX_USER];    /* 0 = aktif, 1 = terblokir */
extern int  gagalLogin[MAX_USER];      /* penghitung gagal login (maks 3) */
extern int  jumlahUser;
extern int  userAktif;                 /* indeks akun yang sedang login, -1 jika belum */

/* ===================== B. ARRAY DATA MAHASISWA ===================== */
extern char  nim[MAX_MHS][20];
extern char  nama[MAX_MHS][50];
extern char  jurusan[MAX_MHS][50];
extern int   angkatan[MAX_MHS];
extern float ipMhs[MAX_MHS];           /* hasil hitungIP() untuk tiap mahasiswa */
extern int   jumlahMahasiswa;

/* ===================== C. ARRAY DATA MATA KULIAH ===================== */
extern char kodeMatkul[MAX_MATKUL][15];
extern char namaMatkul[MAX_MATKUL][50];
extern int  sks[MAX_MATKUL];
extern int  jumlahMatkul;

/* ===================== D. ARRAY DATA PENILAIAN ===================== */
extern char  nimNilai[MAX_NILAI][20];   /* relasi ke nim mahasiswa */
extern char  kodeNilai[MAX_NILAI][15];  /* relasi ke kode mata kuliah */
extern float kehadiran[MAX_NILAI];
extern float tugas[MAX_NILAI];
extern float uts[MAX_NILAI];
extern float uas[MAX_NILAI];
extern float nilaiAkhir[MAX_NILAI];
extern char  nilaiHuruf[MAX_NILAI][3];
extern float bobot[MAX_NILAI];
extern int   jumlahNilai;

/* ===================== PROTOTYPE: main.c (alat bantu input/tampilan) ===================== */
void bersihLayar(void);
void jeda(void);
void judul(const char teks[]);
void garis(int panjang, char karakter);
void bacaString(const char prompt[], char hasil[], int maks);        /* boleh kosong */
void bacaStringWajib(const char prompt[], char hasil[], int maks);   /* tidak boleh kosong */
int  bacaInt(const char prompt[], int min, int max);
int  bacaIntEdit(const char prompt[], int lama, int min, int max);   /* Enter = nilai lama */
float bacaFloat(const char prompt[], float min, float max);
float bacaFloatEdit(const char prompt[], float lama, float min, float max);
int  konfirmasi(const char prompt[]);                                /* 1 jika jawab y/Y */
void keHurufBesar(char s[]);
void keHurufKecil(const char sumber[], char tujuan[]);
void isiDataContoh(void);

/* ===================== PROTOTYPE: login.c (Anggota 1) ===================== */
void login(void);
void menuDosen(void);
void menuMahasiswa(void);
int  cariIndexUser(const char user[]);
int  tambahAkun(const char user[], const char pass[], const char peran[]);   /* 1 = berhasil */
void hapusAkun(const char user[]);

/* ===================== PROTOTYPE: mahasiswa.c (Anggota 2) ===================== */
void tambahMahasiswa(void);
void tampilMahasiswa(void);
void cariMahasiswa(void);
void ubahMahasiswa(void);
void hapusMahasiswa(void);
int  cariIndexMhs(const char nimCari[]);
void profilMahasiswa(int idx);

/* ===================== PROTOTYPE: matakuliah.c (Anggota 2) ===================== */
void tambahMatkul(void);
void tampilMatkul(void);
void ubahMatkul(void);
void hapusMatkul(void);
int  cariIndexMatkul(const char kode[]);

/* ===================== PROTOTYPE: penilaian.c (Anggota 3) ===================== */
void  inputNilai(void);
void  hitungNilai(void);                      /* hitung ulang NA, huruf, bobot semua data nilai */
void  tampilNilai(void);                      /* rekap seluruh nilai (tampilan Dosen) */
void  editNilai(void);
void  hapusNilai(void);
int   cariIndexNilai(const char nimCari[], const char kode[]);
float hitungNA(float h, float t, float u, float a);
void  hitungSatuNilai(int k);
int   simpanNilai(const char nimCari[], const char kode[], float h, float t, float u, float a);
void  hapusNilaiIndex(int k);
void  tampilNilaiMahasiswa(int idx);          /* komponen nilai milik 1 mahasiswa */
void  tampilNilaiAkhirMahasiswa(int idx);     /* NA + huruf milik 1 mahasiswa */

/* ===================== PROTOTYPE: laporan.c (Anggota 3) ===================== */
void hitungIP(void);                          /* isi ipMhs[] untuk semua mahasiswa */
void tampilIPMahasiswa(int idx);
void rankingMahasiswa(void);
void laporanNilai(void);

#endif