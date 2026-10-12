/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8011f38c(Object *o);

void func_801b4228_slot04_12(Object *obj);
void func_801b42d0_slot04_12(Object *obj);
void func_801b4310_slot04_12(Object *obj);
void func_801b4350_slot04_12(Object *obj);
void func_801b43d8_slot04_12(Object *obj);
void func_801b4440_slot04_12(Object *obj);
void func_801b4480_slot04_12(Object *obj);
void func_801b44dc_slot04_12(Object *obj);
void func_801b4544_slot04_12(Object *obj);
void func_801b4a24_slot04_12(Object *obj);
void func_801b4ba8_slot04_12(Object *obj);

void func_801b4228_slot04_12(Object *obj) {
    obj->state_0 = obj->state_0 + 1;
}

void func_801b423c_slot04_12(Object *obj) {
    Object *p = obj->parent;

    if (obj->table_index == 0) {
        *(s32 *)&p->linked_object_2 = 0;
    } else {
        p->linked_object = 0;
    }
    func_8011f38c(obj);
}

void func_801b4278_slot04_12(Object *obj) {
    if (obj->input_dir_0 == 4) {
        func_801b4a24_slot04_12(obj);
    } else if (obj->input_dir_0 != 0) {
        func_801b4544_slot04_12(obj);
    } else {
        func_801b42d0_slot04_12(obj);
    }
}

void func_801b42d0_slot04_12(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        func_801b4440_slot04_12(obj);
    } else {
        func_801b4310_slot04_12(obj);
    }
}

void func_801b4310_slot04_12(Object *obj) {
    if (obj->state_3 == 0) {
        func_801b4350_slot04_12(obj);
    } else {
        func_801b43d8_slot04_12(obj);
    }
}

void func_801b4350_slot04_12(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)func_8013f8c4(obj, -0x14, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        func_801b4ba8_slot04_12(obj);
    }
}

void func_801b43d8_slot04_12(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b4440_slot04_12(Object *obj) {
    if (obj->state_3 == 0) {
        func_801b4480_slot04_12(obj);
    } else {
        func_801b44dc_slot04_12(obj);
    }
}
