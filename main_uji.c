/*
 * main_uji.c - main SEMENTARA untuk menguji penilaian.c + laporan.c sendiri.
 * Berisi: definisi array (seharusnya di main.c asli), helper input/tampilan
 * versi sederhana, cariIndexMhs/cariIndexMatkul versi sederhana, dan data contoh.
 *
 * PAKAI HANYA SAAT file anggota lain belum ada. Jangan dikompilasi bersama
 * main.c / mahasiswa.c / matakuliah.c asli (akan bentrok / duplikat definisi).
 *
 * Compile:  gcc main_uji.c penilaian.c laporan.c -o uji
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "data.h"

char username[MAX_USER][30]; char password[MAX_USER][30]; char role[MAX_USER][15];
int statusBlokir[MAX_USER]; int gagalLogin[MAX_USER]; int jumlahUser; int userAktif;
char nim[MAX_MHS][20]; char nama[MAX_MHS][50]; char jurusan[MAX_MHS][50]; int angkatan[MAX_MHS];
float ipMhs[MAX_MHS]; int jumlahMahasiswa;
char kodeMatkul[MAX_MATKUL][15]; char namaMatkul[MAX_MATKUL][50]; int sks[MAX_MATKUL]; int jumlahMatkul;
char nimNilai[MAX_NILAI][20]; char kodeNilai[MAX_NILAI][15];
float kehadiran[MAX_NILAI], tugas[MAX_NILAI], uts[MAX_NILAI], uas[MAX_NILAI], nilaiAkhir[MAX_NILAI];
char nilaiHuruf[MAX_NILAI][3]; float bobot[MAX_NILAI]; int jumlahNilai;

void bersihLayar(void){ printf("\n----- [layar dibersihkan] -----\n"); }
void jeda(void){ printf("[jeda]\n"); }
void judul(const char t[]){ printf("== %s ==\n", t); }
void garis(int p, char c){ int i; for(i=0;i<p;i++) putchar(c); putchar('\n'); }
void bacaStringWajib(const char p[], char h[], int m){ printf("%s", p); if(scanf("%s",h)!=1) exit(1); h[m-1]=0; }
void bacaString(const char p[], char h[], int m){ bacaStringWajib(p,h,m); }
int bacaInt(const char p[], int mn, int mx){ int v; for(;;){ printf("%s",p); if(scanf("%d",&v)!=1) exit(1); if(v>=mn&&v<=mx) return v; printf("invalid\n"); } }
int bacaIntEdit(const char p[], int l, int mn, int mx){ (void)l; return bacaInt(p,mn,mx); }
float bacaFloat(const char p[], float mn, float mx){ float v; for(;;){ printf("%s",p); if(scanf("%f",&v)!=1) exit(1); if(v>=mn&&v<=mx) return v; printf("  (input ditolak: di luar rentang)\n"); } }
float bacaFloatEdit(const char p[], float l, float mn, float mx){ (void)l; return bacaFloat(p,mn,mx); }
int konfirmasi(const char p[]){ char c[8]; printf("%s",p); if(scanf("%s",c)!=1) exit(1); return c[0]=='y'||c[0]=='Y'; }
void keHurufBesar(char s[]){ int i; for(i=0;s[i];i++) s[i]=toupper((unsigned char)s[i]); }
void keHurufKecil(const char s[], char t[]){ int i; for(i=0;s[i];i++) t[i]=tolower((unsigned char)s[i]); t[i]=0; }
void isiDataContoh(void){}
int cariIndexMhs(const char n[]){ int i; for(i=0;i<jumlahMahasiswa;i++) if(!strcmp(nim[i],n)) return i; return -1; }
int cariIndexMatkul(const char k[]){ int i; for(i=0;i<jumlahMatkul;i++) if(!strcmp(kodeMatkul[i],k)) return i; return -1; }

static void addM(const char*n,const char*nm,const char*j,int a){ strcpy(nim[jumlahMahasiswa],n); strcpy(nama[jumlahMahasiswa],nm); strcpy(jurusan[jumlahMahasiswa],j); angkatan[jumlahMahasiswa++]=a; }
static void addK(const char*k,const char*nm,int s){ strcpy(kodeMatkul[jumlahMatkul],k); strcpy(namaMatkul[jumlahMatkul],nm); sks[jumlahMatkul++]=s; }

int main(void){
  int pilih;
  addM("M001","Andi Pratama","Informatika",2024); addM("M002","Siti Aminah","Informatika",2024);
  addM("M003","Budi Santoso","Sistem Informasi",2023); addM("M004","Dewi Lestari","Informatika",2024);
  addK("IF101","Algoritma",3); addK("IF102","Struktur Data",4); addK("IF103","Basis Data",3);
  simpanNilai("M001","IF101",90,90,85,88);
  simpanNilai("M001","IF102",80,75,70,72);
  simpanNilai("M001","IF103",100,100,100,100);
  simpanNilai("M002","IF101",70,60,65,62);
  simpanNilai("M002","IF102",85,85,84,84);
  hitungIP();

  do {
    bersihLayar();
    judul("MENU UJI MODUL PENILAIAN");
    printf("  1. Input nilai\n  2. Tampil seluruh nilai (Dosen)\n  3. Edit nilai\n  4. Hapus nilai\n");
    printf("  5. Ranking\n  6. Laporan\n  7. Hitung ulang semua nilai\n");
    printf("  8. Tampil nilai & IP mahasiswa (pilih indeks 0-3)\n  0. Keluar\n");
    pilih = bacaInt("  Pilih : ", 0, 8);
    switch (pilih) {
      case 1: inputNilai(); break;
      case 2: tampilNilai(); break;
      case 3: editNilai(); break;
      case 4: hapusNilai(); break;
      case 5: rankingMahasiswa(); break;
      case 6: laporanNilai(); break;
      case 7: hitungNilai(); jeda(); break;
      case 8: {
        int idx = bacaInt("  Indeks mahasiswa : ", 0, jumlahMahasiswa - 1);
        tampilNilaiMahasiswa(idx); printf("\n");
        tampilNilaiAkhirMahasiswa(idx); tampilIPMahasiswa(idx); jeda(); break;
      }
      default: break;
    }
  } while (pilih != 0);
  return 0;
}
