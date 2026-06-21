#include "majapahit.h"

/***************************************************************/
/* KONSTRUKTOR                                                 */
/***************************************************************/

void Create_tree(Tree *T) {
/* Membuat tree kosong                    */
/* IS : T sembarang                       */
/* FS : T menjadi tree kosong (T = NULL)  */
    *T = nil;
}

address Alokasi(infotype X) {
/* Mengalokasikan node baru berisi X      */
/* IS : X terdefinisi                     */
/* FS : Mengembalikan address node baru,  */
/*      atau NULL jika alokasi gagal      */
    address node = (address) malloc(sizeof(Node));
    if (node != nil) {
        node->info  = X;
        node->ps_fs = nil;
        node->ps_nb = nil;
        node->ps_pr = nil;
    }
    return node;
}

/***************************************************************/
/* VALIDASI                                                    */
/***************************************************************/

boolean IsEmpty(Tree T) {
/* Mengirimkan true jika Tree T kosong */
    return (T == nil);
}

/***************************************************************/
/* UPDATE                                                      */
/***************************************************************/

void AddChild(address parent, address child) {
/* Menambahkan child sebagai anak dari parent              */
/* IS : parent dan child sudah dialokasi, child belum      */
/*      terhubung ke manapun                               */
/* FS : child menjadi anak paling kanan dari parent,       */
/*      ps_pr child menunjuk ke parent                     */
    child->ps_pr = parent;

    if (parent->ps_fs == nil) {
        /* Parent belum punya anak -> child jadi first son */
        parent->ps_fs = child;
    } else {
        /* Parent sudah punya anak -> cari saudara paling kanan */
        /* lalu sambungkan child sebagai next brother           */
        address sib = parent->ps_fs;
        while (sib->ps_nb != nil) {
            sib = sib->ps_nb;
        }
        sib->ps_nb = child;
    }
}

/***************************************************************/
/* TRAVERSAL / TAMPILAN                                        */
/***************************************************************/

void TampilSilsilah(Tree T, int level) {
/* Menampilkan silsilah secara bertingkat ke layar          */
/* IS : T terdefinisi, level = kedalaman saat ini           */
/* FS : Seluruh tokoh ditampilkan dengan indentasi sesuai   */
/*      level beserta keterangan asal ibu dan urutan lahir  */
    if (T == nil) return;

    int i;
    /* Cetak indentasi sesuai level */
    for (i = 0; i < level; i++) {
        printf("  |  ");
    }

    /* Cetak nama dan keterangan tokoh */
    if (level == 0) {
        /* Root tidak punya keterangan ibu */
        printf("%s\n", T->info.nama);
    } else {
        char keterangan[60];
        /* Tambahkan keterangan pendeta jika ada */
        if (strcmp(T->info.statusKedudukan, "pendeta") == 0) {
            sprintf(keterangan, "[Anak %s, ke-%d] [PENDETA]",
                    T->info.statusIbu, T->info.urutanLahir);
        } else {
            sprintf(keterangan, "[Anak %s, ke-%d]",
                    T->info.statusIbu, T->info.urutanLahir);
        }
        printf("|- %-26s %s\n", T->info.nama, keterangan);
    }

    /* Rekursif ke anak pertama (turun satu level) */
    TampilSilsilah(T->ps_fs, level + 1);

    /* Rekursif ke saudara berikutnya (level sama) */
    TampilSilsilah(T->ps_nb, level);
}

/* Helper untuk TampilDaftarTokoh: kumpulkan pointer node secara preorder */
static void KumpulkanNode(Tree T, address daftar[], int *idx) {
/* IS : T terdefinisi, daftar[] tersedia, idx = indeks saat ini  */
/* FS : daftar[] terisi pointer node secara preorder             */
    if (T == nil) return;
    daftar[(*idx)++] = T;
    KumpulkanNode(T->ps_fs, daftar, idx);
    KumpulkanNode(T->ps_nb, daftar, idx);
}

