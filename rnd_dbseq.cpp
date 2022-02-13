#include <iostream>
#include <vector>
#include <cmath>
#include "unionfind.h"

using namespace std;

#define ll long long int

int k;
ll m, n;
int num_cycles;
int *cycle;
bool *bwt_enc;

//--------------------------------------------------------------------------------
// Random bitstring representing the encoded bwt
//--------------------------------------------------------------------------------
void rnd_bistr(){
    for (size_t i = 0; i < m; i++)  bwt_enc[i] = rand() % 2;
}

//--------------------------------------------------------------------------------
// Assign cycle to the permutation of the bwt_enc of a dB set
//--------------------------------------------------------------------------------
void bwt2cycle() {
    bool *checked = new bool[n]();
    
    ll i = 0;
    num_cycles = 0;
    while (i < n) {
        while (!checked[i]) {
            checked[i] = 1;
            cycle[i] = num_cycles;
            i = i/2 + (bwt_enc[i/2] ^ (i%2))*m;
        }
        i++;
        if (i<n && !checked[i]) num_cycles++;
    }
    num_cycles++;

    delete [] checked;
}

//--------------------------------------------------------------------------------
// Return the inverse of the bwt_enc of de Bruijn sequence
//--------------------------------------------------------------------------------
void inv_bwt() {
    ll i = 0;
    do {
        //printf("%d", bwt_enc[i/2]^int(i%2)); // dbseq with numbers {0,1}
        printf("%c", 'a'+(bwt_enc[i/2]^int(i%2))); // dbseq with letters {a,b}
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

    int i = 0;
    while (num_cycles > 1) {
        int j = (rand() % (num_edges - i)) + i;
        ll e = edges[j];
        edges[j] = edges[i];
        edges[i] = e;
        // If the edge can be added to the ST swap is applied to perm
        if (ST.Union(cycle[e], cycle[e+1])) {
            bwt_enc[e/2]=!bwt_enc[e/2];
            num_cycles--;
        }
        i++;
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

    m = pow(2, k-1);
    n = m*2;
    bwt_enc = new bool[m];
    cycle = new int[n];

    rnd_debruijn();

    delete [] bwt_enc;
 
    return 0;
}
