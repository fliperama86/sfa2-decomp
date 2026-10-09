/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4818_slot04_06(Object *object);
void func_801b4850_slot04_06(Object *object);

void func_801b38d0_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    Object *other;
    GameState *g = &game_state;

    if ((s16)o->field_3a >= 0) {
        func_80130efc(o);
    } else {
        other = o->other;
        other->field_15b = 1;
        if (obj->field_1c5 < 2 || (obj->field_1c5 & 0x80)) {
            func_80140770(o, 0, 0xc, 3, 0, 0, 0);
        } else {
            func_80140770(o, 0, 0xa, 3, 0, 0, 0);
        }
        if ((s16)other->field_5c < 0) {
            o->field_167 = 2;
            if (o->field_49 != 0) {
                o->field_167 = 0x16;
                o->field_255 = 6;
                g->field_6b = 0;
                func_80147000(o);
            }
        }
        other->field_247 = 0;
        o->field_4c = 0xfffe0000;
        o->field_58 = -0x4800;
        o->field_50 = 0x44000;
        o->field_54 = 0;
        o->field_45 = 1;
        o->field_07++;
        func_801307e0(o, 0x26);
    }
}

void func_801b3a00_slot04_06(Object *obj) {
    func_801b4850_slot04_06(obj);
    if (obj->pos_y <= obj->field_70) {
        func_801b4818_slot04_06(obj);
    } else {
        obj->field_45 = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        obj->field_17b = 0;
        func_801312b8(obj);
    }
}
