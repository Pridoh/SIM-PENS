/*
 * laporan.c - Modul Laporan (Anggota 3)
 * Tugas 34 - 36
 *
 *  34. hitungIP(), tampilIPMahasiswa()   -> Indeks Prestasi semester berbasis SKS
 *  35. rankingMahasiswa()                -> ranking berdasarkan IP / nilai per mata kuliah
 *  36. laporanNilai()                    -> rekapitulasi & laporan hasil penilaian akademik
 *
 *  IP = SUM(Bobot Mutu x SKS) / SUM(SKS)
 */
#include <stdio.h>
#include <string.h>
#include "data.h"

static const char *DAFTAR_HURUF[7] = { "A", "B+", "B", "C+", "C", "D", "E" };
static const float BOBOT_HURUF[7]  = { 4.0f, 3.5f, 3.0f, 2.5f, 2.0f, 1.0f, 0.0f };

/* ================================================================== */
/*                      FUNGSI BANTU (internal)                       */
/* ================================================================== */

/* Dua float dianggap sama jika selisihnya sangat kecil */
static int sama(float a, float b)
{
    float d = a - b;
    if (d < 0) d = -d;
    return d < 0.001f;
}

/* a < b dengan memperhitungkan toleransi float */
static int lebihKecil(float a, float b)
{
    return !sama(a, b) && a < b;
}

/* Banyaknya data nilai milik satu mahasiswa */
static int jumlahNilaiMhs(const char nimCari[])
{
    int k, n = 0;
    for (k = 0; k < jumlahNilai; k++)
        if (strcmp(nimNilai[k], nimCari) == 0) n++;
    return n;
}

/* [34] Hitung IP satu mahasiswa langsung dari array nilai.
 * totalSKS diisi total SKS yang dihitung. IP dibulatkan 2 desimal. */
static float hitungIPSatu(const char nimCari[], int *totalSKS)
{
    int   k, m, totSks = 0;
    float totMutu = 0.0f, ip;

    for (k = 0; k < jumlahNilai; k++) {
        if (strcmp(nimNilai[k], nimCari) != 0) continue;
        m = cariIndexMatkul(kodeNilai[k]);
        if (m == -1) continue;               /* mata kuliah sudah tidak ada */
        totMutu += bobot[k] * sks[m];
        totSks  += sks[m];
    }

    *totalSKS = totSks;
    if (totSks == 0) return 0.0f;

    ip = totMutu / totSks;
    return (float)((int)(ip * 100.0f + 0.5f)) / 100.0f;
}

/* Rata-rata nilai akhir satu mahasiswa (dipakai sebagai pemisah saat IP sama) */
static float rataNilaiMhs(const char nimCari[])
{
    int   k, n = 0;
    float total = 0.0f;

    for (k = 0; k < jumlahNilai; k++) {
        if (strcmp(nimNilai[k], nimCari) != 0) continue;
        total += nilaiAkhir[k];
        n++;
    }
    return (n > 0) ? total / n : 0.0f;
}

/* ================================================================== */
/*                [34] INDEKS PRESTASI (IP) SEMESTER                  */
/* ================================================================== */

/* Isi ipMhs[] untuk semua mahasiswa */
void hitungIP(void)
{
    int i, totSks;
    for (i = 0; i < jumlahMahasiswa; i++)
        ipMhs[i] = hitungIPSatu(nim[i], &totSks);
}

/* Tampilkan IP satu mahasiswa (hanya mencetak, tanpa jeda) */
void tampilIPMahasiswa(int idx)
{
    int totSks;

    if (idx < 0 || idx >= jumlahMahasiswa) {
        printf("  [!] Data mahasiswa tidak ditemukan.\n");
        return;
    }

    ipMhs[idx] = hitungIPSatu(nim[idx], &totSks);

    printf("\n  Jumlah mata kuliah : %d\n", jumlahNilaiMhs(nim[idx]));
    printf("  Total SKS          : %d\n", totSks);
    printf("  IP Semester        : %.2f  (skala 4.00)\n", ipMhs[idx]);
    printf("  Rumus              : IP = SUM(Bobot Mutu x SKS) / SUM(SKS)\n");
}

/* ================================================================== */
/*                       [35] RANKING MAHASISWA                       */
/* ================================================================== */

