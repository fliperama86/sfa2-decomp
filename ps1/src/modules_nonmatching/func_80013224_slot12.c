/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code is 548 bytes against the original's 556 (the original stores
 * the tag of the previous primitive twice in its a == 0xff arm; here only
 * the final store is written). The exact owner of the bytes in the PS1
 * build stays the raw bytes of the module image; the build does not use
 * this file. The differential test next to it (difftest.py, with
 * func_80013224_slot12.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): handles a pointer
 * (data_8002b614_slot12) that walks a buffer of 20-byte primitives. Called
 * with a == 0xff it steps the pointer back by one primitive and links that
 * primitive's tag to the word that data_8002b618_slot12 points at, then
 * makes that word point at the buffer data_8002ab94_slot12 plus 0x540
 * bytes times the selector data_801a27d0. Called with any other a it
 * initializes the primitive at the pointer through the library, sets its
 * position (x and y plus the object's pos_x and pos_y), looks a up in a
 * table of nine bytes and, when found, sets its texture bytes from the
 * index (index * 16, and 0x30), gives it colour 0x80 0x80 0x80 and size
 * 16 by 16, sets its halfword at 0xe from a library call (for object
 * field_48 equal 9: 0x1ed, else 0x1ec minus field_48), and then steps the
 * pointer on by one primitive and links the tag of the finished primitive
 * to that new position.
 *
 * Contract:
 *   Arguments: a0 = object (0x394 bytes in the test), a1 = a (only the low
 *     byte is used; the test sets the upper bits at random), a2 = x and
 *     a3 = y (full words, random; only the low 16 bits of each sum are
 *     kept). No return value.
 *   Reads: object pos_x, pos_y (halfwords) and field_48 (byte); the pointer
 *     words data_8002b614_slot12 and data_8002b618_slot12; the tag word at
 *     the pointer's primitive (or the one before it); the word that
 *     data_8002b618_slot12 points at; data_801a27d0 (a word).
 *   Writes: for a not 0xff: the pointer data_8002b614_slot12 and, in the
 *     primitive, the tag's 24 low bits and bytes 4 to 6, 8 to 0x13 (except
 *     0xc and 0xd when a is not in the table). For a equal 0xff: the
 *     pointer, the 24 low bits of the tag of the previous primitive and the
 *     24 low bits of the word behind data_8002b618_slot12.
 *   Callees: func_8015c100 (Sony's library, 1 argument: the primitive) and
 *     func_8015bdd4 (Sony's library, 2 arguments: 0 and the value),
 *     replaced by recorders. The result of the second is random per case
 *     (it is stored in the primitive). The log watches, at every call, the
 *     primitive (5 words) and the pointer word data_8002b614_slot12.
 *   Aliasing: the object, the primitive buffer and the word behind
 *     data_8002b618_slot12 are distinct blocks. (The original stores the
 *     previous tag twice in its a == 0xff arm; the first store is
 *     overwritten by the second and is seen only when the two alias.)
 *   Not reached: nothing is excluded; every instruction slot is executed.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 *data_8002b614_slot12;
extern u32 *data_8002b618_slot12;
extern u8 data_8002ab94_slot12[];
int func_8015bdd4(int a, int b);

void func_80013224_slot12(Object *obj, u8 a, s16 x, s16 y) {
    u8 tbl[9] = {0x4d, 0x4e, 0x4f, 0x5d, 0x5e, 0x5f, 0x69, 0x6a, 0x6b};
    int i;

    if (a != 0xff) {
        u32 *p;
        func_8015c100(data_8002b614_slot12);
        ((Cell20 *)data_8002b614_slot12)->field_08 = x + (u16)obj->pos_x;
        ((Cell20 *)data_8002b614_slot12)->field_0a = y + (u16)obj->pos_y;
        for (i = 0; i < 9; i++) {
            if (tbl[i] == a) {
                ((Cell20 *)data_8002b614_slot12)->field_0c = i << 4;
                ((Cell20 *)data_8002b614_slot12)->field_0d = 0x30;
                break;
            }
        }
        ((Cell20 *)data_8002b614_slot12)->field_10 = 0x10;
        ((Cell20 *)data_8002b614_slot12)->field_12 = 0x10;
        ((Cell20 *)data_8002b614_slot12)->field_04 = 0x80;
        ((Cell20 *)data_8002b614_slot12)->field_05 = 0x80;
        ((Cell20 *)data_8002b614_slot12)->field_06 = 0x80;
        if (obj->field_48 == 9) {
            ((Cell20 *)data_8002b614_slot12)->field_0e = func_8015bdd4(0, 0x1ed);
        } else {
            ((Cell20 *)data_8002b614_slot12)->field_0e = func_8015bdd4(0, 0x1ec - obj->field_48);
        }
        p = (u32 *)data_8002b614_slot12;
        data_8002b614_slot12 = p + 5;
        ((PrimTag *)p)->addr = (u32)data_8002b614_slot12;
    } else {
        u32 *p = (u32 *)data_8002b614_slot12;
        u32 *ot = data_8002b618_slot12;
        data_8002b614_slot12 = p - 5;
        ((PrimTag *)(p - 5))->addr = ((PrimTag *)ot)->addr;
        ((PrimTag *)ot)->addr = (u32)(data_8002ab94_slot12 + data_801a27d0 * 0x540);
    }
}
