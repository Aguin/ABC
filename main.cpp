#include <bits/stdc++.h>
using namespace std;

const int SN = 40;
const int limit = 100;
const int MCN = 3000;

const int D = 2;
const double lb = -100;
const double ub = 100;

inline double sq(double x) {return x * x;}
inline double fun(double* x) {
    return 0.5 + (sq(sin(sqrt(sq(x[0]) + sq(x[1])))) - 0.5) / sq(1 + 0.001 * (sq(x[0]) + sq(x[1])));
}
inline double calfit(double f) {
    if (f >= 0) return 1 / (f + 1);
    return 1 + fabs(f);
}

struct Source {
    int trial;
    double x[D], f, fit, prob;
    inline void cal() {
        f = fun(x), fit = calfit(f);
    }
    inline void init() {
        trial = 0;
        for (int i = 0; i < D; ++i)
            x[i] = lb + (ub - lb) * rand() / RAND_MAX;
        cal();
    }
} S[SN], O;

inline void init() {
    for (int i = 0; i < SN; ++i) S[i].init();
    O = S[0];
    for (int i = 1; i < SN; ++i)
        if (S[i].fit > O.fit) O = S[i];
}

inline double get(double l, double r) {
    double phi = 2.0 * (1.0 * rand() / RAND_MAX - 0.5);
    return max(min(l + phi * (r - l), ub), lb);
}

inline void SendEmployedBees() {
    for (int i = 0; i < SN; ++i) {
        Source T = S[i];
        int j = rand() % D;
        int k = (i + 1 + (rand() % (SN - 1))) % SN;
        T.x[j] = get(S[i].x[j], S[k].x[j]), T.cal();
        if (T.fit > S[i].fit) {
            S[i] = T, S[i].trial = 0;
            if (S[i].fit > O.fit) O = S[i];
        } else S[i].trial++;
    }
}

inline void calprob() {
    double sumfit = 0.0;
    for (int i = 0; i < SN; ++i) sumfit += S[i].fit;
    for (int i = 0; i < SN; ++i) S[i].prob = S[i].fit / sumfit;
}

void SendOnlookerBees() {
    calprob();
    int i = 0, cnt = 0;
    while (cnt < SN) {
        double r = rand() / RAND_MAX;
        if (r < S[i].prob) {
            cnt++;
            Source T = S[i];
            int j = rand() % D;
            int k = (i + 1 + (rand() % (SN - 1))) % SN;
            T.x[j] = get(S[i].x[j], S[k].x[j]), T.cal();
            if (T.fit > S[i].fit) {
                S[i] = T, S[i].trial = 0;
                if (S[i].fit > O.fit) O = S[i];
            } else S[i].trial++;
        }
        i = (i + 1) % SN;
    }
}

inline void SendScoutBees() {
    int p = 0;
    for (int i = 1; i < SN; ++i)
        if (S[i].trial > S[p].trial) p = i;
    if (S[p].trial >= limit) S[p].init();
}

int main() {
    srand(time(NULL));
    init();
    for (int it = 0; it < MCN; ++it) {
        SendEmployedBees();
        SendOnlookerBees();
        SendScoutBees();
        printf("%f\n", O.f);
    }
    return 0;
}