/* Ranking berdasarkan IP. Urutan: IP terbesar dulu; bila IP sama, rata-rata
 * nilai akhir terbesar dulu. Array master TIDAK diubah urutannya: yang
 * diurutkan hanyalah array indeks urut[]. Hanya mahasiswa yang sudah punya
 * nilai yang diikutkan. */
static void rankingIP(void)
{
    int   urut[MAX_MHS];
    float rata[MAX_MHS];
    int   n = 0, i, j, a, b, tmp, rank = 0, totSks;

    bersihLayar();
    judul("RANKING MAHASISWA BERDASARKAN IP");
    hitungIP();

    for (i = 0; i < jumlahMahasiswa; i++) {
        rata[i] = rataNilaiMhs(nim[i]);
        if (jumlahNilaiMhs(nim[i]) > 0) urut[n++] = i;
    }

    if (n == 0) {
        printf("  Belum ada data nilai untuk diranking.\n");
        jeda();
        return;
    }

    /* Bubble sort pada array indeks (descending) */
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            a = urut[j];
            b = urut[j + 1];
            if (lebihKecil(ipMhs[a], ipMhs[b]) ||
                (sama(ipMhs[a], ipMhs[b]) && lebihKecil(rata[a], rata[b]))) {
                tmp = urut[j];
                urut[j] = urut[j + 1];
                urut[j + 1] = tmp;
            }
        }
    }

    garis(59, '=');
    printf("%-4s %-12s %-22s %4s %7s %5s\n",
           "Rank", "NIM", "Nama", "SKS", "Rata-NA", "IP");
    garis(59, '-');

    for (i = 0; i < n; i++) {
        a = urut[i];
        if (i == 0) {
            rank = 1;
        } else {
            b = urut[i - 1];
            /* IP dan rata-rata sama persis -> peringkat sama */
            if (!(sama(ipMhs[a], ipMhs[b]) && sama(rata[a], rata[b])))
                rank = i + 1;
        }
        hitungIPSatu(nim[a], &totSks);
        printf("%-4d %-12.12s %-22.22s %4d %7.2f %5.2f\n",
               rank, nim[a], nama[a], totSks, rata[a], ipMhs[a]);
    }
    garis(59, '=');
    printf("  Jumlah mahasiswa diranking : %d\n", n);
    jeda();
}

/* Ranking nilai akhir pada satu mata kuliah */
static void rankingMatkul(void)
{
    char  kode[15];
    int   urut[MAX_NILAI];
    int   n = 0, k, m, i, j, a, b, tmp, rank = 0, iM;

    bersihLayar();
    judul("RANKING NILAI PER MATA KULIAH");

    if (jumlahMatkul == 0) {
        printf("  Belum ada data mata kuliah.\n");
        jeda();
        return;
    }

    bacaStringWajib("  Kode Mata Kuliah : ", kode, 15);
    keHurufBesar(kode);
    m = cariIndexMatkul(kode);
    if (m == -1) {
        printf("\n  [!] Kode mata kuliah %s tidak ditemukan.\n", kode);
        jeda();
        return;
    }

    for (k = 0; k < jumlahNilai; k++)
        if (strcmp(kodeNilai[k], kodeMatkul[m]) == 0) urut[n++] = k;

    printf("\n  Mata Kuliah : %s - %s (%d SKS)\n\n", kodeMatkul[m], namaMatkul[m], sks[m]);

    if (n == 0) {
        printf("  Belum ada nilai pada mata kuliah ini.\n");
        jeda();
        return;
    }

    /* Bubble sort descending berdasarkan nilai akhir */
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            a = urut[j];
            b = urut[j + 1];
            if (lebihKecil(nilaiAkhir[a], nilaiAkhir[b])) {
                tmp = urut[j];
                urut[j] = urut[j + 1];
                urut[j + 1] = tmp;
            }
        }
    }

    garis(54, '=');
    printf("%-4s %-12s %-22s %7s %-5s\n", "Rank", "NIM", "Nama", "NA", "Huruf");
    garis(54, '-');

    for (i = 0; i < n; i++) {
        a = urut[i];
        if (i == 0) {
            rank = 1;
        } else if (!sama(nilaiAkhir[a], nilaiAkhir[urut[i - 1]])) {
            rank = i + 1;
        }
        iM = cariIndexMhs(nimNilai[a]);
        printf("%-4d %-12.12s %-22.22s %7.2f %-5s\n",
               rank, nimNilai[a], (iM != -1) ? nama[iM] : "-",
               nilaiAkhir[a], nilaiHuruf[a]);
    }
    garis(54, '=');
    printf("  Jumlah peserta : %d\n", n);
    jeda();
}