void TampilDaftarTokoh(Tree T) {
/* Menampilkan daftar seluruh tokoh bernomor urut            */
/* IS : T terdefinisi dan tidak kosong                       */
/* FS : Daftar tokoh 1..JML_TOKOH tampil di layar           */
    address daftar[JML_TOKOH + 1];
    int idx = 0, i;

    KumpulkanNode(T, daftar, &idx);

    printf("====================================================\n");
    printf("   DAFTAR TOKOH\n");
    printf("====================================================\n");

    /* Tampilkan dua kolom agar lebih rapi */
    int separuh = (idx + 1) / 2;
    for (i = 0; i < separuh; i++) {
        int j = i + separuh;
        if (j < idx) {
            printf("  %2d. %-24s  %2d. %s\n",
                   i + 1, daftar[i]->info.nama,
                   j + 1, daftar[j]->info.nama);
        } else {
            printf("  %2d. %s\n", i + 1, daftar[i]->info.nama);
        }
    }
    printf("====================================================\n");
}

/***************************************************************/
/* PENCARIAN PEWARIS                                           */
/***************************************************************/

address CariNodeKe(Tree T, int target, int *counter) {
/* Mencari node ke-target secara preorder                    */
/* IS : T terdefinisi, target = nomor urut tokoh (1-based)  */
/*      counter dimulai dari 0                               */
/* FS : Mengembalikan address node ke-target, atau NULL      */
    if (T == nil) return nil;

    (*counter)++;
    if (*counter == target) return T;

    address hasil = CariNodeKe(T->ps_fs, target, counter);
    if (hasil != nil) return hasil;

    return CariNodeKe(T->ps_nb, target, counter);
}

/* Helper: cek apakah seorang tokoh layak menjadi pewaris */
static boolean LayakJadiPewaris(address node) {
/* IS : node terdefinisi                                      */
/* FS : Mengembalikan true jika node memenuhi syarat pewaris: */
/*      masih hidup dan bukan pendeta                         */
    return (strcmp(node->info.statusHidup, "hidup") == 0 &&
            strcmp(node->info.statusKedudukan, "normal") == 0);
}

/* Helper: cari anak terbaik dari daftar bersaudara berdasarkan statusIbu */
static address CariAnakTerbaik(address anakPertama, char *statusIbu) {
/* IS : anakPertama = pointer ke anak pertama dari suatu node  */
/*      statusIbu   = "permaisuri" atau "selir"                */
/* FS : Mengembalikan address anak tertua (urutan lahir terkecil) */
/*      yang statusIbu-nya sesuai dan layak jadi pewaris;      */
/*      NULL jika tidak ada                                    */
    address curr     = anakPertama;
    address terbaik  = nil;

    while (curr != nil) {
        if (strcmp(curr->info.statusIbu, statusIbu) == 0 &&
            LayakJadiPewaris(curr)) {
            /* Pilih yang urutan lahirnya paling kecil (paling tua) */
            if (terbaik == nil ||
                curr->info.urutanLahir < terbaik->info.urutanLahir) {
                terbaik = curr;
            }
        }
        curr = curr->ps_nb;
    }
    return terbaik;
}

address CariPewarisAnak(address raja) {
/* Mencari pewaris dari anak-anak raja sesuai aturan suksesi */
/* IS : raja terdefinisi                                      */
/* FS : Mengembalikan address pewaris dari anak permaisuri    */
/*      (prioritas), lalu anak selir; NULL jika tidak ada    */

    /* Langkah 1: cari anak permaisuri tertua yang layak */
    address kandidat = CariAnakTerbaik(raja->ps_fs, "permaisuri");
    if (kandidat != nil) return kandidat;

    /* Langkah 2: tidak ada dari permaisuri -> coba anak selir */
    kandidat = CariAnakTerbaik(raja->ps_fs, "selir");
    return kandidat;
}

address CariPewarisSaudara(address raja) {
/* Mencari pewaris dari saudara kandung raja                  */
/* IS : raja terdefinisi dan memiliki parent                  */
/* FS : Mengembalikan address saudara tertua yang layak,      */
/*      permaisuri diprioritaskan; NULL jika tidak ada        */
    if (raja->ps_pr == nil) return nil;  /* Raja adalah root, tidak ada saudara */

    address anakParent = raja->ps_pr->ps_fs;  /* Mulai dari anak pertama parent */

    /* Prioritas 1: saudara dari permaisuri */
    address curr    = anakParent;
    address terbaik = nil;
    while (curr != nil) {
        /* Jangan pilih raja itu sendiri */
        if (curr != raja &&
            strcmp(curr->info.statusIbu, "permaisuri") == 0 &&
            LayakJadiPewaris(curr)) {
            if (terbaik == nil ||
                curr->info.urutanLahir < terbaik->info.urutanLahir) {
                terbaik = curr;
            }
        }
        curr = curr->ps_nb;
    }
    if (terbaik != nil) return terbaik;

    /* Prioritas 2: saudara dari selir */
    curr = anakParent;
    while (curr != nil) {
        if (curr != raja &&
            strcmp(curr->info.statusIbu, "selir") == 0 &&
            LayakJadiPewaris(curr)) {
            if (terbaik == nil ||
                curr->info.urutanLahir < terbaik->info.urutanLahir) {
                terbaik = curr;
            }
        }
        curr = curr->ps_nb;
    }
    return terbaik;
}

