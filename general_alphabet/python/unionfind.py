class UnionFind:
 
    def __init__(self, num_of_v):
        self.parent = [i for i in range(num_of_v)]
        self.rank = [0] * num_of_v

    def find(self, u):
        if self.parent[u] != u: self.parent[u] = self.find(self.parent[u])
        return self.parent[u]

    def union(self, u, v):
        x = self.find(u-1)
        y = self.find(v-1)

        if x == y: return False

        if self.rank[x] > self.rank[y]: self.parent[x] = y
        elif self.rank[y] > self.rank[x]: self.parent[y] = x
        # If ranks are same, then make one as
        # root and increment its rank by one
        else:
            self.parent[y] = x
            self.rank[x] += 1

        return True
