/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * build keeps the original bytes of the resident executable and does not use
 * this file. The differential test next to it (difftest.py, with
 * func_8013bfa4.py) compares the behavior of this C with the original code
 * on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): for each object of the
 * object table (data_8018f5e0, read downward, count_8018f59c entries) of
 * type 1 it picks a player object (player_left, or player_right when the
 * object's field_65 is 0), skips it when the player is in a state that
 * ignores hits, and tests the player's three box sets (boxes_a, boxes_b,
 * boxes_c, each indexed by the player's frame) against the box of the
 * table object's current frame (func_801397d0). Each box set that overlaps
 * adds its bit (1, 2, 4) to data_80188f30 and records the overlap's
 * corners and size in data_80188ed0. When any overlapped, the object hits the
 * player: the callees apply the hit, the player and an effect object get
 * state bytes, and the table object counts the hit (field_00 + 1, a
 * countdown in field_5c with two state bytes sets).
 *
 * Contract (the roles named for the fields are inferred):
 *   No argument, no return value.
 *   Reads: count_8018f59c (16 bits, signed), the table data_8018f5e0 (words
 *     at 0, -4, -8, ... from its start); the table object's field_00,
 *     field_02, field_49, field_5c, field_65, frame (active, field_04),
 *     box_tables (boxes_b), and, through func_801397d0, its field_08,
 *     field_0b, pos_x, pos_y and wide_boxes; the player's field_163,
 *     field_263, field_27b, field_295, field_69, field_45, field_61,
 *     field_5c, boxes_a, boxes_b, boxes_c, frame (box_a, box_b, box_c);
 *     data_80190468 (a pointer to an object whose bytes at 0x5c to 0x5e it
 *     writes).
 *   Writes: ref_other, ref_third, ref_second, ref_first, data_80190458,
 *     data_801904c0, data_80188ed0 (the in_ and f_ fields), data_80188f30;
 *     the player's field_241 and field_61; the table object's field_00,
 *     field_04 to field_07, field_5c; three bytes at data_80190468 + 0x5c.
 *   Callees that run as the original code: func_80139a1c (clears the
 *     overlap record) and func_801397d0 (computes the overlap).
 *     Callees replaced by recorders, which return 0: func_80139a78 (a, b),
 *     func_8013a3a8 (a, b, box), func_8013af1c (a, b, box), func_80155de0
 *     (box, b). What they do to the objects is outside the test. The
 *     original ignores the result of func_80155de0 (it tests a zero it has
 *     just set), and so does this C.
 *   Watched by the recorders at every call, whole: the table objects, both
 *     players, the object at data_80190468, data_80188ed0, data_80188f30,
 *     ref_first to data_80190414 and the word after it, data_80190458,
 *     data_801904c0, ref_other and ref_third.
 *   Aliasing: the table objects, the players, their frames, box tables and
 *     box lists, and the object at data_80190468 are distinct blocks; the
 *     table may repeat an object.
 *   Excluded: a count above the size of the table (the original reads
 *     the memory below the symbol; the table holds at most 12 entries).
 *   Not reached by any input: none expected; see the coverage line.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"
#include "../object.h"

/* Globals as the original uses them (inferred types; not original
   declarations). */
extern ObjectRef ref_third;
/* data_80190458 is an ObjectRef in the shared header; the function uses its
   word as a pointer to Box6 records. */
extern BoxTables *data_801904c0;

/* The object that data_80190468 points at: three state bytes at 0x5c.
   Inferred from the code. */
typedef struct {
    u8 pad[0x5c];
    u8 state;
    u8 mode;
    u8 mode2;
} StateBytes;
/* data_80190468 is an ObjectRef in the shared header; its word points at this
   object. */

/* The size of an overlap: the distance between two 16-bit coordinates, as
   the original computes it (a negative 16-bit difference is negated). */
static u16 distance(u16 a, u16 b) {
    u16 d = a - b;
    if ((s16)d < 0) d = -d;
    return d;
}

/* Copies the overlap just computed into one set of fields of data_80188ed0
   and adds the set's bit to data_80188f30. */
static void record_hit(u8 bit, u16 *x0, u16 *x1, u16 *y0, u16 *y1, s16 *size) {
    u16 sum;

    data_80188f30 = data_80188f30 + bit;
    *x0 = data_80188ed0.out_3c;
    *x1 = data_80188ed0.out_40;
    sum = distance(data_80188ed0.out_3c, data_80188ed0.out_40);
    *y0 = data_80188ed0.out_44;
    *y1 = data_80188ed0.out_48;
    *size = sum;
    *size = distance(data_80188ed0.out_44, data_80188ed0.out_48) + (u16)*size;
}

void func_8013bfa4(void) {
    s16 last;
    s16 i;
    Object *player;
    Box32 *box;
    Object *obj;

    if (count_8018f59c == 0) return;
    last = count_8018f59c - 1;
    ((ObjectStackRef *)&ref_other)->pp = (Object **)data_8018f5e0;
    for (i = 0; i <= last; i++) {
        ref_third.p = *((ObjectStackRef *)&ref_other)->pp--;
        obj = ref_third.p;
        if (obj->field_00 != 1) continue;
        ref_second.p = &player_left;
        if (obj->field_65 == 0) ref_second.p = &player_right;
        player = ref_second.p;
        if (player->field_263 != 0) continue;
        if (player->field_27b != 0) continue;
        if (player->field_163 != 0) continue;
        if (obj->field_49 == 0 || player->field_295 == 0) {
            if (obj->frame->field_04 < player->field_69) continue;
        }
        if (obj->frame->active == 0) continue;
        data_80190458.p = (Object *)obj->box_tables;
        box = obj->box_tables->boxes_b;
        data_801904c0 = obj->box_tables;
        box = (Box32 *)((u8 *)box + obj->frame->active * sizeof(Box32));
        ref_first.p = (Object *)box;
        func_80139a1c();
        if (player->frame->box_a != 0) {
            if (func_801397d0((s32)obj, player, player->boxes_a + player->frame->box_a) == 0) {
                record_hit(1, &data_80188ed0.f_0c, &data_80188ed0.f_24, &data_80188ed0.f_10,
                           &data_80188ed0.f_28, &data_80188ed0.in_00);
            }
        }
        if (player->frame->box_b != 0) {
            if (func_801397d0((s32)obj, player, player->boxes_b + player->frame->box_b) == 0) {
                record_hit(2, &data_80188ed0.f_14, &data_80188ed0.f_2c, &data_80188ed0.f_18,
                           &data_80188ed0.f_30, &data_80188ed0.in_04);
            }
        }
        if (player->frame->box_c != 0) {
            if (func_801397d0((s32)obj, player, player->boxes_c + player->frame->box_c) == 0) {
                record_hit(4, &data_80188ed0.f_1c, &data_80188ed0.f_34, &data_80188ed0.f_20,
                           &data_80188ed0.f_38, &data_80188ed0.in_08);
            }
        }
        if (data_80188f30 == 0) continue;
        player->field_241 = 1;
        func_80139a78(obj, player);
        if (obj->field_02 == 0x17) {
            player->field_241 = 0;
            if (player->field_61 != 0xff) {
                if (player->field_45 == 0) player->field_61 = 0;
            }
        }
        func_8013a3a8(obj, player, box);
        if (player->field_61 != 0xff && (s16)player->field_5c >= 0) {
            if ((0x800000 >> (obj->field_02 & 31)) != 0) {
                ((StateBytes *)data_80190468.p)->state = 0xf;
                ((StateBytes *)data_80190468.p)->mode = 2;
                ((StateBytes *)data_80190468.p)->mode2 = 2;
            }
            if (obj->field_49 != 0) {
                ((StateBytes *)data_80190468.p)->state = 7;
                ((StateBytes *)data_80190468.p)->mode = 1;
                ((StateBytes *)data_80190468.p)->mode2 = 1;
            }
        }
        func_8013af1c(obj, player, box);
        func_80155de0((Actor *)box, player);
        obj->field_00 = obj->field_00 + 1;
        obj->field_04 = 1;
        obj->field_05 = 1;
        obj->field_06 = 0;
        obj->field_07 = 0;
        obj->field_5c = obj->field_5c - 1;
        if ((s16)obj->field_5c < 0) {
            obj->field_04 = 2;
            obj->field_05 = 0;
            obj->field_06 = 0;
            obj->field_07 = 0;
        }
    }
}
