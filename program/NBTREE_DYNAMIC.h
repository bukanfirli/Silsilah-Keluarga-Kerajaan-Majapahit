#ifndef NBTREE_DYNAMIC_H
#define NBTREE_DYNAMIC_H

#include <stdio.h>
#include <stdlib.h>

/****************************************************/
/*              DEFINISI TIPE DATA                   */
/****************************************************/

/* Tipe data untuk menyimpan informasi dalam node */
typedef char infotype;

/* Tipe address adalah pointer ke Node */
typedef struct tNode* address;

/* Struktur Node untuk Non-Binary Tree Dinamis */
typedef struct tNode {
    infotype info;     // Menyimpan data (misal: karakter/nama node)
    address ps_fs;     // Pointer ke anak pertama (first son)
    address ps_nb;     // Pointer ke saudara berikutnya (next brother)
    address ps_pr;     // Pointer ke parent (induk)
} Node;

/* Tree direpresentasikan sebagai pointer ke root */
typedef address Tree;

/****************************************************/
/*                MODUL KONSTRUKTOR                  */
/****************************************************/

/* Membuat tree kosong (root = NULL) */
void Create_tree(Tree *T);
/* I.S. T sembarang */
/* F.S. T menjadi tree kosong */

/* Mengalokasikan node baru dengan nilai X */
address Alokasi(infotype X);
/* Mengembalikan address node baru */
/* Jika gagal, mengembalikan NULL */

/****************************************************/
/*                MODUL VALIDASI                     */
/****************************************************/

/* Mengecek apakah tree kosong */
int IsEmpty(Tree T);
/* Mengembalikan 1 jika kosong, 0 jika tidak */

/****************************************************/
/*                MODUL UPDATE                      */
/****************************************************/

/* Menambahkan child sebagai anak dari parent */
void AddChild(address parent, address child);
/* I.S. parent dan child sudah dialokasi */
/* F.S. child menjadi anak dari parent */

/****************************************************/
/*                MODUL TRAVERSAL                   */
/****************************************************/

/* Traversal PreOrder */
/* Mengunjungi: node → anak pertama → saudara */
void PreOrder(Tree T);

/* Traversal InOrder (versi non-binary) */
/* Mengunjungi: anak pertama → node → saudara */
void InOrder(Tree T);

/* Traversal PostOrder */
/* Mengunjungi: anak → saudara → node */
void PostOrder(Tree T);

/* Traversal Level Order (BFS) */
void LevelOrder(Tree T);
/* Menggunakan queue untuk traversal per level */

/* Menampilkan struktur tree */
void PrintTree(Tree T);
/* Menampilkan isi tree secara terstruktur */

/****************************************************/
/*                MODUL PENCARIAN                   */
/****************************************************/

/* Mencari node dengan nilai X */
int Search(Tree T, infotype X);
/* Mengembalikan 1 jika ditemukan, 0 jika tidak */

/****************************************************/
/*                MODUL ANALISIS TREE               */
/****************************************************/

/* Menghitung jumlah seluruh node */
int NbElmt(Tree T);

/* Menghitung jumlah daun (node tanpa anak) */
int NbDaun(Tree T);

/* Menghitung level node dengan nilai X */
int Level(Tree T, infotype X);
/* Root berada pada level 1 */

/* Menghitung kedalaman (height) tree */
int Depth(Tree T);

/* Mengembalikan nilai maksimum dari dua bilangan */
int Max(int a, int b);

/****************************************************/
/*                MODUL DEALOKASI                   */
/****************************************************/

/* Menghapus seluruh node dalam tree */
void DeAlokasi(Tree *T);
/* Membebaskan memori tree */

/****************************************************/

#endif