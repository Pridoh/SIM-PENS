/*
 * penilaian.c - Modul Penilaian (Anggota 3)
 * Tugas 27 - 33
 *
 *  27. inputNilai(), simpanNilai()                    -> input kehadiran, tugas, UTS, UAS
 *  28. nilaiValid(), bacaKomponen()                   -> validasi rentang 0 - 100
 *  29. hitungNA(), hitungSatuNilai(), hitungNilai()   -> nilai akhir sesuai bobot persentase
 *  30. konversiNilai(), predikatNilai()               -> nilai angka -> huruf + bobot mutu
 *  31. tampilNilai(), tampilNilaiMahasiswa(),
 *      tampilNilaiAkhirMahasiswa()                    -> tampilan Dosen & Mahasiswa
 *  32. editNilai()
 *  33. hapusNilai(), hapusNilaiIndex()
 */
#include <stdio.h>
#include <string.h>
#include "data.h"

#define NILAI_MIN 0.0f
#define NILAI_MAX 100.0f

/* Cek saat kompilasi: total bobot di data.h wajib 100 */
#if (BOBOT_HADIR + BOBOT_TUGAS + BOBOT_UTS + BOBOT_UAS) != 100
#error "Total BOBOT_HADIR + BOBOT_TUGAS + BOBOT_UTS + BOBOT_UAS di data.h harus 100"
#endif

/* ================================================================== */
/*                      FUNGSI BANTU (internal)                       */
/* ================================================================== */

/* [28] 1 jika nilai berada pada rentang 0 - 100 */
static int nilaiValid(float v)
{
    return (v >= NILAI_MIN && v <= NILAI_MAX);
}

/* Salin string dengan aman (tidak melebihi ukuran array tujuan) */
static void salinString(char tujuan[], const char sumber[], int ukuran)
{
    strncpy(tujuan, sumber, ukuran - 1);
    tujuan[ukuran - 1] = '\0';
}

/* [30] Konversi nilai akhir (angka) -> nilai huruf + bobot mutu
 *  85-100 A 4.0 | 80-84 B+ 3.5 | 75-79 B 3.0 | 70-74 C+ 2.5
 *  60-69  C 2.0 | 50-59 D 1.0  | 0-49  E 0.0                    */
static void konversiNilai(float na, char huruf[], float *bm)
{
    if (na >= 85.0f)      { strcpy(huruf, "A");  *bm = 4.0f; }
    else if (na >= 80.0f) { strcpy(huruf, "B+"); *bm = 3.5f; }
    else if (na >= 75.0f) { strcpy(huruf, "B");  *bm = 3.0f; }
    else if (na >= 70.0f) { strcpy(huruf, "C+"); *bm = 2.5f; }
    else if (na >= 60.0f) { strcpy(huruf, "C");  *bm = 2.0f; }
    else if (na >= 50.0f) { strcpy(huruf, "D");  *bm = 1.0f; }
    else                  { strcpy(huruf, "E");  *bm = 0.0f; }
}

/* [30] Keterangan predikat dari nilai huruf */
static const char *predikatNilai(const char huruf[])
{
    if (strcmp(huruf, "A") == 0)  return "Sangat Baik";
    if (strcmp(huruf, "B+") == 0) return "Baik Sekali";
    if (strcmp(huruf, "B") == 0)  return "Baik";
    if (strcmp(huruf, "C+") == 0) return "Cukup Baik";
    if (strcmp(huruf, "C") == 0)  return "Cukup";
    if (strcmp(huruf, "D") == 0)  return "Kurang";
    return "Tidak Lulus";
}

/* [28] Baca 1 komponen nilai, dipaksa berada pada rentang 0 - 100 */
static float bacaKomponen(const char prompt[])
{
    float v;
    do {
        v = bacaFloat(prompt, NILAI_MIN, NILAI_MAX);
        if (!nilaiValid(v))
            printf("  [!] Nilai harus berada pada rentang 0 - 100.\n");
    } while (!nilaiValid(v));
    return v;
}

