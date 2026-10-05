/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80143e60(Object *object) {
    table_8017b5c4[object->field_05](object);
}

void func_80143ea0(Object *object) {
    table_8017b5d4[object->field_06](object);
    func_80131094(object);
    func_80120028(object);
}

void func_80143efc(Object *object) {
    object->pos_x = 0xc0;
    object->pos_y = 0x70;
    object->field_06 = object->field_06 + 1;
    func_80144ee4(object, 0xf);
    func_80143f4c(object);
}

void func_80143f4c(Object *object) {
    if ((u8)object->field_3a) {
        object->field_06 = object->field_06 + 1;
        object->field_3a = object->field_3a & 0xff00;
        func_80120554(0, 0, 0x207);
    }
}
