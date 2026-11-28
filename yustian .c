#include <stdio.h>
#include <string.h>

#define MAX_MAHASISWA 20
#define MAX_PELAJARAN 20
#define JUMLAH_PERTEMUAN 20
#define PANJANG_NAMA 50

typedef struct
{
    char nama[PANJANG_NAMA];
    int hadir[JUMLAH_PERTEMUAN];  // 1 = hadir, 0 = tidak hadir
    int tugas;
    int uts;
    int uas;
} Pelajaran;

typedef struct
{
    int nomor;
    char nama[PANJANG_NAMA];
    int jumlahPelajaran;
    Pelajaran pelajaran[MAX_PELAJARAN];
} Mahasiswa;

Mahasiswa mahasiswa[MAX_MAHASISWA];
int jumlahMahasiswa = 3;

int inputRentang(char pesan[], int minimum, int maksimum)
{
    int angka;
    int hasilBaca;
    int karakter;

    do
    {
        printf("%s", pesan);
        hasilBaca = scanf("%d", &angka);

        if (hasilBaca != 1)
        {
            printf("Masukkan angka, bukan huruf.\n");

            while ((karakter = getchar()) != '\n' && karakter != EOF)
            {
                /* Buang karakter yang salah sampai akhir baris */
            }

            angka = minimum - 1;
        }
        else if (angka < minimum || angka > maksimum)
        {
            printf("Masukkan angka %d sampai %d.\n", minimum, maksimum);
        }
    } while (angka < minimum || angka > maksimum);

    return angka;
}

int cariMahasiswa(int nomor)
{
    int i;

    for (i = 0; i < jumlahMahasiswa; i++)
    {
        if (mahasiswa[i].nomor == nomor)
        {
            return i;
        }
    }

    return -1;
}

float hitungNilaiAkhir(Pelajaran p)
{
    int jumlahHadir = 0;
    int i;

    for (i = 0; i < JUMLAH_PERTEMUAN; i++)
    {
        jumlahHadir += p.hadir[i];
    }

    float nilaiKehadiran =
        ((float)jumlahHadir / JUMLAH_PERTEMUAN) * 10;

    return nilaiKehadiran
         + p.tugas * 0.20
         + p.uts * 0.30
         + p.uas * 0.40;
}

char *nilaiHuruf(float nilai)
{
    if (nilai >= 85) return "A";
    if (nilai >= 80) return "B+";
    if (nilai >= 75) return "B";
    if (nilai >= 70) return "C";
    return "E";
}

float hitungIP(float nilai)
{
    if (nilai >= 85) return 4.0;
    if (nilai >= 80) return 3.5;
    if (nilai >= 75) return 3.0;
    if (nilai >= 70) return 2.5;
    return 0.0;
}

void inputNilai()
{
    int nomor, indeks, p, i;

    printf("\n--- Input Nilai ---\n");
    printf("Daftar mahasiswa: 24 (Joko), 55 (You), 99 (Wiwo)\n");

    printf("Masukkan nomor mahasiswa: ");
    scanf("%d", &nomor);

    indeks = cariMahasiswa(nomor);

    if (indeks == -1)
    {
        printf("Nomor mahasiswa tidak ditemukan.\n");
        return;
    }

    if (mahasiswa[indeks].jumlahPelajaran >= MAX_PELAJARAN)
    {
        printf("Jumlah pelajaran sudah mencapai batas maksimal.\n");
        return;
    }

    p = mahasiswa[indeks].jumlahPelajaran;

    printf("Masukkan nama pelajaran (tanpa spasi): ");
    scanf("%49s", mahasiswa[indeks].pelajaran[p].nama);

    for (i = 0; i < JUMLAH_PERTEMUAN; i++)
    {
        char status;

        do
        {
            printf("Pertemuan %d, hadir atau tidak? (H/T): ", i + 1);
            scanf(" %c", &status);

            if (status != 'H' && status != 'h' &&
                status != 'T' && status != 't')
            {
                printf("Masukkan H untuk hadir atau T untuk tidak hadir.\n");
            }
        } while (status != 'H' && status != 'h' &&
                 status != 'T' && status != 't');

        mahasiswa[indeks].pelajaran[p].hadir[i] =
            (status == 'H' || status == 'h') ? 1 : 0;
    }

    mahasiswa[indeks].pelajaran[p].tugas =
        inputRentang("Nilai tugas (1-100): ", 1, 100);

    mahasiswa[indeks].pelajaran[p].uts =
        inputRentang("Nilai UTS (1-100): ", 1, 100);

    mahasiswa[indeks].pelajaran[p].uas =
        inputRentang("Nilai UAS (1-100): ", 1, 100);

    mahasiswa[indeks].jumlahPelajaran++;

    printf("Nilai pelajaran berhasil disimpan.\n");
}

