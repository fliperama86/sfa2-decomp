/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../externs.h"

void func_801307e0(Object *object, int arg);
void func_80141f28(Object *object, short delta);
void func_801312b8(Object *object);
void func_80131468(Object *obj);
extern s16 data_8007e878_slot16[];
void overlay_left_kind(Object *object);
void select_box_tables(Object *object);
void build_metrics(Object *object);
void func_80130dc0(Object *object);
void func_801b63fc(Object *object);

void func_8007be8c_slot16(Object *object) {
    int t;

    if (object->attack_param_d != 0) {
        object->kind = 0x13;
        overlay_left_kind(object);
        select_box_tables(object);
        build_metrics(object);
        func_801b63fc(object);
        return;
    }
    if (object->limit_lo & 1) {
        object->input_dir_second = 2;
    }
    object->state_3 = 3;
    object->input_dir_0 = 4;
    object->active_flag_3 = 1;
    func_80141f28(object, *(s16 *) ((char *) data_8007e878_slot16 + (object->direction_index & 0xfe)));
    t = 0xc;
    if (object->variant_index != 0) {
        t = 0x12;
    }
    if (object->input_dir_second != 0) {
        t += 3;
    }
    func_801307e0(object, (object->direction_index >> 1) + t);
}

void func_8007bf74_slot16(Object *object) {
    object->active_flag_3 = 1;
    func_80130dc0(object);
}

void func_8007bf98_slot16(Object *object) {
    func_801312b8(object);
}

void func_8007bfb8_slot16(Object *obj) {
    func_80131468(obj);
}
