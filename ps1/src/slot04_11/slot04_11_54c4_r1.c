/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7838_slot04_11[];

int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80141e5c(Object *object);
void func_80146478(Object *object, u8 a, int dx, int dy);
void func_80130dc0(Object *object);

void func_801b5874_slot04_11(Object *obj);
Object *func_801b5914_slot04_11(Object *obj);

void func_801b54c4_slot04_11(Object *obj) {
    if ((obj->field_3a << 16) < 0) {
        func_801b5874_slot04_11(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b5508_slot04_11(Object *obj) {
    data_801c7838_slot04_11[obj->field_07](obj);
}

void func_801b5548_slot04_11(Object *obj) {
    obj->field_07++;
    func_80141f28(obj, 3);
    func_80120554(obj, obj->side ^ 1, 0x31a);
    func_801204f4(obj, obj->side, 8);
    obj->field_0b = 0;
    if (obj->field_cd != 0) {
        if (func_801b5914_slot04_11(obj)->pos_x + 0xc0 >= obj->pos_x) {
            goto inc;
        }
    } else if ((obj->field_c2 & 0x8000) == 0) {
inc:
        obj->field_0b++;
    }
    func_801307e0(obj, 0x1b);
    func_80141e5c(obj);
}

void func_801b5614_slot04_11(Object *obj) {
    ref_other.p = obj->other;
    if (obj->field_3a & 4) {
        obj->field_07++;
        func_80140770(obj, 8, 5, 0xf, 0, 0, 1);
    } else if (obj->field_3a & 2) {
        obj->field_3a = obj->field_3a & 0xfffd;
        func_80146478(obj, obj->field_12a >> 1, -0x59, 0x1e);
        func_80120554(obj, obj->side ^ 1, 0x30d);
        func_801204f4(obj, obj->side, 6);
    }
    func_80130efc(obj);
    if (obj->field_3a & 1) {
        func_80141e5c(obj);
    }
}

void func_801b56fc_slot04_11(Object *obj) {
    if ((obj->field_3a << 16) < 0) {
        func_801b5874_slot04_11(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b5740_slot04_11(Object *obj) {
    obj->field_159 = 1;
    func_80130dc0(obj);
}
