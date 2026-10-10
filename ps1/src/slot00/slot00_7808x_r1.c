/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8007a010_slot00[];

/* The box is reached by a byte offset: the count is shifted and then added
   to the address of the boxes. Indexed as an array of 32-byte boxes, this
   function is 4 bytes shorter and differs from the original in 22
   instruction slots.
   One local, v, holds two values in turn: the byte of field_50 before it is
   stepped, and later the index into the log. With a local of its own for the
   byte, or with the byte read in place: 2 slots. With the index written into
   the subscript: 49. The log's field c is read as 16 bits; read as it is: 1. */
void func_80077808_slot00(Object *obj) {
    Object *p;
    u8 t;
    u32 n;
    int v;
    obj->field_00 = 2;
    obj->field_74 = 1;
    n = obj->frame->active;
    p = obj->field_3c;
    if (n == 0) {
        obj->field_74 = 0xff;
        obj->field_60 = 0;
    } else {
        n <<= 5;
        t = ((Box32 *)((u8 *)obj->box_tables->boxes_b + n))->field_12;
        if (obj->field_60 != t) {
            obj->field_00 = 1;
            if (obj->field_67 != 0) {
                obj->field_00 = 2;
                obj->field_67 = 0;
                obj->field_60 = t;
            }
        }
    }
    if (game_state.field_4d == 0 && game_state.field_47 == 0 && p->field_7e == 0 && --((Slot00Obj *)obj)->field_a4 >= 0 && p->field_27a == 0) {
        obj->field_46 = (s16)obj->field_46 - 1;
        if ((s16)obj->field_46 < 0) {
            u32 i;
            u32 k;
            LogRec *rec;
            v = ((Slot00Obj *)obj)->field_50;
            k = (v + 1) & 7;
            i = obj->field_66;
            obj->field_50 = k;
            obj->field_46 = 0;
            obj->field_0d = p->field_0d + data_8007a010_slot00[k];
            v = (table_801aa4d8[i] - obj->field_03 * 8) & 0x1f;
            rec = &table_801ac318[i][v];
            obj->field_0b = rec->b;
            obj->frame = obj->frames + *(u16 *)&rec->c;
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
        obj->field_0d = ((Slot00Obj *)obj)->field_54;
    }
}
