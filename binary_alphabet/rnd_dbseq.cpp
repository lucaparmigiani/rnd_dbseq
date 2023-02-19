#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<vector>
#include"unionfind.hpp"

using namespace std;

#define ll unsigned long long int

int k;
ll m, n;
int num_cycles;
int *cycle;
bool *bwt_enc;

//--------------------------------------------------------------------------------
// Random bitstring representing the encoded bwt
//--------------------------------------------------------------------------------
void rnd_bistr(){
    bwt_enc[0] = bwt_enc[m-1] = 1;
    for (size_t i = 1; i < m-1; i++)  bwt_enc[i] = rand() % 2;
}

//--------------------------------------------------------------------------------
// Assign cycle to the permutation of the bwt_enc of a dB set
//--------------------------------------------------------------------------------
void bwt2cycle() {
    ll i = 0;
    num_cycles = 1;
    while (i < n) {
        while (!cycle[i]) {
            cycle[i] = num_cycles;
            i = i/2 + (bwt_enc[i/2] ^ (i%2))*m;
        }
        i++;
        if (i<n && !cycle[i]) num_cycles++;
    }
}

//--------------------------------------------------------------------------------
// Return the inverse of the bwt_enc of de Bruijn sequence
//--------------------------------------------------------------------------------
void inv_bwt() {
    ll i = 0;
    do {
        printf("%d", bwt_enc[i/2]^int(i%2)); // dbseq with numbers {0,1}
        //printf("%c", 'a'+(bwt_enc[i/2]^int(i%2))); // dbseq with letters {a,b}
        i = i/2 + (bwt_enc[i/2] ^ (i%2)) * m;
    } while(i != 0);
    printf("\n");
}

//--------------------------------------------------------------------------------
// Generate a random binary de Bruijn sequence of order k
//--------------------------------------------------------------------------------
void rnd_debruijn() {
    rnd_bistr();
    bwt2cycle();

    if (num_cycles == 1) {
        inv_bwt();
        return;
    }

    // Possible edges of the cycle graph
    vector<ll> edges;
    for (ll i = 0; i < n; i+=2) 
        if (cycle[i] != cycle[i+1]) edges.push_back(i);

    // Generate a spanning tree of the cycle graph
    UnionFind ST = UnionFind(num_cycles);
    int num_edges = edges.size();

    while (num_cycles > 1) {
        int j = rand() % num_edges--;

        ll e = edges[j];
        edges[j] = edges[num_edges];
        edges[num_edges] = e;
        // If the edge can be added to the ST do it by toggling the bit in position e/2
        if (ST.Union(cycle[e], cycle[e+1])) {
            bwt_enc[e/2]=!bwt_enc[e/2];
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
        k = atoi(argv[1]);
    } else {
        k = 4;
    }

    m = 1<<(k-1);
    n = m*2;
    bwt_enc = new bool[m];
    cycle = new int[n]();

    rnd_debruijn();

    delete [] bwt_enc;
 
    return 0;
}
