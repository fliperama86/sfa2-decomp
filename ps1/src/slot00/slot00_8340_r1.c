/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80078458_slot00(Object *obj, int index);

void func_80078340_slot00(Object *obj) {
    obj->field_46 = 0x10;
    obj->field_50 = 0x60;
    obj->field_58 = 0x60;
    if (obj->field_0b == 0) {
        obj->field_4c = 0xfff40000;
        obj->field_54 = 0xc000;
        obj->pos_x = obj->pos_x + 0x60;
    } else {
        obj->field_4c = 0xc0000;
        obj->field_54 = 0xffff4000;
        obj->pos_x = obj->pos_x - 0x60;
    }
    func_80078458_slot00(obj, 0xc);
}

void func_800783c4_slot00(Object *obj) {
    if (obj->field_0b == 0) {
        obj->field_4c = 0xffff0000;
        obj->pos_x = obj->pos_x + 0x38;
    } else {
        obj->field_4c = 0x10000;
        obj->pos_x = obj->pos_x - 0x38;
    }
    obj->field_50 = 0x20000;
    obj->field_54 = 0;
    obj->field_58 = -0x2000;
    obj->pos_y = obj->pos_y - 0x49;
    func_80078458_slot00(obj, 9);
}

void func_80078438_slot00(Object *obj) {
    func_80078458_slot00(obj, 0xb);
}

void func_80078458_slot00(Object *obj, int index) {
    SequenceStep **table;
    if (obj->field_66 == 0) {
        table = data_1f8000ac;
    } else {
        table = data_1f80015c;
    }
    func_80130768(obj, (s16)index, table);
}
