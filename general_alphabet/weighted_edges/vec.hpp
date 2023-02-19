#pragma once
#include <cstdlib>

#define vroundup(x) ((x) == 1 ? 1 : 1 << (64-__builtin_clzl((x)-1)))

template<typename T>
struct Vec {
    T* A;
    size_t n, m;

    Vec(size_t new_m=0) {
        //memset(*this, 0, sizeof(*this));
        A = 0;
        n = m = 0;
        if (new_m) init(new_m);
    }
    ~Vec() {
        free(A);
    }

    inline size_t size() const { return n; }
    inline size_t capacity() const { return m; }
    inline const T pop() { return A[--n]; }
    inline T top() const { return A[n-1]; }
    inline T& operator[](size_t i) const { return A[i]; }

    void init(size_t new_m) {
        n = 0;
        m = vroundup(new_m);
        A = (T*) malloc(m*sizeof(*(A))); 
    }

    void init0(size_t new_m) {
        n = 0;
        m = vroundup(new_m);
        A = (T*) calloc(m, sizeof(*(A))); 
    }

    void clean() {
        free(A);
    }

    void resize(size_t new_m) {
        if (m < new_m) {
            m = new_m;
            m = vroundup(m);
            A = (T*) realloc(A, m*sizeof(*(A)));
        }
    }

    void push(const T &val) {
        if (n == m) {
            m = m ? m << 1 : 2;
            A = (T*) realloc(A, m*sizeof(*(A)));
        }
        A[n++] = val;
    }
};
