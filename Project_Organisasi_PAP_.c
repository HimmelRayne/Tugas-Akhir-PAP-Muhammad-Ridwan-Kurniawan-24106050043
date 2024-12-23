#include <stdio.h> // Header file untuk fungsi input/output seperti printf dan scanf
#include <stdlib.h> // Header file untuk fungsi manajemen memori seperti malloc dan realloc
#include <string.h> // Header file untuk manipulasi string seperti strcpy

#define ANGGOTA_AWAL 15 // Jumlah anggota awal

// Enum untuk jenis transaksi
typedef enum {
    PEMASUKKAN, // Pemasukan
    PENGELUARAN // Pengeluaran
} TipeTransaksi;

// Struct untuk transaksi
typedef struct {
    char deskripsi[50]; // Deskripsi transaksi
    float jumlah;         // Jumlah uang
    TipeTransaksi tipe; // Jenis transaksi (PEMASUKKAN atau PENGELUARAN)
} Transaksi;

// Union untuk data anggota atau transaksi
typedef union {
    char namaAnggota[30];   // Nama anggota
    Transaksi transaksi; // Data transaksi
} Data;

// Struct untuk mencatat anggota
typedef struct {
    char nama[30];  // Nama anggota
    float saldo;  // Saldo anggota
} Anggota;

// Fungsi untuk menambahkan transaksi
void tambahTransaksi(Anggota *anggota, Transaksi *transaksi, int *jumlahTransaksi, int jumlahAnggota) {
    Transaksi *transaksiBaru = (Transaksi *)malloc(sizeof(Transaksi)); // Alokasi memori untuk transaksi baru

    printf("\nMasukkan deskripsi transaksi: ");
    scanf(" %s", transaksiBaru->deskripsi); // Input deskripsi transaksi
    printf("Masukkan jumlah transaksi: ");
    scanf("%f", &transaksiBaru->jumlah); // Input jumlah transaksi

    int tipe;
    printf("Jenis transaksi (0 untuk pemasukan, 1 untuk pengeluaran): ");
    scanf("%d", &tipe); // Input jenis transaksi

    if (tipe == 0) {
        transaksiBaru->tipe = PEMASUKKAN; // Set jenis transaksi ke PEMASUKKAN
    } else if (tipe == 1) {
        transaksiBaru->tipe = PENGELUARAN; // Set jenis transaksi ke PENGELUARAN
    } else {
        printf("Jenis transaksi tidak valid!\n");
        free(transaksiBaru); // Bebaskan memori jika jenis tidak valid
        return;
    }

    printf("Pilih anggota (0-%d): ", jumlahAnggota - 1);
    int indeksAnggota;
    scanf("%d", &indeksAnggota); // Pilih anggota untuk transaksi

    if (indeksAnggota < 0 || indeksAnggota >= jumlahAnggota) {
        printf("Anggota tidak valid!\n");
        free(transaksiBaru); // Bebaskan memori jika anggota tidak valid
        return;
    }

    if (transaksiBaru->tipe == PEMASUKKAN) {
        anggota[indeksAnggota].saldo += transaksiBaru->jumlah; // Tambahkan saldo untuk PEMASUKKAN
    } else if (transaksiBaru->tipe == PENGELUARAN) {
        if (anggota[indeksAnggota].saldo >= transaksiBaru->jumlah) {
            anggota[indeksAnggota].saldo -= transaksiBaru->jumlah; // Kurangi saldo untuk PENGELUARAN
        } else {
            printf("Saldo tidak mencukupi untuk pengeluaran ini!\n");
            free(transaksiBaru); // Bebaskan memori jika saldo tidak cukup
            return;
        }
    }

    transaksi[*jumlahTransaksi] = *transaksiBaru; // Simpan transaksi ke daftar transaksi
    (*jumlahTransaksi)++; // Tambahkan jumlah transaksi
    free(transaksiBaru); // Bebaskan memori transaksi sementara

    printf("Transaksi berhasil ditambahkan!\n");
}

// Fungsi untuk menampilkan data anggota
void tampilkanAnggota(Anggota *anggota, int jumlahAnggota) {
    printf("\nData Anggota dan Saldo:\n");
    for (int i = 0; i < jumlahAnggota; i++) {
        printf("%d. %s - Saldo: %.2f\n", i, anggota[i].nama, anggota[i].saldo); // Tampilkan nama dan saldo anggota
    }
}

// Fungsi untuk menampilkan riwayat transaksi
void tampilkanTransaksi(Transaksi *transaksi, int jumlahTransaksi) {
    printf("\nRiwayat Transaksi:\n");
    for (int i = 0; i < jumlahTransaksi; i++) {
        printf("%d. %s - %.2f (%s)\n", i + 1, transaksi[i].deskripsi, transaksi[i].jumlah, 
               transaksi[i].tipe == PEMASUKKAN ? "Pemasukan" : "Pengeluaran"); // Tampilkan detail transaksi
    }
}