/* [28] Versi edit: tekan Enter = pakai nilai lama */
static float bacaKomponenEdit(const char prompt[], float lama)
{
    float v;
    do {
        v = bacaFloatEdit(prompt, lama, NILAI_MIN, NILAI_MAX);
        if (!nilaiValid(v))
            printf("  [!] Nilai harus berada pada rentang 0 - 100.\n");
    } while (!nilaiValid(v));
    return v;
}

/* Minta NIM + Kode MK, pastikan keduanya terdaftar.
 * Mengembalikan 1 jika valid, 0 jika tidak (pesan sudah dicetak). */
static int pilihMhsMatkul(char nimIn[], char kodeIn[], int *iM, int *iK)
{
    bacaStringWajib("  NIM Mahasiswa    : ", nimIn, 20);
    *iM = cariIndexMhs(nimIn);
    if (*iM == -1) {
        printf("\n  [!] NIM %s tidak ditemukan.\n", nimIn);
        return 0;
    }

    bacaStringWajib("  Kode Mata Kuliah : ", kodeIn, 15);
    keHurufBesar(kodeIn);
    *iK = cariIndexMatkul(kodeIn);
    if (*iK == -1) {
        printf("\n  [!] Kode mata kuliah %s tidak ditemukan.\n", kodeIn);
        return 0;
    }

    printf("\n  Mahasiswa   : %s - %s\n", nim[*iM], nama[*iM]);
    printf("  Mata Kuliah : %s - %s (%d SKS)\n\n",
           kodeMatkul[*iK], namaMatkul[*iK], sks[*iK]);
    return 1;
}

/* ================================================================== */
/*                    [29] PERHITUNGAN NILAI AKHIR                    */
/* ================================================================== */

/* NA = (H*bobotH + T*bobotT + UTS*bobotUTS + UAS*bobotUAS) / 100
 * Hasil dibulatkan 2 desimal supaya konversi huruf tidak meleset
 * karena error pecahan float (mis. 84.9999 padahal seharusnya 85). */
float hitungNA(float h, float t, float u, float a)
{
    float na = (h * BOBOT_HADIR + t * BOBOT_TUGAS +
                u * BOBOT_UTS   + a * BOBOT_UAS) / 100.0f;
    return (float)((int)(na * 100.0f + 0.5f)) / 100.0f;
}

/* Hitung NA, nilai huruf, dan bobot mutu untuk 1 data nilai (indeks k) */
void hitungSatuNilai(int k)
{
    if (k < 0 || k >= jumlahNilai) return;
    nilaiAkhir[k] = hitungNA(kehadiran[k], tugas[k], uts[k], uas[k]);
    konversiNilai(nilaiAkhir[k], nilaiHuruf[k], &bobot[k]);
}

/* Hitung ulang seluruh data nilai (+ perbarui IP semua mahasiswa) */
void hitungNilai(void)
{
    int k;
    for (k = 0; k < jumlahNilai; k++)
        hitungSatuNilai(k);
    hitungIP();
    printf("  %d data nilai berhasil dihitung ulang.\n", jumlahNilai);
}

/* ================================================================== */
/*                       PENCARIAN & PENYIMPANAN                      */
/* ================================================================== */

/* Cari data nilai berdasarkan pasangan NIM + Kode MK. -1 jika tidak ada */
int cariIndexNilai(const char nimCari[], const char kode[])
{
    int i;
    for (i = 0; i < jumlahNilai; i++) {
        if (strcmp(nimNilai[i], nimCari) == 0 && strcmp(kodeNilai[i], kode) == 0)
            return i;
    }
    return -1;
}

/* Simpan nilai satu mahasiswa untuk satu mata kuliah.
 * - Pasangan NIM+Kode belum ada -> ditambahkan, sudah ada -> komponennya diperbarui
 * - Gagal (return 0) jika NIM/Kode tidak terdaftar, nilai di luar 0-100, atau array penuh
 * Return 1 jika berhasil. Cocok juga dipakai isiDataContoh() di main.c. */
