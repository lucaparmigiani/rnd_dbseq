#pragma once

struct UnionFind {
    int *parent;
    int *rank;

    UnionFind(int num_of_v) {
        parent = new int[num_of_v];
        for (int i = 0; i < num_of_v; i++) parent[i] = i;
        rank = new int[num_of_v];
        for (int i = 0; i < num_of_v; i++) rank[i] = 0;
    };

    ~UnionFind() { delete [] parent; delete [] rank; };

    int Find(int u) {
        if (parent[u] != u) parent[u] = Find(parent[u]);
        return parent[u];
    }

    bool Union(int u, int v) {
        int x = Find(u-1);
        int y = Find(v-1);

        if (x == y) return false;

        if (rank[x] > rank[y]) parent[x] = y;
        else if (rank[x] < rank[y]) parent[y] = x;
        else {
            parent[y] = x;
            rank[x]++;
        }
        return true;
    };
};
