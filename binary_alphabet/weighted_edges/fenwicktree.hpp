#pragma once
#include<stdint.h>
#include<iostream>

// Least Significant Bit of i having a value of 1
#define LSB(__i) ((__i) & -(__i))
#define roundup(x) ((x) == 1 ? 1 : 1 << (64-__builtin_clzl((x)-1)))

template<typename T>
struct FenwickTree {
    T *A;
    T n, tot;

    FenwickTree(T _n) : n(_n) {
        A = new T[n]();
        tot = 0;
    }

    ~FenwickTree() {
        delete [] A;
    }

    void linear_construction() {
        for (T i = 0; i < n; i++) {
            T j = i + LSB(i);
            if (j != 0 && j < n)
                A[j] += A[i];
        }
        //tot = prefix_sum(n-1); // This I can avoid doing
    }

    T const size() const { return n; }

    T get(T i) { 
        if (i-- == 0) return A[0];
        T j = i+1;
        T sum = A[j];
        j -= LSB(j);
        for (; i > j; i -= LSB(i)) 
            sum -= A[i];
        return sum;
    }

    T prefix_sum(T i) {
        T sum = A[0];
        for (; i != 0; i -= LSB(i))
            sum += A[i];
        return sum;
    }

    //Returns the largest i s.t. A[i] >= value
    T lower_bound(T value) {
        T l = 0, r = roundup(n);
        
        while (l < r) 
        {
            T mid = (l+r)/2;
            if (mid < n && A[mid]+A[0] <= value)  {
                l = mid+1;
                value -= A[mid];
            }
            else
                r = mid;
        }
        return l;
    }

    //Returns the smallest i s.t. A[i] <= value
    int64_t upper_bound(int64_t value) {
        int64_t l = 0, r = roundup(n);
        
        while (l < r) 
        {
            int64_t mid = (l+r)/2;
            if (A[mid]+A[0] >= value)  
                r = mid;
            else {
                l = mid+1;
                value -= A[mid];
            }
        }
        return l;
    }


    void add(T i, T value) {
        if (i != 0) {
            for (; i < n; i+=LSB(i)) 
                A[i] += value;
        } else A[0] += value;
    }

    void swap_with_last(T i) {
        T x = get(i);
        tot -= x;
        T y = get(--n);
        add(i, y-x);
    }

    void swap_with_last(T i, T x, T y) {
        //T x = get(i);
        tot -= x;
        --n;
        //T y = get(--n);
        add(i, y-x);
    }
};
