/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c7798_slot04_11[];
extern u8 data_801c77a8_slot04_11[];
extern ObjectFn data_801c77b8_slot04_11[];

void func_801b5874_slot04_11(Object *obj);
void func_801b5938_slot04_11(Object *obj);
u8 func_801b49b8_slot04_11(Object *obj, int a);
u8 func_801b49e8_slot04_11(Object *obj);
int func_801b5764_slot04_11(Object *obj);
void func_801b4970_slot04_11(Object *obj);

void func_801b4824_slot04_11(Object *obj) {
    Config *c;

    func_801b4970_slot04_11(obj);
    func_801b5764_slot04_11(obj);
    if (obj->pos_y >= obj->field_70) {
        obj->pos_y = obj->field_70;
        obj->field_14 = 0;
        obj->field_45 = 0;
        func_801209c4(obj);
        if (obj->field_12e < obj->field_12f && ((c = game_state.config)->field_4d | c->field_4e | c->field_04) == 0 && func_8012f56c(obj) == 0) {
            obj->field_07 = 4;
            func_801307e0(obj, 0x4c);
        } else {
            obj->field_07++;
            func_801307e0(obj, 0x6f);
        }
    } else {
        func_80130efc(obj);
    }
}

void func_801b4910_slot04_11(Object *obj) {
    if ((obj->field_3a << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b5874_slot04_11(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b4970_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (o->field_cd == 0) {
        if (*(u16 *)&obj->field_1c0 == 0) {
            if (o->field_134 != 0) {
                *(u16 *)&obj->field_1c0 = o->field_134 & 0x68;
            }
        }
    }
}

u8 func_801b49b8_slot04_11(Object *obj, int a) {
    if (obj->field_0b != 0) {
        return data_801c7798_slot04_11[a >> 3];
    }
    return a;
}

u8 func_801b49e8_slot04_11(Object *obj) {
    return data_801c77a8_slot04_11[func_80151184() & 0xf];
}

void func_801b4a1c_slot04_11(Object *obj) {
    func_801b5938_slot04_11(obj);
}

void func_801b4a3c_slot04_11(Object *obj) {
    data_801c77b8_slot04_11[obj->field_07](obj);
}

void func_801b4a7c_slot04_11(Object *obj) {
    obj->field_17b = 1;
    obj->field_07++;
    if (obj->field_49 == 0) {
        obj->field_177--;
    }
    func_801307e0(obj, 0x20);
}
