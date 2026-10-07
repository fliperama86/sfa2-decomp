/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef34_slot2b;

void func_8007893c_slot2b(Object *obj);
void func_80079208_slot2b(Object *obj);
void func_80079144_slot2b(Object *obj);

void func_80078f8c_slot2b(Object *o) {
    Slot2bObj *obj = (Slot2bObj *)o;
    int d;
    obj->field_47--;
    if ((obj->field_47 & 0x80) != 0) {
        o->field_05++;
        if (data_8007ef34_slot2b->other->field_15b != 0) {
            d = (func_80151184() & 0x3f) - 0x20;
            d += *(u16 *)&o->pos_x;
            o->pos_x = d;
            d = (func_80151184() & 0x3f) - 0x20;
            *(u16 *)&o->pos_y -= d;
            o->field_48 = (game_state.field_24 & 1) + 0x51;
            func_8007893c_slot2b(o);
            func_801204f4(o, data_8007ef34_slot2b->side, 0xf);
        } else {
            o->field_05++;
            obj->field_47 = 0x18;
            func_80079208_slot2b(o);
        }
    } else {
        func_80079144_slot2b(o);
        func_80131094(o);
    }
}

void func_8007909c_slot2b(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_04++;
    } else {
        func_80131094(obj);
    }
}
