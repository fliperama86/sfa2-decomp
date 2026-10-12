/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_801c7844_slot04_11[];
extern ObjectFn data_801c7854_slot04_11[];

void select_box_tables(Object *object);
void build_metrics(Object *object);
void func_80131468(Object *object);
void func_80131638(Object *object);
void set_sequence_by_side(Object *object, int arg);
unsigned short func_80130470(Object *object);

void func_801b5874_slot04_11(Object *obj);
Object *func_801b5914_slot04_11(Object *obj);
int func_801b58d4_slot04_11(Object *obj);
int func_801b58f4_slot04_11(Object *obj);
void func_801b5ab8_slot04_11(Object *obj);

int func_801b580c_slot04_11(Object *obj) {
    int r;

    if (obj->kind == 0x11) {
        if (obj->flags_28b & 1) {
            goto y;
        }
x:
        r = func_801b58f4_slot04_11(obj);
        goto done;
    } else if (obj->flags_28b & 1) {
        goto x;
    }
y:
    r = func_801b58d4_slot04_11(obj);
done:
    return r;
}

void func_801b5874_slot04_11(Object *obj) {
    func_801312b8(obj);
}

void func_801b5894_slot04_11(Object *obj) {
    func_80131468(obj);
}

void func_801b58b4_slot04_11(Object *obj) {
    func_80131638(obj);
}

int func_801b58d4_slot04_11(Object *obj) {
    return ((obj->pressed_bits | obj->released_bits) & 0x94) == 0x94;
}

int func_801b58f4_slot04_11(Object *obj) {
    return ((obj->pressed_bits | obj->released_bits) & 0x68) == 0x68;
}

Object *func_801b5914_slot04_11(Object *obj) {
    return data_801c7844_slot04_11[obj->field_0e];
}

void func_801b5938_slot04_11(Object *obj) {
    if (obj->kind == 0x11 || obj->kind == 0x13) {
        data_801c7854_slot04_11[obj->state_3](obj);
    }
}

void func_801b5990_slot04_11(Object *obj) {
    obj->state_3++;
    obj->flags_28b = 0;
    obj->kind ^= 2;
    func_801b5ab8_slot04_11(obj);
    select_box_tables(obj);
    build_metrics(obj);
    set_sequence_by_side(obj, 0x30);
}

void func_801b59f0_slot04_11(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_801312b8(obj);
        return;
    }
    if (obj->mode_flag_2 == 0) {
        if (func_80130470(obj) & 0xffff) {
            func_8013047c(obj);
            return;
        }
        if (func_8012f970(obj)) {
            func_8012fe60(obj);
            return;
        }
    }
    if ((u8)func_8012f898(obj)) {
        func_8012f8c4(obj);
    } else {
        update_sequence(obj);
    }
}
