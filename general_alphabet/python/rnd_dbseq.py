import argparse
import random 
import sys
from itertools import product
from unionfind import UnionFind
from string import ascii_lowercase

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-a", "--alph", 
            help="pass a string of length sig representing the alphabet.\
                  example: rnd_dbseq 3 2 -a abc.")
    parser.add_argument("sig", type=int, help="size of the alphabet")
    parser.add_argument("k",   type=int, help="value for k")
    args = parser.parse_args()
    if args.alph is not None and len(args.alph) < args.sig:
        raise parser.error('alphabet cannot be smaller than sigma.')
    return args

args = parse_args()

#--------------------------------------------------------------------------------
# Util
#--------------------------------------------------------------------------------
def swap(A, i, j):
    tmp = A[i]
    A[i] = A[j];
    A[j] = tmp

def shuffle(A, start, end):
    for i in range(start, end):
        j = random.randint(start, end-1)
        swap(A, i, j);

#--------------------------------------------------------------------------------
# Random bitstring representing the encoded bwt
#--------------------------------------------------------------------------------
def rnd_bwt(sig, n):
    bwt = [0]*n
    for i in range(0, n, sig):
        for j in range(sig):
            bwt[i+j] = j
    for i in range(0, n, sig): shuffle(bwt, i, i+sig)
    return bwt

#--------------------------------------------------------------------------------
# Assign cycle to the permutation of the bwt of a dB set
#--------------------------------------------------------------------------------
def bwt2cycle(bwt, sig, n, m):
    cycle = [0]*n

    i = 0
    num_cycles = 1
    while i < n:
        while cycle[i]==0:
            cycle[i] = num_cycles
            i = i//sig + (bwt[i])*m
        i+=1
        if i < n and cycle[i]==0: num_cycles+=1

    return [cycle, num_cycles]

#--------------------------------------------------------------------------------
# Return the inverse of the bwt of de Bruijn sequence
#--------------------------------------------------------------------------------
def inv_bwt(bwt, Sig, sig, m):
    i = 0
    dbseq = Sig[bwt[i]]
    i = bwt[0]*m
    if args.alph is None and sig > 10: dbseq+=" "
    while i != 0:
        dbseq += Sig[bwt[i]]
        if args.alph is None and sig > 10: dbseq+=" "
        i = i//sig + (bwt[i])*m
    return dbseq

#--------------------------------------------------------------------------------
# Generate a random binary de Bruijn sequence of order k
#--------------------------------------------------------------------------------
def rnd_debruijn(Sig, k):
    sig = len(Sig)
    m = sig**(k-1)
    n = m*sig
    bwt = rnd_bwt(sig, n)
    [cycle, num_cycles] = bwt2cycle(bwt, sig, n, m)
    
    if num_cycles == 1: 
        return inv_bwt(bwt, Sig, sig, m)

    edges = []
    for i in range(0,n,sig):
        for j in range(i+1, i+sig):
            C1 = cycle[j]
            for z in range(i, j):
                C2 = cycle[z]
                if C1 != C2: edges.append([j,z])

    ST = UnionFind(num_cycles)
    num_edges = len(edges)

    while num_cycles > 1:
        j = random.randint(0, num_edges-1)
        num_edges -= 1

        e = edges[j]
        edges[j]= edges[num_edges]
        edges[num_edges] = e

        if ST.union(cycle[e[0]], cycle[e[1]]):
            swap(bwt, e[0], e[1])
            num_cycles-=1

    return inv_bwt(bwt, Sig, sig, m)

#--------------------------------------------------------------------------------
# Utility, sanity check for de Bruijn sequence
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
    sig = args.sig
    k = args.k
    Sig = args.alph
    if Sig == None:
        Sig = [str(i) for i in range(sig)]

    #Sig = [ascii_lowercase[i] for i in range(sig)]

    dbseq = rnd_debruijn(Sig, k)
    print(dbseq)
    #print(is_debruijn(dbseq, Sig, k))

if __name__ == "__main__":
    main()