int simpanNilai(const char nimCari[], const char kode[],
                float h, float t, float u, float a)
{
    int k;

    if (cariIndexMhs(nimCari) == -1 || cariIndexMatkul(kode) == -1) return 0;
    if (!nilaiValid(h) || !nilaiValid(t) || !nilaiValid(u) || !nilaiValid(a)) return 0;

    k = cariIndexNilai(nimCari, kode);
    if (k == -1) {
        if (jumlahNilai >= MAX_NILAI) return 0;
        k = jumlahNilai;
        salinString(nimNilai[k],  nimCari, (int)sizeof(nimNilai[0]));
        salinString(kodeNilai[k], kode,    (int)sizeof(kodeNilai[0]));
        jumlahNilai++;
    }

    kehadiran[k] = h;
    tugas[k]     = t;
    uts[k]       = u;
    uas[k]       = a;
    hitungSatuNilai(k);
    return 1;
}

/* ================================================================== */
/*                      [27] INPUT NILAI KOMPONEN                     */
/* ================================================================== */
void inputNilai(void)
{
    char  nimIn[20], kodeIn[15], huruf[3];
    int   iM, iK;
    float h, t, u, a, na, bm;

    do {
        bersihLayar();
        judul("INPUT NILAI MAHASISWA");

        if (jumlahMahasiswa == 0 || jumlahMatkul == 0) {
            printf("  [!] Data mahasiswa atau mata kuliah masih kosong.\n");
            jeda();
            return;
        }
        if (jumlahNilai >= MAX_NILAI) {
            printf("  [!] Penyimpanan nilai penuh (maksimal %d data).\n", MAX_NILAI);
            jeda();
            return;
        }

        if (pilihMhsMatkul(nimIn, kodeIn, &iM, &iK)) {
            if (cariIndexNilai(nim[iM], kodeMatkul[iK]) != -1) {
                printf("  [!] Nilai untuk mahasiswa & mata kuliah ini sudah ada.\n");
                printf("      Gunakan menu Edit Nilai untuk mengubahnya.\n");
            } else {
                printf("  Masukkan nilai komponen (skala 0 - 100)\n");
                h = bacaKomponen("  Kehadiran : ");
                t = bacaKomponen("  Tugas     : ");
                u = bacaKomponen("  UTS       : ");
                a = bacaKomponen("  UAS       : ");

                na = hitungNA(h, t, u, a);
                konversiNilai(na, huruf, &bm);
                printf("\n  Nilai Akhir : %.2f | Huruf : %s | Bobot Mutu : %.1f (%s)\n\n",
                       na, huruf, bm, predikatNilai(huruf));

                if (konfirmasi("  Simpan nilai ini? (y/n) : ")) {
                    if (simpanNilai(nim[iM], kodeMatkul[iK], h, t, u, a)) {
                        hitungIP();
                        printf("\n  [OK] Nilai berhasil disimpan.\n");
                    } else {
                        printf("\n  [!] Nilai gagal disimpan.\n");
                    }
                } else {
                    printf("\n  Penyimpanan dibatalkan.\n");
                }
            }
        }
    } while (konfirmasi("\n  Input nilai lagi? (y/n) : "));
}

/* ================================================================== */
/*                  [31] TAMPILAN DAFTAR NILAI                        */
/* ================================================================== */

/* Tampilan Dosen: rekap seluruh data nilai */
void tampilNilai(void)
{
    int k, iM;

    bersihLayar();
    judul("DAFTAR NILAI SELURUH MAHASISWA");

    if (jumlahNilai == 0) {
        printf("  Belum ada data nilai.\n");
        jeda();
        return;
    }

    garis(79, '=');
    printf("%-3s %-12s %-15s %-7s %5s %5s %5s %5s %6s %-3s %3s\n",
           "No", "NIM", "Nama", "Kode", "Hadir", "Tugas", "UTS", "UAS", "NA", "Hrf", "BM");
    garis(79, '-');

    for (k = 0; k < jumlahNilai; k++) {
        iM = cariIndexMhs(nimNilai[k]);
        printf("%-3d %-12.12s %-15.15s %-7.7s %5.1f %5.1f %5.1f %5.1f %6.2f %-3s %3.1f\n",
               k + 1, nimNilai[k], (iM != -1) ? nama[iM] : "-", kodeNilai[k],
               kehadiran[k], tugas[k], uts[k], uas[k],
               nilaiAkhir[k], nilaiHuruf[k], bobot[k]);
    }
    garis(79, '=');
    printf("  Total data nilai : %d\n", jumlahNilai);
    jeda();
}