void rankingMahasiswa(void)
{
    int pilih;

    do {
        bersihLayar();
        judul("RANKING MAHASISWA");
        printf("  1. Ranking berdasarkan IP semester\n");
        printf("  2. Ranking nilai akhir per mata kuliah\n");
        printf("  0. Kembali\n");
        garis(40, '-');
        pilih = bacaInt("  Pilih menu : ", 0, 2);

        switch (pilih) {
            case 1: rankingIP();     break;
            case 2: rankingMatkul(); break;
            default: break;
        }
    } while (pilih != 0);
}

/* ================================================================== */
/*                  [36] REKAPITULASI & LAPORAN                       */
/* ================================================================== */

/* Laporan 1: rekap statistik per mata kuliah */
static void laporanPerMatkul(void)
{
    int   m, k, peserta, lulus;
    float total, nilaiMaks, nilaiMin, na;

    bersihLayar();
    judul("REKAP NILAI PER MATA KULIAH");

    if (jumlahMatkul == 0) {
        printf("  Belum ada data mata kuliah.\n");
        jeda();
        return;
    }

    garis(79, '=');
    printf("%-8s %-24s %3s %7s %7s %6s %6s %5s %5s\n",
           "Kode", "Mata Kuliah", "SKS", "Peserta", "Rata2", "Maks", "Min", "Lulus", "Gagal");
    garis(79, '-');

    for (m = 0; m < jumlahMatkul; m++) {
        peserta = 0;
        lulus = 0;
        total = 0.0f;
        nilaiMaks = 0.0f;
        nilaiMin = 100.0f;

        for (k = 0; k < jumlahNilai; k++) {
            if (strcmp(kodeNilai[k], kodeMatkul[m]) != 0) continue;
            na = nilaiAkhir[k];
            peserta++;
            total += na;
            if (na > nilaiMaks) nilaiMaks = na;
            if (na < nilaiMin)  nilaiMin = na;
            if (na >= NILAI_LULUS) lulus++;
        }

        if (peserta == 0) {
            printf("%-8.8s %-24.24s %3d %7d %7s %6s %6s %5s %5s\n",
                   kodeMatkul[m], namaMatkul[m], sks[m], 0, "-", "-", "-", "-", "-");
        } else {
            printf("%-8.8s %-24.24s %3d %7d %7.2f %6.2f %6.2f %5d %5d\n",
                   kodeMatkul[m], namaMatkul[m], sks[m], peserta,
                   total / peserta, nilaiMaks, nilaiMin, lulus, peserta - lulus);
        }
    }
    garis(79, '=');
    printf("  Lulus = nilai akhir >= %.0f\n", NILAI_LULUS);
    jeda();
}

/* Laporan 2: rekap IP seluruh mahasiswa */
static void rekapIPSemua(void)
{
    int i, totSks, jmk;

    bersihLayar();
    judul("REKAP IP SELURUH MAHASISWA");

    if (jumlahMahasiswa == 0) {
        printf("  Belum ada data mahasiswa.\n");
        jeda();
        return;
    }

    hitungIP();

    garis(72, '=');
    printf("%-3s %-12s %-20s %-14s %4s %3s %4s %5s\n",
           "No", "NIM", "Nama", "Jurusan", "Akt.", "MK", "SKS", "IP");
    garis(72, '-');

    for (i = 0; i < jumlahMahasiswa; i++) {
        jmk = jumlahNilaiMhs(nim[i]);
        hitungIPSatu(nim[i], &totSks);
        if (jmk == 0) {
            printf("%-3d %-12.12s %-20.20s %-14.14s %4d %3d %4d %5s\n",
                   i + 1, nim[i], nama[i], jurusan[i], angkatan[i], 0, 0, "-");
        } else {
            printf("%-3d %-12.12s %-20.20s %-14.14s %4d %3d %4d %5.2f\n",
                   i + 1, nim[i], nama[i], jurusan[i], angkatan[i], jmk, totSks, ipMhs[i]);
        }
    }
    garis(72, '=');
    printf("  Total mahasiswa : %d\n", jumlahMahasiswa);
    jeda();
}

