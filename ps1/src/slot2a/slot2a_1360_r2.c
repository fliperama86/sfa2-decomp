/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot2aSlot data_801e5164_slot2a[][2];
extern u16 *data_801e52c8_slot2a;

void func_801e14cc_slot2a(Object *obj) {
    int i;
    Cell20 *a;
    Cell20 *b;
    int y;

    a = data_801e5164_slot2a[obj->field_03][0].cells;
    b = data_801e5164_slot2a[obj->field_03][1].cells;
    y = (3 - ((Slot2aSel *)data_801e52c8_slot2a)->field_04) * 8 - 16;
    for (i = 0; i < 2; i++) {
        func_8015c100(a);
        a->field_08 = obj->pos_x + (i << 7);
        a->field_0a = obj->pos_y + y;
        a->field_0c = i << 7;
        a->field_0d = *(u8 *)data_801e52c8_slot2a * 48;
        a->field_10 = 0x80;
        a->field_12 = 0x30;
        a->field_04 = 0x80;
        a->field_05 = 0x80;
        a->field_06 = 0x80;
        a->field_0e = (u16)func_8015bdd4(0x2c0, 0x80);
        a++;
        func_8015c100(b);
        b->field_08 = obj->pos_x + (i << 7);
        b->field_0a = obj->pos_y + y;
        b->field_0c = i << 7;
        b->field_0d = *(u8 *)data_801e52c8_slot2a * 48;
        b->field_10 = 0x80;
        b->field_12 = 0x30;
        b->field_04 = 0x80;
        b->field_05 = 0x80;
        b->field_06 = 0x80;
        b->field_0e = (u16)func_8015bdd4(0x2c0, 0x80);
        b++;
    }
    func_80158a2c((Prim *)a, 0, 0, 0xc, 0);
    func_80158a2c((Prim *)b, 0, 0, 0xc, 0);
}
