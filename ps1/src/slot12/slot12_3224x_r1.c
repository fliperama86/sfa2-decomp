/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

typedef struct { u8 b[9]; } Tbl9;

extern Cell20 *data_8002b614_slot12;
extern u32 *data_8002b618_slot12;
extern u8 data_8002ab94_slot12[];
int func_8015bdd4(int a, int b);

/* What it does (inferred, not an original name): handles a pointer (data_8002b614_slot12) that walks a buffer of 20-byte primitives. Called with a == 0xff it steps the pointer back by one primitive and links that primitive's tag to the address held in the word that data_8002b618_slot12 points at, then sets that word to the buffer data_8002ab94_slot12 plus 0x540 bytes times data_801a27d0. Called with any other a it initializes the primitive at the pointer, sets its position from x, y and the object's position, looks a up in a table of nine bytes and, when found, sets its texture bytes from the index, gives it colour 0x80 and size 16 by 16, sets its halfword at 0xe from a library call, steps the pointer on by one primitive and links the finished primitive's tag to it. */
void func_80013224_slot12(Object *obj, u8 a, s16 x, s16 y) {
    /* Exception, not recovered source: the table of nine bytes is read from a string constant of twelve bytes, "MNO]^_ijk" and the bytes 0, 1, 0, through a struct of the nine bytes the function copies. In the original the bytes 00 01 00 follow the table at 0x80010021 (the function reads only the nine; the compiler aligns every object it emits, so only one constant of twelve bytes puts those three there). With a plain array of nine bytes the function's instructions are the same, but the three bytes after the table are zeros: the image then differs from the original in 1 byte. */
    Tbl9 tbl = *(Tbl9 *)"MNO]^_ijk\0\1";
    int i;

    if (a != 0xff) {
        u32 *p;
        func_8015c100(data_8002b614_slot12);
        data_8002b614_slot12->field_08 = x + (u16)obj->pos_x;
        data_8002b614_slot12->field_0a = y + (u16)obj->pos_y;
        for (i = 0; i < 9; i++) {
            if (tbl.b[i] == a) {
                data_8002b614_slot12->field_0c = i << 4;
                data_8002b614_slot12->field_0d = 0x30;
                break;
            }
        }
        data_8002b614_slot12->field_10 = 0x10;
        data_8002b614_slot12->field_12 = 0x10;
        data_8002b614_slot12->field_04 = 0x80;
        data_8002b614_slot12->field_05 = 0x80;
        data_8002b614_slot12->field_06 = 0x80;
        if (obj->field_48 == 9) {
            data_8002b614_slot12->field_0e = func_8015bdd4(0, 0x1ed);
        } else {
            data_8002b614_slot12->field_0e = func_8015bdd4(0, 0x1ec - obj->field_48);
        }
        p = (u32 *)data_8002b614_slot12;
        data_8002b614_slot12 = (Cell20 *)(p + 5);
        ((PrimTag *)p)->addr = (u32)data_8002b614_slot12;
    } else {
        u32 *p = (u32 *)data_8002b614_slot12;
        u32 *ot = data_8002b618_slot12;
        data_8002b614_slot12 = (Cell20 *)(p - 5);
        ((PrimTag *)(p - 5))->addr = 0xffffff;
        ((PrimTag *)(p - 5))->addr = ((PrimTag *)ot)->addr;
        ((PrimTag *)ot)->addr = (u32)(data_8002ab94_slot12 + data_801a27d0 * 0x540);
    }
}