/* Laporan 3: transkrip nilai satu mahasiswa */
static void transkripMahasiswa(void)
{
    char nimIn[20];
    int  idx;

    bersihLayar();
    judul("TRANSKRIP NILAI MAHASISWA");

    if (jumlahMahasiswa == 0) {
        printf("  Belum ada data mahasiswa.\n");
        jeda();
        return;
    }

    bacaStringWajib("  NIM Mahasiswa : ", nimIn, 20);
    idx = cariIndexMhs(nimIn);
    if (idx == -1) {
        printf("\n  [!] NIM %s tidak ditemukan.\n", nimIn);
        jeda();
        return;
    }

    printf("\n");
    tampilNilaiAkhirMahasiswa(idx);
    tampilIPMahasiswa(idx);
    jeda();
}

/* Laporan 4: statistik umum + distribusi nilai huruf */
static void statistikUmum(void)
{
    int   i, k, h, b, cnt, adaMhs = 0, lulus = 0, iTinggi = -1, iRendah = -1;
    float totalNA = 0.0f, totalIP = 0.0f;

    bersihLayar();
    judul("STATISTIK UMUM PENILAIAN AKADEMIK");

    if (jumlahNilai == 0) {
        printf("  Belum ada data nilai.\n");
        jeda();
        return;
    }

    hitungIP();

    for (i = 0; i < jumlahMahasiswa; i++) {
        if (jumlahNilaiMhs(nim[i]) == 0) continue;
        adaMhs++;
        totalIP += ipMhs[i];
        if (iTinggi == -1 || ipMhs[i] > ipMhs[iTinggi]) iTinggi = i;
        if (iRendah == -1 || ipMhs[i] < ipMhs[iRendah]) iRendah = i;
    }

    for (k = 0; k < jumlahNilai; k++) {
        totalNA += nilaiAkhir[k];
        if (nilaiAkhir[k] >= NILAI_LULUS) lulus++;
    }

    printf("  Jumlah mahasiswa               : %d (%d sudah punya nilai)\n", jumlahMahasiswa, adaMhs);
    printf("  Jumlah mata kuliah             : %d\n", jumlahMatkul);
    printf("  Jumlah data nilai              : %d\n", jumlahNilai);
    printf("  Rata-rata nilai akhir          : %.2f\n", totalNA / jumlahNilai);
    printf("  Persentase kelulusan nilai     : %.1f%% (%d dari %d data, batas lulus %.0f)\n",
           100.0f * lulus / jumlahNilai, lulus, jumlahNilai, NILAI_LULUS);

    if (adaMhs > 0) {
        printf("  Rata-rata IP                   : %.2f\n", totalIP / adaMhs);
        printf("  IP tertinggi                   : %.2f (%s - %s)\n",
               ipMhs[iTinggi], nim[iTinggi], nama[iTinggi]);
        printf("  IP terendah                    : %.2f (%s - %s)\n",
               ipMhs[iRendah], nim[iRendah], nama[iRendah]);
    }

    printf("\n  Distribusi nilai huruf\n");
    garis(60, '-');
    for (h = 0; h < 7; h++) {
        cnt = 0;
        for (k = 0; k < jumlahNilai; k++)
            if (strcmp(nilaiHuruf[k], DAFTAR_HURUF[h]) == 0) cnt++;

        printf("  %-2s (%.1f) : %3d  ", DAFTAR_HURUF[h], BOBOT_HURUF[h], cnt);
        for (b = 0; b < cnt && b < 40; b++) putchar('*');
        putchar('\n');
    }
    garis(60, '-');
    jeda();
}

/* Menu utama laporan */
void laporanNilai(void)
{
    int pilih;

    do {
        bersihLayar();
        judul("LAPORAN HASIL PENILAIAN AKADEMIK");
        printf("  1. Rekap nilai per mata kuliah\n");
        printf("  2. Rekap IP seluruh mahasiswa\n");
        printf("  3. Transkrip nilai per mahasiswa\n");
        printf("  4. Statistik umum & distribusi nilai huruf\n");
        printf("  0. Kembali\n");
        garis(40, '-');
        pilih = bacaInt("  Pilih menu : ", 0, 4);

        switch (pilih) {
            case 1: laporanPerMatkul();    break;
            case 2: rekapIPSemua();        break;
            case 3: transkripMahasiswa();  break;
            case 4: statistikUmum();       break;
            default: break;
        }
    } while (pilih != 0);
}
