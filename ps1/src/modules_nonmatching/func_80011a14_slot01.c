/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * (a constant is held in another register and stored at another point).
 * The exact owner of the bytes in the PS1 build stays the raw bytes of the
 * module image; the build does not use this file. The differential test
 * next to it (difftest.py, with func_80011a14_slot01.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): sets up an object as a
 * panel piece. It sets the common fields and the default handlers, then
 * takes one of three shapes by the object's field_03: with bit 7 set it is
 * the piece that shows a picked entry (a byte chosen from tables by the
 * kind and a selector of the object game_state.field_78 points at, or by
 * the random generator func_80151184 when that object's field_2ac is
 * negative), stored in game_state.field_112, and starts an animation with
 * func_80130768; with field_03 non-zero but bit 7 clear it is placed at
 * the left margin; with field_03 zero it is placed at the right margin
 * and, when field_48 is not 0, mirrored (position, field_4c and field_54).
 * The last two shapes record a cursor with func_80011fe8_slot01 and then
 * call func_80011d00_slot01.
 *
 * Contract:
 *   Argument: a0 = pointer to an object (0x394 bytes). No return value.
 *   Reads: the object's field_03, field_04, field_48; game_state.field_78
 *     (the other object), field_0c (s16), field_13a; that object's
 *     field_2ac (s32), field_5c (read as s16) and kind; for the picked
 *     shape the tables data_8002cbfc_slot01, data_8002ce3c_slot01,
 *     data_800192dc_slot01, data_80021170_slot01 and data_80021224_slot01
 *     (indexed by kind); box_margin[0]; the random generator state that
 *     func_80151184 uses.
 *   Writes: the object's field_01, 04, 05, 09, 0b, 0c, 0d, 0e, 20, 24, 90,
 *     98, 9c, pos_x, pos_y, in the right-margin shape also field_46, 4c,
 *     54; game_state.field_112; data_8002121c_slot01[0]; and whatever the
 *     real callees write.
 *   Callees: all run as the original code in both runs (func_80151184 only
 *     steps the random generator in memory; func_80130768 sets an
 *     animation from a table entry; func_80011fe8_slot01 records a cursor
 *     in memory; func_80011d00_slot01 calls func_80130768 with the table
 *     data_800212ac_slot01). The setup builds valid state for them: the
 *     sequence tables get entries pointing at blocks the setup made, and
 *     the object's frame table pointer points at a block.
 *   Aliasing: the object, the other object, the tables' targets and the
 *     sequence blocks are distinct blocks.
 *   Excluded inputs: the other object's field_2ac outside -8..7 (the
 *     tables it indexes are small blocks the setup made), a kind that
 *     indexes beyond the data of the image (the setup uses 0 to 15).
 *   Not reached by any input: none (every instruction slot of the original
 *     is executed).
 *   The setup writes entries of module tables (table pointers at the
 *     object's kind), the same for both runs.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 box_margin[];
extern u8 data_80019330_slot01[];
extern u8 data_800195b8_slot01[];
extern u8 data_80059000[];
extern u8 data_80071668[];
extern u8 data_8002cbfc_slot01[];
extern u8 **data_8002ce3c_slot01[];
extern int *data_800192dc_slot01[];
extern int data_8002121c_slot01[];
extern void *data_80021170_slot01[];
extern SequenceStep *data_80021224_slot01[];
extern Slot01Rec152ec data_800152ec_slot01;
void func_80011d00_slot01(Object *obj, u8 a);
void func_80011fe8_slot01(Object *obj, Slot01Rec152ec *a1);

void func_80011a14_slot01(Object *obj) {
    Object *other = game_state.field_78;
    int sel;
    int s;
    u8 v;

    obj->field_01 = 1;
    obj->field_09 = 1;
    obj->field_98 = data_80019330_slot01;
    obj->field_9c = data_800195b8_slot01;
    obj->field_90 = data_80059000;
    obj->field_05 = 0;
    obj->field_0c = 0;
    obj->field_0d = 0;
    obj->field_0e = 0;
    obj->field_20 = 0;
    obj->field_24 = 0;
    obj->field_0b = 0;
    obj->field_04++;
    if (obj->field_03 & 0x80) {
        if (other->field_2ac < 0) {
            if (game_state.field_0c != 0) {
                v = func_80151184() & 3;
            } else {
                s = 0;
                if ((s16)other->field_5c < 0x80) {
                    s = (s16)other->field_5c < 0xf ? 0x20 : 0x10;
                }
                s += func_80151184() & 0xf;
                sel = ~other->field_2ac;
                v = data_8002ce3c_slot01[other->kind][sel][s];
            }
        } else {
            sel = other->field_2ac;
            s = data_8002cbfc_slot01[other->kind * 24 + game_state.field_13a];
            v = data_8002ce3c_slot01[other->kind][sel][s];
        }
        game_state.field_112 = v;
        obj->field_09 = 0;
        obj->pos_y = 0xa0;
        obj->pos_x = box_margin[0] + 0x18;
        data_8002121c_slot01[0] = data_800192dc_slot01[other->kind][game_state.field_112];
        obj->field_90 = data_80071668;
        obj->field_98 = data_80021170_slot01[other->kind * 2];
        obj->field_9c = data_80021170_slot01[other->kind * 2 + 1];
        func_80130768(obj, 0, data_80021224_slot01);
    } else {
        if (obj->field_03 != 0) {
            obj->pos_x = box_margin[0] + 5;
            obj->pos_y = 0x60;
        } else {
            obj->pos_x = box_margin[0] + 0x17f;
            obj->pos_y = 0x90;
            obj->field_4c = -0xc;
            obj->field_54 = 2;
            obj->field_46 = 0x1f;
            if (obj->field_48 != 0) {
                obj->pos_x = box_margin[0] - 0x175;
                obj->field_4c = 0xc;
                obj->field_54 = -2;
            }
        }
        func_80011fe8_slot01(obj, &data_800152ec_slot01);
        func_80011d00_slot01(obj, 0);
    }
}
