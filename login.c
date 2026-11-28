/*
 * login.c - Autentikasi, blokir akun, dan menu dashboard (Anggota 1)
 */
#include <stdio.h>
#include <string.h>
#include "data.h"

/* ===================== MANAJEMEN AKUN ===================== */
int cariIndexUser(const char user[]) {
    int i;
    for (i = 0; i < jumlahUser; i++) {
        if (strcmp(username[i], user) == 0) {
            return i;
        }
    }
    return -1;
}

int tambahAkun(const char user[], const char pass[], const char peran[]) {
    if (jumlahUser >= MAX_USER || cariIndexUser(user) != -1) {
        return 0;
    }
    strcpy(username[jumlahUser], user);
    strcpy(password[jumlahUser], pass);
    strcpy(role[jumlahUser], peran);
    statusBlokir[jumlahUser] = 0;
    gagalLogin[jumlahUser] = 0;
    jumlahUser++;
    return 1;
}

void hapusAkun(const char user[]) {
    int idx = cariIndexUser(user);
    int i;

    if (idx == -1) {
        return;
    }
    for (i = idx; i < jumlahUser - 1; i++) {
        strcpy(username[i], username[i + 1]);
        strcpy(password[i], password[i + 1]);
        strcpy(role[i], role[i + 1]);
        statusBlokir[i] = statusBlokir[i + 1];
        gagalLogin[i] = gagalLogin[i + 1];
    }
    jumlahUser--;
    if (userAktif > idx) {
        userAktif--;
    }
}

/* ===================== LOGIN ===================== */
void login(void) {
    char user[30], pass[30];
    int percobaan = 0;
    int idx;

    bersihLayar();
    judul("LOGIN");
    printf("  (Maksimal %d kali percobaan)\n\n", MAX_GAGAL_LOGIN);

    while (percobaan < MAX_GAGAL_LOGIN) {
        bacaStringWajib("Username : ", user, 30);
        bacaStringWajib("Password : ", pass, 30);
        idx = cariIndexUser(user);

        if (idx != -1 && statusBlokir[idx] == 1) {
            printf("\n[!] Akun \"%s\" sedang DIBLOKIR. Hubungi dosen untuk membuka blokir.\n", user);
            jeda();
            return;
        }

        if (idx != -1 && strcmp(password[idx], pass) == 0) {
            gagalLogin[idx] = 0;
            userAktif = idx;
            printf("\nLogin berhasil. Selamat datang, %s!\n", username[idx]);
            jeda();

            if (strcmp(role[idx], "dosen") == 0) {
                menuDosen();
            } else {
                menuMahasiswa();
            }
            userAktif = -1;
            return;
        }

        /* login gagal */
        percobaan++;
        if (idx != -1) {
            gagalLogin[idx]++;
            if (gagalLogin[idx] >= MAX_GAGAL_LOGIN) {
                statusBlokir[idx] = 1;
                printf("\n[!] Gagal login %d kali berturut-turut. Akun \"%s\" DIBLOKIR.\n",
                       MAX_GAGAL_LOGIN, user);
                jeda();
                return;
            }
        }
        printf("\n[!] Username atau password salah. Sisa percobaan: %d\n\n",
               MAX_GAGAL_LOGIN - percobaan);
    }

    printf("[!] Percobaan login habis. Kembali ke halaman awal.\n");
    jeda();
}

/* ===================== BUKA BLOKIR (DOSEN) ===================== */
static void bukaBlokir(void) {
    char user[30];
    int i, idx, ada = 0;

    bersihLayar();
    judul("BUKA BLOKIR AKUN");
    printf("Daftar akun terblokir:\n");
    for (i = 0; i < jumlahUser; i++) {
        if (statusBlokir[i] == 1) {
            printf("  - %s (%s)\n", username[i], role[i]);
            ada = 1;
        }
    }
    if (!ada) {
        printf("  (tidak ada akun yang terblokir)\n");
        jeda();
        return;
    }

    bacaString("\nUsername yang dibuka blokirnya (Enter = batal): ", user, 30);
    if (user[0] == '\0') {
        return;
    }
    idx = cariIndexUser(user);
    if (idx == -1 || statusBlokir[idx] == 0) {
        printf("[!] Akun tidak ditemukan atau tidak sedang diblokir.\n");
    } else {
        statusBlokir[idx] = 0;
        gagalLogin[idx] = 0;
        printf("Blokir akun \"%s\" berhasil dibuka.\n", user);
    }
    jeda();
}