address CariPewaris(address raja) {
/* Fungsi utama pencarian pewaris tahta                        */
/* IS : raja adalah node tokoh yang sedang dicari pewarisnya  */
/* FS : Mengembalikan address pewaris yang paling berhak       */
/*      sesuai aturan suksesi; NULL jika tidak ada kandidat   */

    /* Langkah 1 & 2: cari dari anak-anak raja */
    address pewaris = CariPewarisAnak(raja);
    if (pewaris != nil) return pewaris;

    /* Langkah 3: raja tidak punya anak yang layak -> cari saudara */
    pewaris = CariPewarisSaudara(raja);
    return pewaris;
}

void TampilHasilPencarian(address raja, address pewaris) {
/* Menampilkan hasil pencarian pewaris ke layar               */
/* IS : raja terdefinisi; pewaris boleh NULL                  */
/* FS : Hasil pencarian tampil dalam format terstruktur        */
    printf("\n====================================================\n");
    printf("   PENCARIAN PEWARIS TAHTA\n");
    printf("====================================================\n");
    printf("Raja saat ini    : %s\n", raja->info.nama);
    printf("----------------------------------------------------\n");

    if (pewaris == nil) {
        printf("Pewaris Tahta    : TIDAK DITEMUKAN\n");
        printf("Alasan           : Tidak ada keturunan maupun\n");
        printf("                   saudara yang memenuhi syarat.\n");
    } else {
        printf("Pewaris Tahta    : %s\n", pewaris->info.nama);

        /* Tentukan dan tampilkan alasan pemilihan */
        if (pewaris->ps_pr == raja) {
            /* Pewaris adalah anak langsung raja */
            if (strcmp(pewaris->info.statusIbu, "permaisuri") == 0) {
                /* Cek apakah ada anak permaisuri yang dilewati karena pendeta */
                boolean adaPendeta = false;
                address curr = raja->ps_fs;
                while (curr != nil) {
                    if (strcmp(curr->info.statusIbu, "permaisuri") == 0 &&
                        strcmp(curr->info.statusKedudukan, "pendeta") == 0 &&
                        curr->info.urutanLahir < pewaris->info.urutanLahir) {
                        adaPendeta = true;
                    }
                    curr = curr->ps_nb;
                }
                if (adaPendeta) {
                    printf("Alasan           : Anak permaisuri tertua yang layak\n");
                    printf("                   naik tahta. (Ada saudara yang\n");
                    printf("                   dilewati karena berstatus pendeta.)\n");
                } else {
                    printf("Alasan           : Anak permaisuri tertua yang layak\n");
                    printf("                   naik tahta.\n");
                }
            } else {
                printf("Alasan           : Tidak ada anak permaisuri yang layak.\n");
                printf("                   Pewaris dipilih dari anak selir tertua\n");
                printf("                   yang memenuhi syarat.\n");
            }
        } else {
            /* Pewaris adalah saudara raja */
            printf("Alasan           : %s tidak memiliki anak\n", raja->info.nama);
            printf("                   yang memenuhi syarat. Pewaris\n");
            printf("                   dipilih dari saudara kandung tertua\n");
            if (strcmp(pewaris->info.statusIbu, "permaisuri") == 0) {
                printf("                   dari permaisuri yang layak.\n");
            } else {
                printf("                   dari selir yang layak.\n");
            }
        }
    }
    printf("====================================================\n");
}

/***************************************************************/
/* DEALOKASI                                                   */
/***************************************************************/

void DeAlokasi(Tree *T) {
/* Membebaskan seluruh memori yang dipakai Tree T  */
/* IS : T terdefinisi                              */
/* FS : Seluruh node dibebaskan, T = NULL          */
    if (*T == nil) return;
    DeAlokasi(&(*T)->ps_fs);
    DeAlokasi(&(*T)->ps_nb);
    free(*T);
    *T = nil;
}
