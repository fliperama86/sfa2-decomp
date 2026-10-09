/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * and register choice (the load of field_66 is placed later, so the
 * object and the constant 1 exchange registers). The exact owner of the
 * bytes in the PS1 build stays the raw bytes of the module image; the build
 * does not use this file. The differential test next to it
 * (func_80077808_slot00.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): the per-frame update of a
 * follower object (a trail element of the object that field_3c points at).
 * It first sets field_00 to 2 and field_74 to 1, then reads the "active"
 * byte of the current frame record: 0 means field_74 = 0xff and field_60 =
 * 0; otherwise it looks up a byte in the wide box table indexed by that
 * value and, when it differs from field_60, sets field_00 to 1, and if
 * field_67 is set, field_00 back to 2, clears field_67 and takes the byte
 * for field_60. Then, unless one of the game_state bytes field_4d and
 * field_47, the leader's field_7e or field_27a is non-zero, or the
 * countdown field_a4 (decremented here) went negative, it decrements the
 * halfword field_46; when that goes negative it resets it to 0 and advances
 * a ring index (field_50, modulo 8), sets field_0d from the leader's plus a
 * byte of data_8007a010_slot00, and loads from the record table
 * table_801ac318 (row field_66, entry selected by table_801aa4d8 minus
 * field_03 * 8, modulo 32) the fields field_0b, the frame pointer
 * (frames + 16 * index), pos_x and pos_y; then it either clears field_01
 * (game_state.field_226 not 0) or calls func_80120028. In the skipped case
 * it instead sets field_00 and field_04 to 2, clears field_05, field_06,
 * field_07 and field_74 and copies field_54 to field_0d.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: game_state.field_4d, field_47, field_226; the object's field_03,
 *     field_3c (leader), field_46, field_50 (byte), field_54, field_60,
 *     field_66, field_67, field_a4 (s32), frame, frames, box_tables;
 *     frame->active; byte 0x12 of the 32-byte entry (frame->active * 32)
 *     of box_tables->boxes_b (the entry is read only when active is not
 *     0); the leader's field_0d, field_7e, field_27a; data_8007a010_slot00
 *     at the ring index; table_801aa4d8[field_66]; the record
 *     table_801ac318[field_66][index] (12 bytes: a byte at 2, a word at 4
 *     of which the low half is used, halves at 8 and 0xa).
 *   Writes: field_00, field_74, field_60, field_67, field_a4, field_46,
 *     field_50 (a whole word), field_0d, field_0b, frame, pos_x, pos_y,
 *     field_01, field_04, field_05, field_06, field_07.
 *   Callee replaced by a recorder: func_80120028 (1 argument: the object;
 *     it returns nothing used). The log watches the object (0x394 bytes) and
 *     the leader (0x394 bytes) at the call. The frame pointer is only
 *     stored, never read through, so the frames block is not dereferenced.
 *   Aliasing: the object, the leader, the frame record and the box table
 *     are distinct blocks.
 *   Excluded inputs: none; the setup keeps field_66 below 16 so that the
 *     tables read are bytes the setup filled in.
 *   Slots no input reaches: none known; see the coverage line.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8007a010_slot00[];

void func_80077808_slot00(Object *obj) {
    Slot00Obj *state = (Slot00Obj *)obj;
    Object *leader;
    u8 active;
    u8 wanted;
    u32 ring;
    LogRec *rec;

    obj->field_00 = 2;
    obj->field_74 = 1;
    active = obj->frame->active;
    leader = obj->field_3c;
    if (active == 0) {
        obj->field_74 = 0xff;
        obj->field_60 = 0;
    } else {
        wanted = ((Box32 *)((u8 *)obj->box_tables->boxes_b + active * 32))->field_12;
        if (obj->field_60 != wanted) {
            obj->field_00 = 1;
            if (obj->field_67 != 0) {
                obj->field_00 = 2;
                obj->field_67 = 0;
                obj->field_60 = wanted;
            }
        }
    }
    if (game_state.field_4d == 0 && game_state.field_47 == 0 && leader->field_7e == 0 &&
        --state->field_a4 >= 0 && leader->field_27a == 0) {
        obj->field_46 = (s16)obj->field_46 - 1;
        if ((s16)obj->field_46 < 0) {
            obj->field_46 = 0;
            ring = (state->field_50 + 1) & 7;
            obj->field_50 = ring;
            obj->field_0d = leader->field_0d + data_8007a010_slot00[ring];
            rec = &table_801ac318[obj->field_66][(table_801aa4d8[obj->field_66] - obj->field_03 * 8) & 0x1f];
            obj->field_0b = rec->b;
            obj->frame = obj->frames + (u16)rec->c;
            obj->pos_x = rec->d;
            obj->pos_y = rec->e;
            if (game_state.field_226 != 0) {
                obj->field_01 = 0;
            } else {
                func_80120028(obj);
            }
        }
    } else {
        obj->field_00 = 2;
        obj->field_04 = 2;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
        obj->field_74 = 0;
        obj->field_0d = state->field_54;
    }
}