/* Komponen nilai milik 1 mahasiswa (idx = indeks di array mahasiswa).
 * Fungsi ini hanya mencetak tabel; pemanggil (menu) yang mengurus jeda(). */
void tampilNilaiMahasiswa(int idx)
{
    int k, m, no = 0;

    if (idx < 0 || idx >= jumlahMahasiswa) {
        printf("  [!] Data mahasiswa tidak ditemukan.\n");
        return;
    }

    printf("  NIM     : %s\n", nim[idx]);
    printf("  Nama    : %s\n", nama[idx]);
    printf("  Jurusan : %s (Angkatan %d)\n", jurusan[idx], angkatan[idx]);
    printf("  Bobot   : Hadir %d%% | Tugas %d%% | UTS %d%% | UAS %d%%\n\n",
           BOBOT_HADIR, BOBOT_TUGAS, BOBOT_UTS, BOBOT_UAS);

    garis(69, '=');
    printf("%-3s %-8s %-24s %3s %6s %6s %6s %6s\n",
           "No", "Kode", "Mata Kuliah", "SKS", "Hadir", "Tugas", "UTS", "UAS");
    garis(69, '-');

    for (k = 0; k < jumlahNilai; k++) {
        if (strcmp(nimNilai[k], nim[idx]) != 0) continue;
        m = cariIndexMatkul(kodeNilai[k]);
        no++;
        printf("%-3d %-8.8s %-24.24s %3d %6.1f %6.1f %6.1f %6.1f\n",
               no, kodeNilai[k], (m != -1) ? namaMatkul[m] : "-",
               (m != -1) ? sks[m] : 0,
               kehadiran[k], tugas[k], uts[k], uas[k]);
    }
    garis(69, '=');
    if (no == 0) printf("  Belum ada nilai yang diinput.\n");
}

/* Nilai akhir + huruf milik 1 mahasiswa. Hanya mencetak tabel (tanpa jeda). */
void tampilNilaiAkhirMahasiswa(int idx)
{
    int k, m, no = 0;

    if (idx < 0 || idx >= jumlahMahasiswa) {
        printf("  [!] Data mahasiswa tidak ditemukan.\n");
        return;
    }

    printf("  NIM     : %s\n", nim[idx]);
    printf("  Nama    : %s\n", nama[idx]);
    printf("  Jurusan : %s (Angkatan %d)\n\n", jurusan[idx], angkatan[idx]);

    garis(78, '=');
    printf("%-3s %-7s %-20s %3s %6s %-5s %4s %-11s %-11s\n",
           "No", "Kode", "Mata Kuliah", "SKS", "NA", "Huruf", "BM", "Predikat", "Status");
    garis(78, '-');

    for (k = 0; k < jumlahNilai; k++) {
        if (strcmp(nimNilai[k], nim[idx]) != 0) continue;
        m = cariIndexMatkul(kodeNilai[k]);
        no++;
        printf("%-3d %-7.7s %-20.20s %3d %6.2f %-5s %4.1f %-11s %-11s\n",
               no, kodeNilai[k], (m != -1) ? namaMatkul[m] : "-",
               (m != -1) ? sks[m] : 0,
               nilaiAkhir[k], nilaiHuruf[k], bobot[k],
               predikatNilai(nilaiHuruf[k]),
               (nilaiAkhir[k] >= NILAI_LULUS) ? "Lulus" : "Tidak Lulus");
    }
    garis(78, '=');
    if (no == 0) printf("  Belum ada nilai yang diinput.\n");
}