/* ===================== SUB MENU DATA ===================== */
static void menuDataMahasiswa(void) {
    int pilih;
    do {
        bersihLayar();
        judul("DATA MAHASISWA");
        printf("  1. Tambah Mahasiswa\n");
        printf("  2. Tampil Semua Mahasiswa\n");
        printf("  3. Cari Mahasiswa\n");
        printf("  4. Ubah Data Mahasiswa\n");
        printf("  5. Hapus Mahasiswa\n");
        printf("  0. Kembali\n\n");
        pilih = bacaInt("Pilih menu: ", 0, 5);

        switch (pilih) {
            case 1: tambahMahasiswa(); break;
            case 2: bersihLayar(); judul("DAFTAR MAHASISWA"); tampilMahasiswa(); jeda(); break;
            case 3: cariMahasiswa(); break;
            case 4: ubahMahasiswa(); break;
            case 5: hapusMahasiswa(); break;
        }
    } while (pilih != 0);
}

static void menuDataMatkul(void) {
    int pilih;
    do {
        bersihLayar();
        judul("DATA MATA KULIAH");
        printf("  1. Tambah Mata Kuliah\n");
        printf("  2. Tampil Semua Mata Kuliah\n");
        printf("  3. Ubah Mata Kuliah\n");
        printf("  4. Hapus Mata Kuliah\n");
        printf("  0. Kembali\n\n");
        pilih = bacaInt("Pilih menu: ", 0, 4);

        switch (pilih) {
            case 1: tambahMatkul(); break;
            case 2: bersihLayar(); judul("DAFTAR MATA KULIAH"); tampilMatkul(); jeda(); break;
            case 3: ubahMatkul(); break;
            case 4: hapusMatkul(); break;
        }
    } while (pilih != 0);
}

/* ===================== MENU DOSEN ===================== */
static void dashboardDosen(void) {
    int i, sudahDinilai = 0, terblokir = 0;
    float total = 0;

    for (i = 0; i < jumlahUser; i++) {
        if (statusBlokir[i] == 1) {
            terblokir++;
        }
    }
    /* hitung mahasiswa yang punya minimal 1 nilai */
    for (i = 0; i < jumlahMahasiswa; i++) {
        int k, punya = 0;
        for (k = 0; k < jumlahNilai; k++) {
            if (strcmp(nimNilai[k], nim[i]) == 0) {
                punya = 1;
                break;
            }
        }
        sudahDinilai += punya;
    }
    for (i = 0; i < jumlahNilai; i++) {
        total += nilaiAkhir[i];
    }

    printf("  Ringkasan Data Akademik\n");
    garis(60, '-');
    printf("  Jumlah mahasiswa          : %d\n", jumlahMahasiswa);
    printf("  Jumlah mata kuliah        : %d\n", jumlahMatkul);
    printf("  Data nilai tersimpan      : %d\n", jumlahNilai);
    printf("  Mahasiswa sudah dinilai   : %d dari %d\n", sudahDinilai, jumlahMahasiswa);
    if (jumlahNilai > 0) {
        printf("  Rata-rata nilai akhir     : %.2f\n", total / jumlahNilai);
    } else {
        printf("  Rata-rata nilai akhir     : -\n");
    }
    printf("  Akun terblokir            : %d\n", terblokir);
    garis(60, '-');
}

