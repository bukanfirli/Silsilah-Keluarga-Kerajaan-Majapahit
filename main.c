#include <stdio.h>
#include <string.h>
#include "majapahit.h"

/* ============================================================
   BuatNode : helper untuk membuat node tokoh dengan satu baris
   ============================================================ */
static address BuatNode(char *nama, char *statusHidup,
                        char *statusKedudukan, char *statusIbu,
                        int urutanLahir) {
    infotype data;
    strncpy(data.nama,            nama,            MAKS_NAMA   - 1);
    strncpy(data.statusHidup,     statusHidup,     MAKS_STATUS - 1);
    strncpy(data.statusKedudukan, statusKedudukan, MAKS_STATUS - 1);
    strncpy(data.statusIbu,       statusIbu,       MAKS_STATUS - 1);
    data.urutanLahir = urutanLahir;
    return Alokasi(data);
}

/* ============================================================
   BangunSilsilah : membangun seluruh tree silsilah Majapahit
   Data 18 tokoh sudah tetap (hardcoded) sesuai sumber sejarah
   ============================================================ */
static void BangunSilsilah(Tree *T) {
    /*
     * Format BuatNode:
     * (nama, statusHidup, statusKedudukan, statusIbu, urutanLahir)
     *
     * statusHidup     : "hidup" / "meninggal"
     * statusKedudukan : "normal" / "pendeta"
     * statusIbu       : "permaisuri" / "selir"
     * urutanLahir     : 1 = anak tertua dari ayah tersebut
     *
     * Catatan: Wikramawardhana secara historis adalah menantu
     * Hayam Wuruk. Untuk keperluan representasi tree ia
     * ditempatkan sebagai anak ke-3 Kusumawardhani.
     */

    /* ── Generasi 1 : Pendiri ── */
    address radenWijaya = BuatNode(
        "Raden Wijaya", "meninggal", "normal", "-", 1);

    /*
     * Catatan status hidup:
     * Program disimulasikan pada masa pemerintahan Suhita (1429-1447).
     * Tokoh yang lahir sebelum era ini dan relevan untuk suksesi
     * ditetapkan "hidup" agar seluruh skenario demo dapat berjalan.
     * Tokoh generasi jauh ke bawah ditetapkan "meninggal" karena
     * belum lahir pada era simulasi.
     */

    /* ── Generasi 2 : Anak Raden Wijaya ── */
    address jayanagara  = BuatNode(
        "Jayanagara",  "meninggal", "normal", "selir",      1);
    address tribhuwana  = BuatNode(
        "Tribhuwana",  "hidup",     "normal", "permaisuri", 2);
    address rajadewi    = BuatNode(
        "Rajadewi",    "hidup",     "normal", "permaisuri", 3);

    /* ── Generasi 3 : Anak Tribhuwana ── */
    address hayamWuruk  = BuatNode(
        "Hayam Wuruk", "meninggal", "normal", "permaisuri", 1);

    /* ── Generasi 4 : Anak Hayam Wuruk ── */
    address kusumawardhani = BuatNode(
        "Kusumawardhani", "hidup",     "normal", "permaisuri", 1);
    address bhreWirabhumi  = BuatNode(
        "Bhre Wirabhumi",  "meninggal", "normal", "selir",      2);

    /* ── Generasi 5 : Anak Kusumawardhani ── */
    address suhita           = BuatNode(
        "Suhita",           "hidup",     "normal",  "permaisuri", 1);
    address kertawijaya      = BuatNode(
        "Kertawijaya",      "hidup",     "normal",  "selir",      2);
    address wikramawardhana  = BuatNode(
        "Wikramawardhana",  "hidup",     "pendeta", "selir",      3);

    /* ── Generasi 6 : Anak Kertawijaya ── */
    address rajasawardhana   = BuatNode(
        "Rajasawardhana",   "hidup",     "normal", "permaisuri", 1);
    address girisawardhana   = BuatNode(
        "Girisawardhana",   "hidup",     "normal", "permaisuri", 2);

    /* ── Generasi 7 : Anak Rajasawardhana ── */
    address samarawijaya     = BuatNode(
        "Samarawijaya",     "hidup",     "normal", "permaisuri", 1);
    address wijayakarana     = BuatNode(
        "Wijayakarana",     "meninggal", "normal", "permaisuri", 2);
    address wijayakusuma     = BuatNode(
        "Wijayakusuma",     "meninggal", "normal", "permaisuri", 3);
    address kertabhumi       = BuatNode(
        "Kertabhumi",       "hidup",     "normal", "permaisuri", 4);

    /* ── Generasi 7 : Anak Girisawardhana ── */
    address singhawikrama    = BuatNode(
        "Singhawikramawardhana", "hidup", "normal", "permaisuri", 1);

    /* ── Generasi 8 : Anak Singhawikramawardhana ── */
    address ranawijaya       = BuatNode(
        "Ranawijaya",       "hidup",     "normal", "permaisuri", 1);

    /* ── Susun hubungan parent-child ── */
    *T = radenWijaya;                       /* ROOT */

    AddChild(radenWijaya,     jayanagara);
    AddChild(radenWijaya,     tribhuwana);
    AddChild(radenWijaya,     rajadewi);

    AddChild(tribhuwana,      hayamWuruk);

    AddChild(hayamWuruk,      kusumawardhani);
    AddChild(hayamWuruk,      bhreWirabhumi);

    AddChild(kusumawardhani,  suhita);
    AddChild(kusumawardhani,  kertawijaya);
    AddChild(kusumawardhani,  wikramawardhana);

    AddChild(kertawijaya,     rajasawardhana);
    AddChild(kertawijaya,     girisawardhana);

    AddChild(rajasawardhana,  samarawijaya);
    AddChild(rajasawardhana,  wijayakarana);
    AddChild(rajasawardhana,  wijayakusuma);
    AddChild(rajasawardhana,  kertabhumi);

    AddChild(girisawardhana,  singhawikrama);

    AddChild(singhawikrama,   ranawijaya);
}

