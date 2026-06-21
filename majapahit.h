#ifndef majapahit_h
#define majapahit_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "boolean.h"

/***************************************************************/
/* KAMUS TIPE DATA                                             */
/***************************************************************/

/* Panjang maksimal string untuk nama dan status */
#define MAKS_NAMA   50
#define MAKS_STATUS 15
#define JML_TOKOH   18

/* Nilai sentinel untuk pointer kosong */
#define nil NULL

/*
 * infotype : tipe data yang disimpan di setiap node
 * Menyimpan seluruh informasi satu tokoh Kerajaan Majapahit
 */
typedef struct {
    char nama[MAKS_NAMA];           /* Nama lengkap tokoh              */
    char statusHidup[MAKS_STATUS];  /* "hidup" atau "meninggal"        */
    char statusKedudukan[MAKS_STATUS]; /* "normal" atau "pendeta"      */
    char statusIbu[MAKS_STATUS];    /* "permaisuri" atau "selir"       */
    int  urutanLahir;               /* Urutan lahir dari ayah (1=tua)  */
} infotype;

/* address : pointer ke sebuah Node */
typedef struct tNode* address;

/*
 * Struktur satu Node dalam Non-Binary Tree
 * Menggunakan representasi First Son - Next Sibling
 */
typedef struct tNode {
    infotype info;   /* Data tokoh yang disimpan di node ini  */
    address  ps_fs;  /* Pointer ke anak pertama (first son)   */
    address  ps_nb;  /* Pointer ke saudara berikutnya (next brother) */
    address  ps_pr;  /* Pointer ke induk (parent)             */
} Node;

/* Tree direpresentasikan sebagai pointer ke node ROOT */
typedef address Tree;

/***************************************************************/
/* S P E S I F I K A S I                                       */
/***************************************************************/

/***** Konstruktor *****/

void Create_tree(Tree *T);
/* Membuat tree kosong                    */
/* IS : T sembarang                       */
/* FS : T menjadi tree kosong (T = NULL)  */

address Alokasi(infotype X);
/* Mengalokasikan node baru berisi X      */
/* IS : X terdefinisi                     */
/* FS : Mengembalikan address node baru,  */
/*      atau NULL jika alokasi gagal      */

/***** Validasi *****/

boolean IsEmpty(Tree T);
/* Mengirimkan true jika Tree T kosong    */

/***** Update *****/

void AddChild(address parent, address child);
/* Menambahkan child sebagai anak dari parent              */
/* IS : parent dan child sudah dialokasi, child belum      */
/*      terhubung ke manapun                               */
/* FS : child menjadi anak paling kanan dari parent,       */
/*      ps_pr child menunjuk ke parent                     */

/***** Traversal / Tampilan *****/

void TampilSilsilah(Tree T, int level);
/* Menampilkan silsilah secara bertingkat ke layar          */
/* IS : T terdefinisi, level = kedalaman saat ini           */
/* FS : Seluruh tokoh ditampilkan dengan indentasi sesuai   */
/*      level beserta keterangan asal ibu dan urutan lahir  */

void TampilDaftarTokoh(Tree T);
/* Menampilkan daftar seluruh tokoh bernomor urut            */
/* IS : T terdefinisi dan tidak kosong                       */
/* FS : Daftar tokoh 1..JML_TOKOH tampil di layar           */

/***** Pencarian Pewaris *****/

address CariNodeKe(Tree T, int target, int *counter);
/* Mencari node ke-target secara preorder                    */
/* IS : T terdefinisi, target = nomor urut tokoh (1-based)  */
/*      counter dimulai dari 0                               */
/* FS : Mengembalikan address node ke-target, atau NULL      */

address CariPewarisAnak(address raja);
/* Mencari pewaris dari anak-anak raja sesuai aturan suksesi */
/* IS : raja terdefinisi                                      */
/* FS : Mengembalikan address pewaris dari anak permaisuri    */
/*      (prioritas), lalu anak selir; NULL jika tidak ada    */

address CariPewarisSaudara(address raja);
/* Mencari pewaris dari saudara kandung raja                  */
/* IS : raja terdefinisi dan memiliki parent                  */
/* FS : Mengembalikan address saudara tertua yang layak,      */
/*      permaisuri diprioritaskan; NULL jika tidak ada        */

address CariPewaris(address raja);
/* Fungsi utama pencarian pewaris tahta                        */
/* IS : raja adalah node tokoh yang sedang dicari pewarisnya  */
/* FS : Mengembalikan address pewaris yang paling berhak       */
/*      sesuai aturan suksesi; NULL jika tidak ada kandidat   */

void TampilHasilPencarian(address raja, address pewaris);
/* Menampilkan hasil pencarian pewaris ke layar               */
/* IS : raja terdefinisi; pewaris boleh NULL                  */
/* FS : Hasil pencarian tampil dalam format terstruktur        */

/***** Dealokasi *****/

void DeAlokasi(Tree *T);
/* Membebaskan seluruh memori yang dipakai Tree T  */
/* IS : T terdefinisi                              */
/* FS : Seluruh node dibebaskan, T = NULL          */

#endif
