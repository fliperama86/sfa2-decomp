/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7a10_slot04_11[];
extern ObjectFn data_801c7a1c_slot04_11[];
extern ObjectFn data_801c7a24_slot04_11[];

void func_80142a14(Object *object);

void func_801b5f64_slot04_11(Object *obj);
void func_801b5f88_slot04_11(Object *obj);
int func_801b5fc8_slot04_11(Object *obj);
void func_801b604c_slot04_11(Object *obj);

void func_801b5b3c_slot04_11(Object *obj) {
    data_801c7a10_slot04_11[obj->input_dir_0 >> 1](obj);
}

void func_801b5b80_slot04_11(Object *obj) {
    obj->field_157 = 0;
    data_801c7a1c_slot04_11[obj->state_3](obj);
}

void func_801b5bc0_slot04_11(Object *obj) {
    obj->state_3++;
    if (obj->direction_index == 0) {
        func_801b5f64_slot04_11(obj);
    } else if (obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) && (func_8013f8c4(obj, -0x14, 0xe) & 0xff)) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        func_801b5f64_slot04_11(obj);
        if (obj->input_dir_second == 0 && obj->direction_index != 0) {
            obj->field_278 = 1;
            obj->sequence_hold_flag = 1;
        }
        func_801b5f88_slot04_11(obj);
    }
}

void func_801b5c94_slot04_11(Object *obj) {
    func_80142a14(obj);
    func_801b5f88_slot04_11(obj);
}

void func_801b5cc4_slot04_11(Object *obj) {
    obj->field_157 = 1;
    data_801c7a24_slot04_11[obj->state_3](obj);
}

void func_801b5d08_slot04_11(Object *obj) {
    obj->state_3++;
    func_801b5f64_slot04_11(obj);
    if (obj->input_dir_second != 0) {
        if (obj->direction_index == 4) {
            obj->field_278 = 1;
            func_801b5f88_slot04_11(obj);
        }
    } else if (obj->direction_index == 4) {
        obj->field_29c = 1;
        obj->sequence_hold_flag = 1;
    }
}

void func_801b5d8c_slot04_11(Object *obj) {
    func_80142a14(obj);
    func_801b5f88_slot04_11(obj);
}

void func_801b5dbc_slot04_11(Object *obj) {
    if (obj->active_flag != 0 && *(u8 *)&obj->step_flags != 0 && (obj->pressed_bits & 8)) {
        func_801307e0(obj, obj->variant_index != 0 ? 0x19 : 0x18);
    } else if (func_801b5fc8_slot04_11(obj) < 0 && obj->pos_y >= obj->ground_y) {
        func_801b604c_slot04_11(obj);
        func_801209c4(obj);
    } else {
        update_sequence(obj);
    }
}
