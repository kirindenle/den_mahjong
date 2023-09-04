#include <stdint.h>
#include <assert.h>
#include <stdio.h>

#include <intrin.h>

// рука может влезть в одно 64 битное число
// первые 34 бита на то какие тайлы вообще есть, оставшиеся биты разбиты на пары
// и каждая пара означает количество тайлов соответствующего единичного бита
// просто по порядку.

// Минусы
// сброс тайла требует перестроения всего
// нет объявлений
typedef uint64_t u64;
typedef u64 Ordered_Hand;

u64 all_bits  = 0xFFFFFFFFFFFFFFFFULL;
u64 used_bits = 0x3FFFFFFFFFFFFFFFULL; // 62й и 63й биты не пригождаются

// TODO вынести в libs
// строит число, в котором в давпазоне битовый позиций [from, until) стоят единички, а остальные нули
u64 bit_range64(u64 from, u64 until) {
    if (from == until) return 0ULL;
    assert(until <= 64ULL);
    assert(from < until);
    u64 tail = 64ULL - until;
    u64 ret = all_bits;
    (ret >>= from) <<= from;
    (ret <<= tail) >>= tail;
    return ret;
}
u64 reverse64(u64 n) {
    u64 ret = 0;
    for (u64 bit = 0; bit < 64ULL; ++bit) {
        ret |= (((n >> bit) & 1ULL) << (64ULL - bit));
    }
    return ret;
}

u64 count_different_tiles(Ordered_Hand h) {
    u64 ret = __popcnt64(h & ((0x1ULL << 35ULL) - 1ULL));
    assert(ret <= 14ULL);
    return ret;
}

u64 count_tiles(Ordered_Hand h) {
    h &= used_bits;
    u64 cnt = count_different_tiles(h); // так как 0 в секции количеств означает, что тайл всего один, то сначала считаем количество разных тайлов, а уже потом накидываем из счётчиков
    h >>= 34; // пропускаем указатели существования
    while (h) {
        cnt += (h & 0x3ULL);
        h >>= 2ULL;
    }
    return cnt;
}

bool have_tile(Ordered_Hand h, u64 tile) {
    return (h >> tile) & 1ULL;
}

u64 get_tile_pos(Ordered_Hand h, u64 tile) {
    return __popcnt64(h & ((1ULL << tile) - 1ULL));
}

u64 get_tile_counter_from_pos(Ordered_Hand h, u64 pos) {
    return (h >> (34ULL + 2ULL * pos)) & 0x3ULL;
}

u64 set_tile_counter_from_pos(Ordered_Hand h, u64 pos, u64 count) {
    h &= ~(0x3ULL << (34ULL + 2ULL * pos));
    h |= (count << (34ULL + 2ULL * pos));
    return h;
}

u64 maybe_increment_tile_counter_from_pos(Ordered_Hand h, u64 pos) {
    u64 count = get_tile_counter_from_pos(h, pos);
    assert(count <= 3ULL);
    if (count == 3ULL) return h;
    return set_tile_counter_from_pos(h, pos, count + 1ULL);
}

u64 get_tile_count(Ordered_Hand h, u64 tile) {
    assert(0ULL <= tile && tile < 34ULL);
    if (!have_tile(h, tile)) return 0ULL;

    u64 pos = get_tile_pos(h, tile);
    return get_tile_counter_from_pos(h, pos) + 1ULL;
}

Ordered_Hand maybe_add_tile(Ordered_Hand h, u64 tile) {
    if (have_tile(h, tile)) {
        u64 pos = get_tile_pos(h, tile);
        return maybe_increment_tile_counter_from_pos(h, pos);
    }

    u64 n_different = count_different_tiles(h);
    if (n_different == 14ULL) return h;

    h |= (0x1ULL << tile);

    u64 pos = get_tile_pos(h, tile);
    // if (pos == n_different) return h; // исключаем пустой bit range, но это не обязательно

    u64 rest_mask = bit_range64(34ULL + 2ULL * pos, 34ULL + 2ULL * n_different);
    return (h & ~rest_mask) | ((h & rest_mask) << 2ULL);
}


// Если влезать в 68 бит, то можно просто хранить для каждого тайла сколька их в руке, в двух битах по индексу



void tests() {
    Ordered_Hand h = 0ULL;

    Ordered_Hand expected = 0b1000000000000000100000000000000000000000000111111111ULL;
    u64 tiles[13] = {0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 8, 8};
    for (int i = 0; i < 13; ++i) {
        h = maybe_add_tile(h, tiles[i]);
    }
    if (h != expected) {
        printf("ERROR\n");
        for (u64 t = 0; t < 34; ++t) {
            u64 hcnt = get_tile_count(h, t);
            u64 ecnt = get_tile_count(expected, t);
            if (hcnt != ecnt) printf("%llu: got %llu, expected %llu\n", t, hcnt, ecnt);
        }
    }
}

int main() {tests();}
