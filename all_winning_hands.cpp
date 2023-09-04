#include <vector>

#define N_THREES 4

namespace {
typedef bool (*Next)(void *);

struct Three {
    int base = 1;
    int ponchi = 0;
};

bool nextThree(Three *t) {
    if (t->ponchi == 0 && t->base == 34) {
        t->base = 1;
        t->ponchi = 1;
        return false;
    }
    if (t->ponchi == 1) {
        if (t->base == 9*0 + 7) {
            t->base += 3;
            return false;
        }
        if (t->base == 9*1 + 7) {
            t->base += 3;
            return false;
        }
        if (t->base == 9*2 + 7) {
            t->base = 1;
            t->ponchi = 0;
            return true;
        }
    }
    t->base += 1;
    return false;
}

struct Pair {
    int base = 1;
};

bool nextPair(Pair *p) {
    if (p->base == 34) {
        p->base = 1;
        return true;
    }
    p->base += 1;
    return false;
}

struct BasicHand {
    Three t[N_THREES];
    Pair p;
};

bool sortedBasicHand(BasicHand const* h) {
    int pons[4];
    int npons = 0;
    int chis[N_THREES];
    int nchis = 0;
    int pair = h->p.base;
    for (int i = 0; i < N_THREES; ++i) {
        Three const& t = h->t[i];
        if (t.ponchi == 0) {
            if (pair == t.base) return false;
            pons[npons++] = t.base;
        } else {
            chis[nchis++] = t.base;
        }
        if (i < 3) {
            if (t.ponchi == 1 && h->t[i+1].ponchi == 0) {
                return false;
            }
        }
    }
    for (int i = 0; i + 1 < npons; ++i) {
        if (pons[i] >= pons[i+1]) {
            return false;
        }
    }
    for (int i = 0; i + 1 < nchis; ++i) {
        if (chis[i] > chis[i+1]) {
            return false;
        }
        if (i + 2 < nchis && chis[i] == chis[i+1] && chis[i] == chis[i+2]) {
            return false;
        }
    }
    return true;
}

bool haveChiBasicHand(BasicHand const& h) {
    for (int i = 0; i < N_THREES; ++i) {
        Three const& t = h.t[i];
        if (t.ponchi == 1) {
            return true;
        }
    }
    return false;
}

bool onlyChiBasicHand(BasicHand const& h) {
    for (int i = 0; i < N_THREES; ++i) {
        Three const& t = h.t[i];
        if (t.ponchi == 0) {
            return false;
        }
    }
    return true;
}

bool nextSortedBasicHand(BasicHand *h) {
    do {
        int i;
        for (i = N_THREES - 1; i >= 0; --i) {
            bool loopedThree = nextThree(&h->t[i]);
            if (!loopedThree) break;
        }
        if (i == -1) {
            bool loopedPair = nextPair(&h->p);
            if (loopedPair) return true;
        }
    } while (!sortedBasicHand(h));
    return false;
}

struct Iter {
    void *data;
    Next nextFunction;
    bool next() {
        return nextFunction(data);
    }
};

char suite[34 + 1] = "mmmmmmmmmssssssssspppppppppzzzzzzz";
char ranks[34 + 1] = "1234567891234567891234567891234567";
char nice[34 + 1]  = "123456789123456789123456789ESWNHGR";

void printBasicHand(BasicHand& h) {
    Pair& p = h.p;
    int base = p.base - 1;
    char c = nice[base];
    char s = suite[base];
    printf("%c%c%c", c, c, s);
    for (int j = 0; j < N_THREES; ++j) {
        //if (j > 0) printf(" ");
        printf(" ");
        Three& t = h.t[j];
        int base = t.base - 1;
        if (t.ponchi == 0) {
            char c = nice[base];
            char s = suite[base];
            printf("%c%c%c%c", c, c, c, s);
        } else {
            char l = nice[base];
            char m = nice[base + 1];
            char h = nice[base + 2];
            char s = suite[base];
            printf("%c%c%c%c", l, m, h, s);
        }
    }
    printf("\n");
}

void countTiles(BasicHand const& h, int counters[34]) {
    memset((void *)counters, 0, 34 * sizeof(int));
    for (int i = 0; i < N_THREES; ++i) {
        int base = h.t[i].base - 1;
        int ponchi = h.t[i].ponchi;
        if (ponchi == 0) {
            counters[base] += 3;
        } else {
            counters[base] += 1;
            counters[base + 1] += 1;
            counters[base + 2] += 1;
        }
    }
    int base = h.p.base - 1;
    counters[base] += 2;
}

bool possibleBasicHand(BasicHand& h) {
    int counters[34];
    countTiles(h, counters);
    for (int i = 0; i < 34; ++i) {
        if (counters[i] > 4) return false;
    }
    return true;
}

int countSameLookingBasicHands(BasicHand const& h) {
    int const choose4[5] = { 1, 4, 6, 4, 1 };
    int counters[34];
    countTiles(h, counters);
    int nHands = 1;
    for (int i = 0; i < 34; ++i) {
        int n = counters[i];
        if (n > 4) return 0;
        nHands *= choose4[n];
    }
    return nHands;
}

} // namespace

int main() {
    std::vector<BasicHand> hands;
    BasicHand h;

    int possibleBasicHands = 0;
    long long possibleBasicHandsTotal = 0;
    int toitois = 0;
    int onlychis = 0;
    long long toitoisTotal = 0;
    long long onlychisTotal = 0;
    bool looped;
    do {
        bool possible = possibleBasicHand(h);
        if (possible) {
            int sameHands = countSameLookingBasicHands(h);
            possibleBasicHands += 1;
            possibleBasicHandsTotal += sameHands;
            if (!haveChiBasicHand(h)) {
                toitois += 1;
                toitoisTotal += sameHands;
            }
            if (onlyChiBasicHand(h)) {
                onlychis += 1;
                onlychisTotal += sameHands;
            }
            //printBasicHand(h);
            //hands.push_back(h);
        }
        looped = nextSortedBasicHand(&h);
    } while (!looped);
    printf("hands = %d, total = %lld\n", possibleBasicHands, possibleBasicHandsTotal);
    printf("toitois = %d, total = %lld\n", toitois, toitoisTotal);
    printf("onlychis = %d, total = %lld\n", onlychis, onlychisTotal);
    /*
    for (int i = 0; i < (int)hands.size(); ++i) {
        printBasicHand(h);
    }
    */

    return 0;
}
