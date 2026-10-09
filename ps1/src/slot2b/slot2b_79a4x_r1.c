/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern FrameRecord *data_1f8000a8;
extern FrameRecord *data_1f800158;
extern u16 data_80079910_slot2b[];
extern s32 data_80079920_slot2b[];
extern u8 data_8007991c_slot2b[];
extern u8 data_800798f8_slot2b[];
void func_80077afc_slot2b(Object *obj);

/* The local k holds the byte field_ac, then the half index idx that is passed to func_80138070: written with a local for each, this function differs from the original in 30 instruction slots. The store of 0 to field_50 before the store of speed_y stays: without it 2 instruction slots differ. The locals dy and speed_y hold the table values before the stores: written in place, 24 and 28 instruction slots differ. */
void func_800779a4_slot2b(Object *obj) {
    Object *p;
    u8 k = obj->field_ac;
    int i = (s8)k;
    int idx;
    int speed_x;
    int speed_y;
    int dx;
    int dy;
    obj->field_09 = 0;
    obj->field_04++;
    p = obj->field_3c;
    obj->field_1c = p->field_1c;
    obj->field_1e = p->field_1e;
    obj->field_70 = p->field_70;
    obj->box_tables = (BoxTables *)data_800798f8_slot2b;
    obj->field_67 = 0;
    obj->field_45 = 0;
    obj->field_50 = 0;
    dx = data_80079910_slot2b[i];
    speed_x = data_80079920_slot2b[i >> 1];
    dy = data_80079910_slot2b[i + 1];
    speed_y = data_80079920_slot2b[(i >> 1) + 3];
    if (obj->field_0b == 0) {
        speed_x = -speed_x;
        dx = -dx;
    }
    obj->field_4c = speed_x;
    obj->field_50 = speed_y;
    obj->pos_x = dx + obj->pos_x;
    obj->pos_y -= dy;
    idx = obj->field_ac >> 1;
    obj->field_a0 = data_8007991c_slot2b[idx];
    k = idx;
    if (p->side == 0) {
        obj->frames = data_1f8000a8;
    } else {
        obj->frames = data_1f800158;
    }
    func_80138070(obj, k);
    func_80077afc_slot2b(obj);
}