// Fungsi untuk menambahkan anggota baru
void tambahAnggota(Anggota **anggota, int *jumlahAnggota, int *kapasitasAnggota) {
    if (*jumlahAnggota >= *kapasitasAnggota) {
        *kapasitasAnggota *= 2; // Gandakan kapasitas jika penuh
        *anggota = (Anggota *)realloc(*anggota, *kapasitasAnggota * sizeof(Anggota)); // Alokasikan ulang memori
        if (*anggota == NULL) {
            printf("Gagal mengalokasikan memori tambahan!\n");
            exit(1); // Keluar jika gagal mengalokasikan memori
        }
    }

    printf("\nMasukkan nama anggota baru: ");
    scanf(" %s", (*anggota)[*jumlahAnggota].nama); // Input nama anggota baru
    (*anggota)[*jumlahAnggota].saldo = 0.0; // Set saldo awal anggota ke 0
    (*jumlahAnggota)++; // Tambahkan jumlah anggota

    printf("Anggota baru berhasil ditambahkan!\n");
}

// Fungsi untuk menggunakan union Data sebagai alat pencarian anggota atau transaksi
void cariData(Anggota *anggota, Transaksi *transaksi, int jumlahAnggota, int jumlahTransaksi) {
    Data data;
    printf("\nMasukkan nama anggota atau deskripsi transaksi untuk dicari: ");
    scanf(" %s", data.namaAnggota); // Input nama atau deskripsi

    printf("\nHasil pencarian:\n");

    // Cari di daftar anggota
    for (int i = 0; i < jumlahAnggota; i++) {
        if (strcmp(data.namaAnggota, anggota[i].nama) == 0) {
            printf("Anggota ditemukan: %s - Saldo: %.2f\n", anggota[i].nama, anggota[i].saldo);
        }
    }

    // Cari di daftar transaksi
    for (int i = 0; i < jumlahTransaksi; i++) {
        if (strcmp(data.namaAnggota, transaksi[i].deskripsi) == 0) {
            printf("Transaksi ditemukan: %s - %.2f (%s)\n", transaksi[i].deskripsi, transaksi[i].jumlah, 
                   transaksi[i].tipe == PEMASUKKAN ? "Pemasukan" : "Pengeluaran");
        }
    }
}

int main() {
    int kapasitasAnggota = ANGGOTA_AWAL; // Kapasitas awal anggota
    Anggota *anggota = (Anggota *)malloc(kapasitasAnggota * sizeof(Anggota)); // Alokasi memori untuk daftar anggota
    Transaksi *transaksi = (Transaksi *)malloc(100 * sizeof(Transaksi)); // Alokasi memori untuk transaksi
    int jumlahTransaksi = 0; // Hitungan awal transaksi
    int jumlahAnggota = ANGGOTA_AWAL; // Hitungan awal anggota

    // Nama anggota
    char *nama[ANGGOTA_AWAL] = {"Andi", "Budi", "Cinta", "Dina", "Erik", "Faisal", "Ghani", "Heru", "Iwan", "Justin", "Kirana", "Leni", "Marlina", "Neni", "Omar"};

    for (int i = 0; i < ANGGOTA_AWAL; i++) {
        strcpy(anggota[i].nama, nama[i]); // Salin nama anggota dari array ke struct
        anggota[i].saldo = 0.0; // Set saldo awal anggota ke 0
    }

    int pilihan;
    do {
        printf("\n=== Sistem Keuangan Remaja Masjid ===\n");
        printf("1. Tambah Transaksi\n");
        printf("2. Lihat Data Anggota\n");
        printf("3. Lihat Riwayat Transaksi\n");
        printf("4. Tambah Anggota Baru\n");
        printf("5. Cari Data\n");
        printf("6. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &pilihan); // Input pilihan menu

        switch (pilihan) {
            case 1:
                tambahTransaksi(anggota, transaksi, &jumlahTransaksi, jumlahAnggota); // Tambah transaksi
                break;
            case 2:
                tampilkanAnggota(anggota, jumlahAnggota); // Lihat data anggota
                break;
            case 3:
                tampilkanTransaksi(transaksi, jumlahTransaksi); // Lihat riwayat transaksi
                break;
            case 4:
                tambahAnggota(&anggota, &jumlahAnggota, &kapasitasAnggota); // Tambah anggota baru
                break;
            case 5:
                cariData(anggota, transaksi, jumlahAnggota, jumlahTransaksi); // Cari data anggota atau transaksi
                break;
            case 6:
                printf("Keluar dari program.\n"); // Keluar dari program
                break;
            default:
                printf("Pilihan tidak valid!\n"); // Pesan jika pilihan tidak valid
        }
    } while (pilihan != 6); // Ulangi sampai pengguna memilih keluar

    free(anggota); // Bebaskan memori untuk anggota
    free(transaksi); // Bebaskan memori untuk transaksi

    return 0; // Akhiri program
}
