#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <vector>

typedef unsigned long long u64;
typedef unsigned int       u32;
typedef unsigned short     u16;
typedef unsigned char      u8;

// numbers
u8 One   = 0;
u8 Two   = 1;
u8 Three = 2;
u8 Four  = 3;
u8 Five  = 4;
u8 Six   = 5;
u8 Seven = 6;
u8 Eight = 7;
u8 Nine  = 8;

u8 East  = 0;
u8 South = 1;
u8 West  = 2;
u8 North = 3;
u8 Ton = East;
u8 Nan = South;
u8 Sha = West;
u8 Pei = North;

u8 White = 0;
u8 Green = 1;
u8 Red   = 2;
u8 Haku  = White;
u8 Hatsu = Green;
u8 Chun  = Red;

// types
u8 Dots       = 1;
u8 Bamboo     = 2;
u8 Characters = 3;
u8 Honormask = ~(0x3);
u8 Wind   = 4;
u8 Dragon = 5;
u8 Pin = Dots;
u8 So  = Bamboo;
u8 Man = Characters;

// flags
u8 Akadora = 0x1;

u8 n_numbers(u8 type) {
    if (!(type & Honormask)) return 9;
    if (type == Wind) return 4;
    if (type == Dragon) return 3;
    assert(false);
    return 0;
}

struct Tile {
    Tile() {}
    Tile(u8 n, u8 t, u8 f = 0) : number(n), type(t), flags(f) {
        assert(n < n_numbers(t));
    }
    u8 number = 0;
    u8 type = 0;
    u8 flags = 0;
};

Tile dora_from_indicator(Tile t) {
    t.number = (t.number + 1) % n_numbers(t.type);
    t.flags  = 0;
    return t;
}

char* show(Tile t) {
    static char buffer[100] = {};
    char* c = buffer;
    if (!(t.type & Honormask)) {
        *c++ = '1' + t.number;
        *c++ = "_psmwd"[t.type];
    }
    if (t.type == Wind) *c++ = "ESWN"[t.number];
    if (t.type == Dragon) *c++ = "HGR"[t.number];
    *c = 0;
    return buffer;
}

#define N_TILES 136
Tile tiles[N_TILES + 8];
void init_tiles() {
    memset(tiles, 0, sizeof tiles);
    int idx = 0;
    for (u8 type = Dots; type <= Dragon; ++type) {
        u8 n = n_numbers(type);
        for (u8 i = 0; i < n; ++i) {
            tiles[idx++] = Tile(i, type, (!(type & Honormask) && n == Five ? Akadora : 0));
            tiles[idx++] = Tile(i, type);
            tiles[idx++] = Tile(i, type);
            tiles[idx++] = Tile(i, type);
        }
    }
    printf("tiles = %d\n", idx);
    for (int i = 0; i < idx; ++i) {
        printf("(%d,%s)", i, show(tiles[i]));
        if (i % 4 == 3) puts("");
        else printf(" ");
    }
}

struct Hand {
    Tile* draw = NULL;
    Tile* in_hand[13];
    int   n_in_hand;
    std::vector<Tile*> discard;
};

struct Game {
    Tile wall[N_TILES];
    Hand hands[4];
    int n_players = 4;
};

void deal(Game *game) {
    for (int idx = 0; idx < N_TILES; ++idx) {
    }
}


int main() {
    init_tiles();
    Tile t;
    printf("%llu 0x%0+16llx\n", sizeof t, *(u64*)&t);
    return 0;
}