void menuDosen(void) {
    int pilih;

    do {
        hitungNilai();
        bersihLayar();
        judul("DASHBOARD DOSEN");
        dashboardDosen();
        printf("\n");
        printf("  1. Data Mahasiswa\n");
        printf("  2. Data Mata Kuliah\n");
        printf("  3. Input Nilai\n");
        printf("  4. Edit Nilai\n");
        printf("  5. Hapus Nilai\n");
        printf("  6. Rekap Nilai\n");
        printf("  7. Ranking Mahasiswa\n");
        printf("  8. Laporan Nilai\n");
        printf("  9. Buka Blokir Akun\n");
        printf("  0. Logout\n\n");
        pilih = bacaInt("Pilih menu: ", 0, 9);

        switch (pilih) {
            case 1: menuDataMahasiswa(); break;
            case 2: menuDataMatkul(); break;
            case 3: inputNilai(); break;
            case 4: editNilai(); break;
            case 5: hapusNilai(); break;
            case 6:
                bersihLayar(); judul("REKAP NILAI");
                tampilNilai();
                jeda();
                break;
            case 7:
                bersihLayar(); judul("RANKING MAHASISWA");
                rankingMahasiswa();
                jeda();
                break;
            case 8:
                bersihLayar(); judul("LAPORAN HASIL PENILAIAN");
                laporanNilai();
                jeda();
                break;
            case 9: bukaBlokir(); break;
            case 0:
                printf("\nLogout berhasil.\n");
                jeda();
                break;
        }
    } while (pilih != 0);
}

/* ===================== MENU MAHASISWA ===================== */
void menuMahasiswa(void) {
    int pilih;
    int idx = cariIndexMhs(username[userAktif]);   /* username mahasiswa = NIM */
    int i, jumlahMk;
    int totalSKS;

    if (idx == -1) {
        printf("[!] Data mahasiswa untuk akun ini tidak ditemukan.\n");
        jeda();
        return;
    }

    do {
        hitungNilai();
        hitungIP();

        jumlahMk = 0;
        totalSKS = 0;
        for (i = 0; i < jumlahNilai; i++) {
            if (strcmp(nimNilai[i], nim[idx]) == 0) {
                jumlahMk++;
                if (cariIndexMatkul(kodeNilai[i]) != -1) {
                    totalSKS += sks[cariIndexMatkul(kodeNilai[i])];
                }
            }
        }

        bersihLayar();
        judul("DASHBOARD MAHASISWA");
        printf("  Nama                  : %s\n", nama[idx]);
        printf("  NIM                   : %s\n", nim[idx]);
        printf("  Mata kuliah dinilai   : %d dari %d\n", jumlahMk, jumlahMatkul);
        printf("  Total SKS dinilai     : %d\n", totalSKS);
        if (totalSKS > 0) {
            printf("  IP Semester           : %.2f\n", ipMhs[idx]);
        } else {
            printf("  IP Semester           : - (belum ada nilai)\n");
        }
        garis(60, '-');
        printf("\n");
        printf("  1. Profil\n");
        printf("  2. Mata Kuliah\n");
        printf("  3. Nilai (rincian komponen)\n");
        printf("  4. Nilai Akhir\n");
        printf("  5. Indeks Prestasi\n");
        printf("  0. Logout\n\n");
        pilih = bacaInt("Pilih menu: ", 0, 5);

        switch (pilih) {
            case 1: bersihLayar(); judul("PROFIL MAHASISWA"); profilMahasiswa(idx); jeda(); break;
            case 2: bersihLayar(); judul("DAFTAR MATA KULIAH"); tampilMatkul(); jeda(); break;
            case 3: bersihLayar(); judul("RINCIAN NILAI KOMPONEN"); tampilNilaiMahasiswa(idx); jeda(); break;
            case 4: bersihLayar(); judul("NILAI AKHIR"); tampilNilaiAkhirMahasiswa(idx); jeda(); break;
            case 5: bersihLayar(); judul("INDEKS PRESTASI SEMESTER"); tampilIPMahasiswa(idx); jeda(); break;
            case 0:
                printf("\nLogout berhasil.\n");
                jeda();
                break;
        }
    } while (pilih != 0);
}