void tampilkanNilai()
{
    int i, j, k, jumlahHadir;
    float total, ip;

    printf("\n--- Daftar Nilai Mahasiswa ---\n");

    for (i = 0; i < jumlahMahasiswa; i++)
    {
        printf("\nNomor: %d | Nama: %s\n",
               mahasiswa[i].nomor, mahasiswa[i].nama);

        if (mahasiswa[i].jumlahPelajaran == 0)
        {
            printf("Belum ada nilai yang dimasukkan.\n");
        }

        for (j = 0; j < mahasiswa[i].jumlahPelajaran; j++)
        {
            jumlahHadir = 0;

            for (k = 0; k < JUMLAH_PERTEMUAN; k++)
            {
                jumlahHadir += mahasiswa[i].pelajaran[j].hadir[k];
            }

            total = hitungNilaiAkhir(mahasiswa[i].pelajaran[j]);
            ip = hitungIP(total);

            printf("  Pelajaran: %s\n",
                   mahasiswa[i].pelajaran[j].nama);
            printf("  Kehadiran: %d/%d hadir\n",
                   jumlahHadir, JUMLAH_PERTEMUAN);
            printf("  Tugas: %d | UTS: %d | UAS: %d\n",
                   mahasiswa[i].pelajaran[j].tugas,
                   mahasiswa[i].pelajaran[j].uts,
                   mahasiswa[i].pelajaran[j].uas);
            printf("  Nilai akhir: %.2f | Huruf: %s | IP: %.1f\n",
                   total, nilaiHuruf(total), ip);
        }
    }
}

void editNilai()
{
    int nomor, indeks, pilihan, i;

    printf("\n--- Edit Nilai Mahasiswa ---\n");
    tampilkanNilai();

    printf("\nMasukkan nomor mahasiswa yang ingin diedit: ");
    scanf("%d", &nomor);

    indeks = cariMahasiswa(nomor);

    if (indeks == -1)
    {
        printf("Nomor mahasiswa tidak ditemukan.\n");
        return;
    }

    if (mahasiswa[indeks].jumlahPelajaran == 0)
    {
        printf("Mahasiswa ini belum memiliki data pelajaran.\n");
        return;
    }

    for (i = 0; i < mahasiswa[indeks].jumlahPelajaran; i++)
    {
        printf("%d. %s\n", i + 1, mahasiswa[indeks].pelajaran[i].nama);
    }

    pilihan = inputRentang("Pilih nomor pelajaran: ",
                           1, mahasiswa[indeks].jumlahPelajaran) - 1;

    printf("Edit nilai pelajaran %s:\n",
           mahasiswa[indeks].pelajaran[pilihan].nama);

    for (i = 0; i < JUMLAH_PERTEMUAN; i++)
    {
        char status;

        do
        {
            printf("Pertemuan %d, hadir atau tidak? (H/T): ", i + 1);
            scanf(" %c", &status);
        } while (status != 'H' && status != 'h' &&
                 status != 'T' && status != 't');

        mahasiswa[indeks].pelajaran[pilihan].hadir[i] =
            (status == 'H' || status == 'h') ? 1 : 0;
    }

    mahasiswa[indeks].pelajaran[pilihan].tugas =
        inputRentang("Nilai tugas (1-100): ", 1, 100);

    mahasiswa[indeks].pelajaran[pilihan].uts =
        inputRentang("Nilai UTS (1-100): ", 1, 100);

    mahasiswa[indeks].pelajaran[pilihan].uas =
        inputRentang("Nilai UAS (1-100): ", 1, 100);

    printf("Nilai berhasil diedit.\n");
}

