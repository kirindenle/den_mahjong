int suit_to_shift[3] = {0, 9, 18};
char ready_hands[20000000][34];
unsigned long long n = 0;

#define NORMAL 0
#define PAIRS 1
#define ORPHANS 2

bool is_last_chii(int chii, int suit) {
    return chii == 7 && suit == 2;
}

struct hand {
    int  type;
    int  pair;
    int  n_chiis;
    int  suits[4];
    int  chiis[4];
    int  pons[4];
    bool iskan[4];
    int  pairs[7];
    bool is_last_normal() {
        if (type == NORMAL && pair == 33 && n_chiis == 4) {
            for (int i = 0; i < n_chiis; ++i) {
                if (!is_last_chii(chiis[i], suits[i])) return false;
            }
            return true;
        }
    }
    void next() {
        if (is_last_normal()) {
            type = PAIRS;
            for (int i = 0; i < 7; ++i) pairs[i] = i;
            return;
        }
        if (type == NORMAL) {

        }

    }
};

int main() {
    for (int pair = 0; pair < 34; ++pair) {
        for (unsigned char n_chiis = 0; n_chiis != 5; ++n_chiis) {
            for (int triple = 0; triple < 4; ++triple) {
                if (triple < n_chiis) {
                    for (int color = 0; color != 4; ++color) {
                        for (int middle = 2-1; middle != 9-1; ++middle) {

                        }
                    }
                } else {
                    for (int pon = 0; pon < 34; ++pon) {
                        for (int iskan = 0; iskan != 2; ++iskan) {

                        }
                    }
                }
            }
        }
    }
}
