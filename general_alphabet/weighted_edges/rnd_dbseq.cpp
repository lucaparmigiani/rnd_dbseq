#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<vector>
#include<string.h>
#include<cmath>
#include"unionfind.hpp"
#include"fenwicktree.hpp"
#include"vec.hpp"

#define ll long long int

int k, sig;
ll n, m;
int num_cycles;
int *cycle;
int *bwt;
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

void shuffle(int *A, int start, int end){
    for (size_t i=start; i<end; i++){
        int j = (rand() % (end-start)) + (start);
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

    int num_possible_edges = num_cycles*(num_cycles-1)/2;

    // Possible edges of Spanning Tree between cycle
    Vec< Vec< Edge<ll> > > edges (num_possible_edges);
    for (int i = 0; i < edges.capacity(); i++) edges[i] = Vec< Edge<ll> >();
    FenwickTree<ll> edges_weight(num_possible_edges);

    for (ll i = 0; i < n; i+=sig) {
        for (ll j = i+1; j < i+sig; j++) {
            int C1 = cycle[j];
            for (ll z = i; z < j; z++) {
                int C2 = cycle[z];
                if (C1 != C2) {
                    int c1 = std::min(C1, C2)-1;
                    int c2 = std::max(C1, C2)-1;
                    int index = ((c1 * (2*num_cycles-c1-1))>>1) + c2-c1-1;
                    Edge<ll> edg = {z,j};
                    edges[index].push(edg);
                    edges_weight.A[index]++;
                    edges_weight.tot++;
                }
            }
        }
    }
    edges_weight.linear_construction();
    // Generate a Spanning tree of the Cycle graph
    UnionFind ST = UnionFind(num_cycles);

    while (num_cycles > 1) {
        int j = edges_weight.lower_bound(rand() % edges_weight.tot);

        // If the edge can be added to the ST swap is applied to bwt
        if (edges[j].size() != 0) 
        {
            int z = rand() % edges[j].size();
            if (ST.Union(cycle[edges[j][z].u], cycle[edges[j][z].v])) {
                swap(bwt, edges[j][z].u, edges[j][z].v);
                num_cycles--;
            }
        }
        edges_weight.swap_with_last(j, edges[j].size(), edges[edges_weight.n-1].size());
        Vec< Edge<ll> > e = edges[j];
        edges[j] = edges[edges_weight.n];
        edges[edges_weight.n] = e;
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


    int i = 0;
    m = pow(sig, k-1);
    n = m*sig;

    cycle = new int[n]();
    bwt = new int[n];

    rnd_debruijn();

    delete [] bwt;

    return 0;
}
