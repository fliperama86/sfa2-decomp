/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

typedef struct { u8 b[9]; } Tbl9;

extern u32 *data_8002b614_slot12;
extern u32 *data_8002b618_slot12;
extern u8 data_8002ab94_slot12[];
int func_8015bdd4(int a, int b);

/* What it does (inferred, not an original name): handles a pointer (data_8002b614_slot12) that walks a buffer of 20-byte primitives. Called with a == 0xff it steps the pointer back by one primitive and links that primitive's tag to the address held in the word that data_8002b618_slot12 points at, then sets that word to the buffer data_8002ab94_slot12 plus 0x540 bytes times data_801a27d0. Called with any other a it initializes the primitive at the pointer, sets its position from x, y and the object's position, looks a up in a table of nine bytes and, when found, sets its texture bytes from the index, gives it colour 0x80 and size 16 by 16, sets its halfword at 0xe from a library call, steps the pointer on by one primitive and links the finished primitive's tag to it. */
void func_80013224_slot12(Object *obj, u8 a, s16 x, s16 y) {
    /* The bytes are written as the string constant of twelve bytes whose first nine are the table: the original has the bytes 00 01 00 after the table at 0x80010021 (no instruction reads them) and the build compares them. The struct of nine bytes copies the nine the function uses; read this way a PC compiler gets the same nine bytes (a string constant of bytes, nothing depends on alignment or the character set, the bytes are written as hex escapes). */
    /* Stand-in, not recovered source: this string constant of twelve bytes reproduces the original's read-only data (the three bytes 00 01 00 behind the table). Without it this function differs from the original in 0 instruction slots (the read-only range of the unit then differs from the original in 1 byte and the image check fails). */
    Tbl9 tbl = *(Tbl9 *)"\x4d\x4e\x4f\x5d\x5e\x5f\x69\x6a\x6b\0\1";
    int i;

    if (a != 0xff) {
        u32 *p;
        func_8015c100((Cell20 *)data_8002b614_slot12);
        ((Cell20 *)data_8002b614_slot12)->field_08 = x + (u16)obj->pos_x;
        ((Cell20 *)data_8002b614_slot12)->field_0a = y + (u16)obj->pos_y;
        for (i = 0; i < 9; i++) {
            if (tbl.b[i] == a) {
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
        p = data_8002b614_slot12;
        data_8002b614_slot12 = (p + 5);
        ((PrimTag *)p)->addr = (u32)data_8002b614_slot12;
    } else {
        u32 *p = data_8002b614_slot12;
        u32 *ot = data_8002b618_slot12;
        data_8002b614_slot12 = (p - 5);
        ((PrimTag *)(p - 5))->addr = 0xffffff;
        ((PrimTag *)(p - 5))->addr = ((PrimTag *)ot)->addr;
        ((PrimTag *)ot)->addr = (u32)(data_8002ab94_slot12 + data_801a27d0 * 0x540);
    }
}
