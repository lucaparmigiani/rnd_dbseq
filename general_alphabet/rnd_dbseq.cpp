#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <stdint.h>
#include <cmath>
#include <vector>
#include "unionfind.hpp"

#define ll unsigned long long int

int k, sig;
ll n, m;
int num_cycles;
uint32_t *cycle, *bwt;

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
    //char DNA[4] {'A', 'C', 'G', 'T'};
    do {
        printf("%d", bwt[i]);
        //printf("%c", DNA[bwt[i]]); //DNA
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
int main(int argc, char **argv) {
    srand(time(NULL));

    if (argc > 1){
        sig = atoi(argv[1]);
        k = atoi(argv[2]);
    } else {
        sig = 4;
        k = 4;
    }

    m = pow(sig, k-1);
    n = m*sig;

    bwt = new uint32_t[n];
    cycle = new uint32_t[n]();

    rnd_debruijn();

    delete [] bwt;
 
    return 0;
}