/* ================================================================== */
/*                       [32] EDIT NILAI KOMPONEN                     */
/* ================================================================== */
void editNilai(void)
{
    char  nimIn[20], kodeIn[15];
    int   iM, iK, k;
    float h, t, u, a;

    bersihLayar();
    judul("EDIT NILAI MAHASISWA");

    if (jumlahNilai == 0) {
        printf("  Belum ada data nilai yang bisa diedit.\n");
        jeda();
        return;
    }
    if (!pilihMhsMatkul(nimIn, kodeIn, &iM, &iK)) {
        jeda();
        return;
    }

    k = cariIndexNilai(nim[iM], kodeMatkul[iK]);
    if (k == -1) {
        printf("  [!] Belum ada nilai untuk mahasiswa & mata kuliah ini.\n");
        printf("      Gunakan menu Input Nilai terlebih dahulu.\n");
        jeda();
        return;
    }

    printf("  Nilai saat ini : Hadir %.1f | Tugas %.1f | UTS %.1f | UAS %.1f\n",
           kehadiran[k], tugas[k], uts[k], uas[k]);
    printf("  Nilai Akhir    : %.2f (%s)\n\n", nilaiAkhir[k], nilaiHuruf[k]);
    printf("  Masukkan nilai baru (Enter = pertahankan nilai lama)\n");

    h = bacaKomponenEdit("  Kehadiran : ", kehadiran[k]);
    t = bacaKomponenEdit("  Tugas     : ", tugas[k]);
    u = bacaKomponenEdit("  UTS       : ", uts[k]);
    a = bacaKomponenEdit("  UAS       : ", uas[k]);

    if (!konfirmasi("\n  Simpan perubahan? (y/n) : ")) {
        printf("\n  Perubahan dibatalkan.\n");
        jeda();
        return;
    }

    kehadiran[k] = h;
    tugas[k]     = t;
    uts[k]       = u;
    uas[k]       = a;
    hitungSatuNilai(k);
    hitungIP();

    printf("\n  [OK] Nilai berhasil diperbarui.\n");
    printf("  Nilai Akhir baru : %.2f | Huruf : %s | Bobot Mutu : %.1f\n",
           nilaiAkhir[k], nilaiHuruf[k], bobot[k]);
    jeda();
}

/* ================================================================== */
/*                       [33] HAPUS DATA PENILAIAN                    */
/* ================================================================== */

/* Hapus data nilai pada indeks k, lalu geser elemen di belakangnya ke depan.
 * Catatan untuk pemanggil yang menghapus dalam LOOP (mis. hapusMahasiswa,
 * hapusMatkul): telusuri dari indeks TERBESAR ke 0 agar pergeseran tidak
 * membuat data terlewat. */
void hapusNilaiIndex(int k)
{
    int i;

    if (k < 0 || k >= jumlahNilai) return;

    for (i = k; i < jumlahNilai - 1; i++) {
        strcpy(nimNilai[i],   nimNilai[i + 1]);
        strcpy(kodeNilai[i],  kodeNilai[i + 1]);
        kehadiran[i]  = kehadiran[i + 1];
        tugas[i]      = tugas[i + 1];
        uts[i]        = uts[i + 1];
        uas[i]        = uas[i + 1];
        nilaiAkhir[i] = nilaiAkhir[i + 1];
        strcpy(nilaiHuruf[i], nilaiHuruf[i + 1]);
        bobot[i]      = bobot[i + 1];
    }
    jumlahNilai--;
}

void hapusNilai(void)
{
    char nimIn[20], kodeIn[15];
    int  iM, iK, k;

    bersihLayar();
    judul("HAPUS DATA PENILAIAN");

    if (jumlahNilai == 0) {
        printf("  Belum ada data nilai yang bisa dihapus.\n");
        jeda();
        return;
    }
    if (!pilihMhsMatkul(nimIn, kodeIn, &iM, &iK)) {
        jeda();
        return;
    }

    k = cariIndexNilai(nim[iM], kodeMatkul[iK]);
    if (k == -1) {
        printf("  [!] Belum ada nilai untuk mahasiswa & mata kuliah ini.\n");
        jeda();
        return;
    }

    printf("  Hadir %.1f | Tugas %.1f | UTS %.1f | UAS %.1f\n",
           kehadiran[k], tugas[k], uts[k], uas[k]);
    printf("  Nilai Akhir %.2f (%s)\n\n", nilaiAkhir[k], nilaiHuruf[k]);

    if (konfirmasi("  Yakin hapus data penilaian ini? (y/n) : ")) {
        hapusNilaiIndex(k);
        hitungIP();
        printf("\n  [OK] Data penilaian berhasil dihapus.\n");
    } else {
        printf("\n  Penghapusan dibatalkan.\n");
    }
    jeda();
}