/* ============================================================
   main
   ============================================================ */
int main() {
    Tree silsilah;
    int  pilihan, counter;
    address nodeRaja, pewaris;

    /* ── 1. Bangun tree ── */
    Create_tree(&silsilah);
    BangunSilsilah(&silsilah);

    /* ── 2. Tampilkan silsilah ── */
    printf("====================================================\n");
    printf("   SILSILAH KELUARGA KERAJAAN MAJAPAHIT\n");
    printf("====================================================\n");
    TampilSilsilah(silsilah, 0);
    printf("====================================================\n\n");

    /* ── 3. Tampilkan daftar tokoh ── */
    TampilDaftarTokoh(silsilah);

    /* ── 4. Input nomor tokoh dari user (dengan validasi loop) ── */
    do {
        printf("Masukkan nomor tokoh (1-%d): ", JML_TOKOH);
        if (scanf("%d", &pilihan) != 1) {
            /* Bersihkan buffer jika input bukan angka */
            while (getchar() != '\n');
            pilihan = -1;
        }
        if (pilihan < 1 || pilihan > JML_TOKOH) {
            printf("Input tidak valid. Masukkan nomor antara 1 hingga %d.\n\n",
                   JML_TOKOH);
        }
    } while (pilihan < 1 || pilihan > JML_TOKOH);

    /* ── 5. Cari node yang dipilih ── */
    counter  = 0;
    nodeRaja = CariNodeKe(silsilah, pilihan, &counter);

    /* ── 6. Jalankan pencarian pewaris ── */
    pewaris = CariPewaris(nodeRaja);

    /* ── 7. Tampilkan hasil ── */
    TampilHasilPencarian(nodeRaja, pewaris);

    /* ── 8. Bebaskan memori ── */
    DeAlokasi(&silsilah);

    return 0;
}
