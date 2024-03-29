import random 
import sys
from itertools import product
from unionfind import UnionFind

Sig = ['0','1']
#Sig = ['a','b']

#--------------------------------------------------------------------------------
# Random bitstring representing the encoded bwt
#--------------------------------------------------------------------------------
def rnd_bitstr(m):
    bwt_enc = [0]*m
    for i in range(m):
        bwt_enc[i] = random.randint(0,1)
    return bwt_enc

#--------------------------------------------------------------------------------
# Assign cycle to the permutation of the bwt_enc of a dB set
#--------------------------------------------------------------------------------
def bwt2cycle(bwt_enc, n, m):
    cycle = [0]*n

    i = 0
    num_cycles = 1
    while i < n:
        while cycle[i] == 0:
            cycle[i] = num_cycles
            i = i//2 + (bwt_enc[i//2] ^ (i%2))*m
        i+=1
        if i<n and cycle[i]==0: num_cycles+=1

    return [cycle, num_cycles]

#--------------------------------------------------------------------------------
# Return the inverse of the bwt_enc of de Bruijn sequence
#--------------------------------------------------------------------------------
def inv_bwt(bwt_enc, m):
    i = 0
    dbseq = Sig[bwt_enc[i]]
    i = bwt_enc[0]*m
    while i != 0:
        dbseq += Sig[bwt_enc[i//2]^i%2]  # print db seq with {a,b}
        i = i//2 + (bwt_enc[i//2]^(i%2))*m
    return dbseq

#--------------------------------------------------------------------------------
# Generate a random binary de Bruijn sequence of order k
#--------------------------------------------------------------------------------
def rnd_debruijn(k):
    m = 2**(k-1)
    n = m*2
    bwt_enc = rnd_bitstr(m)
    [cycle, num_cycles] = bwt2cycle(bwt_enc, n, m)
    
    if num_cycles == 1: 
        return inv_bwt(bwt_enc, m)

    edges = []
    for i in range(0,n,2):
        if cycle[i] != cycle[i+1]: edges.append(i)

    ST = UnionFind(num_cycles)
    num_edges = len(edges)

    while num_cycles > 1:
        j = random.randint(0, num_edges-1)
        num_edges -= 1

        e = edges[j]
        edges[j]= edges[num_edges]
        edges[num_edges] = e

        if ST.union(cycle[e], cycle[e+1]):
            bwt_enc[e//2] = not bwt_enc[e//2]
            num_cycles-=1

    return inv_bwt(bwt_enc, m)

#--------------------------------------------------------------------------------
# Check for de Bruijn sequence
#--------------------------------------------------------------------------------
def is_debruijn(s, Sig, k):
    d = dict()
    if len(s) == len(Sig)**k:
        # initialize dict of all kmers with 0s
        for i in product(Sig, repeat=k):
            d[''.join(i)] = 0
        # Control the first kmers until
        for i in range(len(s)):
            j = (i+k)%len(s)
            if j < i:
                kmer = s[i:] + s[:j]
            else:
                kmer = s[i:j]

            if d[kmer] == 0:
                d[kmer] += 1
            else:
                return False
    else:
        return False

    return True

#--------------------------------------------------------------------------------
# MAIN
#--------------------------------------------------------------------------------
def main():
    #random.seed(1479)
    if len(sys.argv) >= 2:
        k = int(sys.argv[1])
    else:
        k = 6
    dbseq = rnd_debruijn(k)
    print(dbseq)
    #print(is_debruijn(dbseq, Sig, k))

if __name__ == "__main__":
    main()
