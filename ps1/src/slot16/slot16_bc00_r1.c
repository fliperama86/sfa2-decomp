/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8007e868_slot16[];
void func_8007bc84_slot16(Object *obj);
void func_8007bdb4_slot16(Object *obj);
void func_8007bf74_slot16(Object *obj);
void func_801b606c(Object *obj);
void overlay_left_kind(Object *obj);

void func_8007bc00_slot16(Object *obj) {
    if (obj->field_219 != 0) {
        obj->kind = 0x13;
        overlay_left_kind(obj);
        select_box_tables(obj);
        build_metrics(obj);
        func_801b606c(obj);
    } else if (obj->field_128 != 0) {
        func_8007bdb4_slot16(obj);
    } else {
        func_8007bc84_slot16(obj);
    }
}

void func_8007bc84_slot16(Object *obj) {
    obj->field_157 = 0;
    data_8007e868_slot16[obj->field_07](obj);
}

void func_8007bcc4_slot16(Object *obj) {
    obj->field_07++;
    obj->field_0b = obj->field_158;
    if (obj->field_12a == 0 || obj->field_218 == 0 || (u8)func_8013f8c4(obj, -0x17, 0x14) == 0) {
        func_8007bf74_slot16(obj);
    } else {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    }
}