void hapusPelajaran()
{
    int nomor, indeks, pilihan, i;

    tampilkanNilai();

    printf("\nMasukkan nomor mahasiswa: ");
    scanf("%d", &nomor);

    indeks = cariMahasiswa(nomor);

    if (indeks == -1 || mahasiswa[indeks].jumlahPelajaran == 0)
    {
        printf("Mahasiswa tidak ditemukan atau belum memiliki pelajaran.\n");
        return;
    }

    for (i = 0; i < mahasiswa[indeks].jumlahPelajaran; i++)
    {
        printf("%d. %s\n", i + 1, mahasiswa[indeks].pelajaran[i].nama);
    }

    pilihan = inputRentang("Pilih pelajaran yang akan dihapus: ",
                           1, mahasiswa[indeks].jumlahPelajaran) - 1;

    for (i = pilihan; i < mahasiswa[indeks].jumlahPelajaran - 1; i++)
    {
        mahasiswa[indeks].pelajaran[i] =
            mahasiswa[indeks].pelajaran[i + 1];
    }

    mahasiswa[indeks].jumlahPelajaran--;

    printf("Pelajaran berhasil dihapus.\n");
}

void tampilkanRanking()
{
    int i, j;
    float rataRata[MAX_MAHASISWA];
    int urutan[MAX_MAHASISWA];

    for (i = 0; i < jumlahMahasiswa; i++)
    {
        urutan[i] = i;
        rataRata[i] = 0;

        for (j = 0; j < mahasiswa[i].jumlahPelajaran; j++)
        {
            rataRata[i] += hitungNilaiAkhir(mahasiswa[i].pelajaran[j]);
        }

        if (mahasiswa[i].jumlahPelajaran > 0)
        {
            rataRata[i] /= mahasiswa[i].jumlahPelajaran;
        }
    }

    for (i = 0; i < jumlahMahasiswa - 1; i++)
    {
        for (j = i + 1; j < jumlahMahasiswa; j++)
        {
            if (rataRata[urutan[j]] > rataRata[urutan[i]])
            {
                int sementara = urutan[i];
                urutan[i] = urutan[j];
                urutan[j] = sementara;
            }
        }
    }

    printf("\n--- Ranking Mahasiswa berdasarkan rata-rata nilai akhir ---\n");

    for (i = 0; i < jumlahMahasiswa; i++)
    {
        int x = urutan[i];

        printf("%d. %s (No. %d) - ",
               i + 1, mahasiswa[x].nama, mahasiswa[x].nomor);

        if (mahasiswa[x].jumlahPelajaran == 0)
        {
            printf("Belum ada nilai\n");
        }
        else
        {
            printf("Rata-rata %.2f\n", rataRata[x]);
        }
    }
}

int main()
{
    int pilihan;
    //tes data mahasiswa

    mahasiswa[0].nomor = 24;
    strcpy(mahasiswa[0].nama, "Joko");
    mahasiswa[0].jumlahPelajaran = 0;

    mahasiswa[1].nomor = 55;
    strcpy(mahasiswa[1].nama, "You");
    mahasiswa[1].jumlahPelajaran = 0;

    mahasiswa[2].nomor = 99;
    strcpy(mahasiswa[2].nama, "Wiwo");
    mahasiswa[2].jumlahPelajaran = 0;

    //menu pilian dosenna 
    do
    {
        printf("\n===== MENU DOSEN =====\n");
        printf("1. Input nilai mahasiswa\n");
        printf("2. Lihat daftar nilai mahasiswa\n");
        printf("3. Edit nilai mahasiswa\n");
        printf("4. Hapus salah satu pelajaran\n");
        printf("0. Keluar\n");
        pilihan = inputRentang("Pilih menu (0-5): ", 0, 4);

        switch (pilihan)
        {
            case 1:
                inputNilai();
                break;
            case 2:
                tampilkanNilai();
                tampilkanRanking();
                break;
            case 3:
                editNilai();
                break;
            case 4:
                hapusPelajaran();
                break;

            case 0:
                printf("Program selesai.\n");
                break;

        }
    } while (pilihan != 0);

    return 0;
}