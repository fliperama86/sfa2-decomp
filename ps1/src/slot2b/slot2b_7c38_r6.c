/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079a18_slot2b[];
extern u8 data_80079a00_slot2b[];
extern Object *data_8007ef28_slot2b;
void func_80078280_slot2b(Object *o);

void func_80078168_slot2b(Object *o) {
    Slot2bObj *s = (Slot2bObj *)o;
    o->field_a0 = 0xff;
    o->state_0++;
    o->field_1c = data_8007ef28_slot2b->field_1c;
    o->field_1e = data_8007ef28_slot2b->field_1e;
    s->box_tables = (BoxTables *)data_80079a00_slot2b;
    o->field_5c = 0xfff;
    o->field_45 = 0;
    o->field_09 = 0;
    o->vel_x = 0x12000;
    o->accel_x = 0;
    o->vel_y = 0x100000;
    o->accel_y = 0xfffe8000;
    if (o->field_0b == 0) {
        o->vel_x = -o->vel_x;
        o->accel_x = -o->accel_x;
    }
    o->pos_x += 8;
    o->pos_y -= 1;
    if (data_8007ef28_slot2b->side == 0) {
        o->frames = data_1f8000a8;
    } else {
        o->frames = data_1f800158;
    }
    func_80138070(o, 4);
    func_80078280_slot2b(o);
}

void func_80078280_slot2b(Object *o) {
    data_80079a18_slot2b[o->state_1](o);
}
