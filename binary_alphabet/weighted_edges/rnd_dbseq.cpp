#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<vector>
#include"unionfind.hpp"
#include"fenwicktree.hpp"

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
    int num_possible_edges = num_cycles*(num_cycles-1)/2;

    std::vector< std::vector<ll> > edges = std::vector< std::vector<ll> >(num_possible_edges);
    for (int i = 0; i < edges.size(); i++) edges[i] = std::vector<ll>();
    FenwickTree<ll> edges_weight(num_possible_edges);

    for (ll i = 0; i < n; i+=2) {
        if (cycle[i] != cycle[i+1]) {
            int c1 = std::min(cycle[i], cycle[i+1])-1;
            int c2 = std::max(cycle[i], cycle[i+1])-1;
            int index = ((c1 * (2*num_cycles-c1-1))>>1) + c2-c1-1;
            edges[index].push_back(i);
            edges_weight.A[index]++;
            edges_weight.tot++;
        }
    }
    edges_weight.linear_construction();

    // Generate a spanning tree of the cycle graph
    UnionFind ST = UnionFind(num_cycles);
    int num_edges = edges.size();

    while (num_cycles > 1) {
        int j = edges_weight.lower_bound(rand() % edges_weight.tot);
        // If the edge can be added to the ST do it by toggling the bit in position e/2
        if (edges[j].size() != 0) {
            int z = rand() % edges[j].size();
            if (ST.Union(cycle[edges[j][z]], cycle[edges[j][z]+1])) {
                bwt_enc[edges[j][z]/2]=!bwt_enc[edges[j][z]/2];
                num_cycles--;
            }
        }
        edges_weight.swap_with_last(j, edges[j].size(), edges[edges_weight.n-1].size());
        edges[j].swap(edges[edges_weight.n]);
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
    cycle = new int[n]();
    bwt_enc = new bool[m];

    rnd_debruijn();

    delete [] bwt_enc;
 
    return 0;
}
