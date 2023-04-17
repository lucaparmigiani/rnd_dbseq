#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <stdint.h>
#include <string.h>
#include <cmath>
#include <vector>
#include "unionfind.hpp"

#define ll unsigned long long int

int k, sig;
ll n, m;
int num_cycles;
uint32_t *cycle, *bwt;
const char *alphabet = NULL;

//--------------------------------------------------------------------------------
// Utils
//--------------------------------------------------------------------------------
template <class T>
void swap(T *A, int i, int j){
    T tmp = A[i];
    A[i] = A[j];
    A[j] = tmp;
}

void shuffle(uint32_t *A, int start, int end){
    for (size_t i=start; i<end; i++){
        size_t j = (rand() % (end-start)) + (start);
        swap(A, i, j);
    }
}

//--------------------------------------------------------------------------------
// Random permutation of a bwt of a dB set
//--------------------------------------------------------------------------------
void rnd_bwt() {
    for (size_t i = 0; i < n; i+=sig) {
        for (int j = 0; j < sig; j++) {
            bwt[i+j] = j;
        }
    }
    //Shuffle
    for (size_t i = 0; i < n; i+=sig) shuffle(bwt, i, i+sig);
}

//--------------------------------------------------------------------------------
// Assign cycle to the permutation of the bwt of a dB set
//--------------------------------------------------------------------------------
void bwt2cycle() {
    ll i = 0;
    num_cycles = 1;
    while (i < n) {
        while (!cycle[i]) {
            cycle[i] = num_cycles;
            i = i/sig + bwt[i]*m;
        }
        i++;
        if (i<n && !cycle[i]) num_cycles++;
    }
}

//--------------------------------------------------------------------------------
// Return the inverse of the BWT
//--------------------------------------------------------------------------------
void inv_bwt() {
    ll i = 0;
    do {
        if (alphabet != NULL) printf("%c", alphabet[bwt[i]]);
        else { 
            printf("%d", bwt[i]);
            if (sig > 10) printf(" ");
        }
        i = i/sig + bwt[i]*m;
    } while(i != 0);
    printf("\n");
}

void rnd_debruijn() {
    rnd_bwt();
    bwt2cycle();

    if (num_cycles == 1) {
        inv_bwt();
        return;
    }

    // Possible edges of Spanning Tree between cycle
    std::vector< Edge<ll> > edges;
    for (ll i = 0; i < n; i+=sig) {
        for (ll j = i+1; j < i+sig; j++) {
            uint32_t C1 = cycle[j];
            for (ll z = i; z < j; z++) {
                uint32_t C2 = cycle[z];
                Edge<ll> edg = {z, j};
                if (C1 != C2) edges.push_back(edg);
            }
        }
    }

    // Generate a Spanning tree of the Cycle graph
    UnionFind ST = UnionFind(num_cycles);
    ll num_edges = edges.size();

    while (num_cycles > 1) {
        size_t j = rand() % num_edges--;

        Edge<ll> e = edges[j];
        edges[j] = edges[num_edges];
        edges[num_edges] = e;
        // If the edge can be added to the ST swap is applied to bwt
        if (ST.Union(cycle[e.u], cycle[e.v])) {
            swap(bwt, e.u, e.v);
            num_cycles--;
        }
    }

    delete [] cycle;
    
    inv_bwt();
}

//--------------------------------------------------------------------------------
// MAIN
//--------------------------------------------------------------------------------
void show_help(){
    printf("Usage: rnd_dbseq <sig> <k> [-a abc..]\n");
    printf("Options:\n");
    printf("  -h, --help        Show this help message and exit\n");
    printf("  -a, --alph        Pass a string of length sig representing\n");
    printf("                    the alphabet.\n");
    printf("                    Example: rnd_dbseq 3 2 -a abc.\n");
}

int main(int argc, char **argv) {
    srand(time(NULL));

    if (argc > 1){
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
                show_help();
                exit(0);
            }
        }
        sig = atoi(argv[1]);
        k = atoi(argv[2]);
        for (int i = 3; i < argc; i++) {
            if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--alph") == 0) {
                if (i+1 <= argc) {
                    alphabet = argv[i+1];
                    if (strlen(alphabet) < sig ) {
                        printf("Error: alphabet cannot be smaller than sigma.\n");
                        exit(1);
                    }
                }
            }
        }
    } else {
        show_help();
        exit(0);
    }

    m = pow(sig, k-1);
    n = m*sig;

    bwt = new uint32_t[n];
    cycle = new uint32_t[n]();

    rnd_debruijn();

    delete [] bwt;
 
    return 0;
}
