/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../externs.h"

void func_801307e0(Object *object, int index);
void func_80141f28(Object *object, short delta);
void func_801312b8(Object *object);
void func_80131468(void);
extern s16 data_8007e878_slot16[];
void overlay_left_kind(Object *object);
void select_box_tables(Object *object);
void build_metrics(Object *object);
void func_80130dc0(Object *object);
void func_801b63fc(Object *object);

void func_8007be8c_slot16(Object *object) {
    int t;

    if (object->field_219 != 0) {
        object->kind = 0x13;
        overlay_left_kind(object);
        select_box_tables(object);
        build_metrics(object);
        func_801b63fc(object);
        return;
    }
    if (object->field_211 & 1) {
        object->field_129 = 2;
    }
    object->field_07 = 3;
    object->field_128 = 4;
    object->field_159 = 1;
    func_80141f28(object, *(s16 *) ((char *) data_8007e878_slot16 + (object->field_12a & 0xfe)));
    t = 0xc;
    if (object->field_48 != 0) {
        t = 0x12;
    }
    if (object->field_129 != 0) {
        t += 3;
    }
    func_801307e0(object, (object->field_12a >> 1) + t);
}

void func_8007bf74_slot16(Object *object) {
    object->field_159 = 1;
    func_80130dc0(object);
}

void func_8007bf98_slot16(Object *object) {
    func_801312b8(object);
}

void func_8007bfb8_slot16(void) {
    func_80131468();
}
