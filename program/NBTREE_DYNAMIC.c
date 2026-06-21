#include "nbtree_dynamic.h"

/* MODUL KONSTRUKTOR */
void Create_tree(Tree *T) {
    *T = NULL;
}

address Alokasi(infotype X) {
    address P = (address)malloc(sizeof(Node));
    if (P != NULL) {
        P->info = X;
        P->ps_fs = NULL;
        P->ps_nb = NULL;
        P->ps_pr = NULL;
    }
    return P;
}

/* MODUL VALIDASI */
int IsEmpty(Tree T) {
    return T == NULL;
}

/* MODUL UPDATE */
void AddChild(address parent, address child) {
    if (parent != NULL && child != NULL) {
        child->ps_pr = parent;
        if (parent->ps_fs == NULL) {
            parent->ps_fs = child;
        } else {
            address temp = parent->ps_fs;
            while (temp->ps_nb != NULL) {
                temp = temp->ps_nb;
            }
            temp->ps_nb = child;
        }
    }
}

/* MODUL TRAVERSAL */
void PreOrder(Tree T) {
    if (T != NULL) {
        printf("%c ", T->info);
        address child = T->ps_fs;
        while (child != NULL) {
            PreOrder(child);
            child = child->ps_nb;
        }
    }
}

void InOrder(Tree T) {
    if (T != NULL) {
        if (T->ps_fs != NULL) {
            InOrder(T->ps_fs);
        }
        printf("%c ", T->info);
        if (T->ps_fs != NULL) {
            address child = T->ps_fs->ps_nb;
            while (child != NULL) {
                InOrder(child);
                child = child->ps_nb;
            }
        }
    }
}

void PostOrder(Tree T) {
    if (T != NULL) {
        address child = T->ps_fs;
        while (child != NULL) {
            PostOrder(child);
            child = child->ps_nb;
        }
        printf("%c ", T->info);
    }
}

void LevelOrder(Tree T) {
    if (T == NULL) return;
    address queue[100];
    int front = 0, rear = 0;
    
    queue[rear++] = T;
    while (front < rear) {
        address current = queue[front++];
        printf("%c ", current->info);
        
        address child = current->ps_fs;
        while (child != NULL) {
            queue[rear++] = child;
            child = child->ps_nb;
        }
    }
}

/* Fungsi Bantuan untuk PrintTree bersarang */
void PrintTreeHelper(Tree T, int *index) {
    if (T != NULL) {
        printf("--> Node ke-%d\n", *index);
        printf("----------------------------------------\n");
        printf("info node          : %c\n", T->info);
        printf("first son          : %c\n", T->ps_fs ? T->ps_fs->info : '-');
        printf("next brother       : %c\n", T->ps_nb ? T->ps_nb->info : '-');
        printf("parent             : %c\n", T->ps_pr ? T->ps_pr->info : '-');
        printf("----------------------------------------\n\n");
        (*index)++;
        
        address child = T->ps_fs;
        while (child != NULL) {
            PrintTreeHelper(child, index);
            child = child->ps_nb;
        }
    }
}

void PrintTree(Tree T) {
    int index = 1;
    printf("Seluruh Node pada Non Binary Tree:\n\n");
    PrintTreeHelper(T, &index);
}

/* MODUL PENCARIAN */
int Search(Tree T, infotype X) {
    if (T == NULL) return 0;
    if (T->info == X) return 1;
    
    address child = T->ps_fs;
    while (child != NULL) {
        if (Search(child, X)) return 1;
        child = child->ps_nb;
    }
    return 0;
}

/* MODUL ANALISIS TREE */
int NbElmt(Tree T) {
    if (T == NULL) return 0;
    int count = 1;
    address child = T->ps_fs;
    while (child != NULL) {
        count += NbElmt(child);
        child = child->ps_nb;
    }
    return count;
}

int NbDaun(Tree T) {
    if (T == NULL) return 0;
    if (T->ps_fs == NULL) return 1; /* Daun = Tidak punya anak (first son) */
    
    int count = 0;
    address child = T->ps_fs;
    while (child != NULL) {
        count += NbDaun(child);
        child = child->ps_nb;
    }
    return count;
}

int LevelHelper(Tree T, infotype X, int currentLevel) {
    if (T == NULL) return 0;
    if (T->info == X) return currentLevel;
    
    address child = T->ps_fs;
    while (child != NULL) {
        int lvl = LevelHelper(child, X, currentLevel + 1);
        if (lvl != 0) return lvl;
        child = child->ps_nb;
    }
    return 0;
}

int Level(Tree T, infotype X) {
    return LevelHelper(T, X, 1); 
}

int Max(int a, int b) {
    return (a > b) ? a : b;
}

int Depth(Tree T) {
    if (T == NULL) return 0;
    if (T->ps_fs == NULL) return 1;
    
    int maxDepth = 0;
    address child = T->ps_fs;
    while (child != NULL) {
        maxDepth = Max(maxDepth, Depth(child));
        child = child->ps_nb;
    }
    return maxDepth + 1;
}

/* MODUL DEALOKASI */
void DeAlokasi(Tree *T) {
    if (*T != NULL) {
        address child = (*T)->ps_fs;
        while (child != NULL) {
            address next = child->ps_nb;
            DeAlokasi(&child);
            child = next;
        }
        free(*T);
        *T = NULL;
    }
}

/* Fungsi Tambahan untuk Membandingkan dua Tree */
int CompareTree(Tree T1, Tree T2) {
    if (T1 == NULL && T2 == NULL) return 1;
    if (T1 == NULL || T2 == NULL) return 0;
    if (T1->info != T2->info) return 0;
    
    address c1 = T1->ps_fs;
    address c2 = T2->ps_fs;
    
    while (c1 != NULL && c2 != NULL) {
        if (!CompareTree(c1, c2)) return 0;
        c1 = c1->ps_nb;
        c2 = c2->ps_nb;
    }
    
    /* Pastikan keduanya sudah mencapai NULL bersamaan */
    return c1 == NULL && c2 == NULL;
}