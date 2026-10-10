/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code has another size and another register choice and
 * scheduling. The exact owner of the bytes in the
 * PS1 build stays the raw bytes of the module image; the build does not
 * use this file. The differential test next to it (difftest.py, with
 * func_80012dec_slot01.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): turns a list of sprite
 * parts into drawing primitives. The object's sequence step points at a
 * description: a count, a list of 4-byte texture records and a list of
 * 4-byte position records. For each part it fills one 0x28-byte primitive
 * with four corner positions (the part's position plus the object's
 * pos_x and pos_y, and a size of 16 times the texture record's second and
 * third bytes) and four texture coordinates (from the first byte: its low
 * nibble times 16 as u, its high nibble as v, each plus the size), then
 * links the primitive into the ordering list whose index is
 * data_80015780_slot01[object->field_09].
 *
 * Contract:
 *   Arguments: a0 = pointer to an object, a1 = pointer to an array of
 *     primitives (Poly28, 0x28 bytes each, as many as the count). No
 *     return value.
 *   Reads: the object's field_09, pos_x, pos_y and sequence; the step's
 *     field_04 (the description); the description's s16 count at offset 0,
 *     texture-record pointer at 4 and position-record pointer at 8; the
 *     table data_80015780_slot01 entry at index field_09; the list base
 *     pointer data_801987c8 and the list head word it selects.
 *   Texture record, 4 bytes: byte 0 texture byte, bytes 1 unused, byte 2
 *     width in units of 16, byte 3 height in units of 16. Position record,
 *     4 bytes: u16 x, u16 y.
 *   Writes: for each part the primitive's offsets 0x08, 0x0a, 0x0c, 0x0d,
 *     0x10, 0x12, 0x14, 0x15, 0x18, 0x1a, 0x1c, 0x1d, 0x20, 0x22, 0x24,
 *     0x25, and the first word (the link: low 24 bits) of the primitive
 *     and of the list head. The list head is written once per part.
 *   Aliasing: the object, the description, texture records, position
 *     records, the primitive array and the list array are distinct blocks.
 *   Excluded inputs: a count of 0 (the loop is a do-while that counts down
 *     from the count and would run 2^32 times), and a negative count.
 *   The setup writes data_80015780_slot01 at the index field_09, to a
 *     small index so that the list head lies in the list array it
 *     builds; this is module data, written the same for both runs.
 *   Not reached by any input: none (every instruction slot of the original is executed).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_80015780_slot01[];

/* The description a sequence step points at. Inferred from the code; not
   an original declaration. */
typedef struct {
    s16 count;
    u8 pad[2];
    u8 *tex;
    u16 *pts;
} Slot01Parts;

void func_80012dec_slot01(Object *obj, unsigned char *p_arg) {
    Poly28 *p = (Poly28 *)p_arg;
    Slot01Parts *parts = (Slot01Parts *)obj->sequence->field_04;
    u32 *ot = (u32 *)data_801987c8 + data_80015780_slot01[obj->field_09];
    u8 *tex = parts->tex;
    u16 *pts = parts->pts;
    int n = parts->count;

    do {
        int tu = (tex[0] & 0xf) << 4;
        int tv = tex[0] & 0xf0;
        int w = tex[2] << 4;
        int h = tex[3] << 4;
        int x = pts[0] + (u16)obj->pos_x;
        int y = pts[1] + (u16)obj->pos_y;

        tex += 4;
        pts += 2;
        p->field_08 = x;
        p->field_0a = y;
        p->field_10 = x + w;
        p->field_12 = y;
        p->field_18 = x;
        p->field_1a = y + h;
        p->field_20 = x + w;
        p->field_22 = y + h;
        p->field_0c = tu;
        p->field_0d = tv;
        p->field_14 = tu + w;
        p->field_15 = tv;
        p->field_1c = tu;
        p->field_1d = tv + h;
        p->field_24 = tu + w;
        p->field_25 = tv + h;
        ((PrimTag *)p)->addr = ((PrimTag *)ot)->addr;
        ((PrimTag *)ot)->addr = (u32)p;
        p++;
    } while (--n);
}
