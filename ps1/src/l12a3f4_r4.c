/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u16 func_80130470(Object *object);
int func_80130258(Object *object);
int func_8012f7c0(Object *object);
int func_8012f618(Object *object);
void func_80130678(Object *object, int index);

void func_8012b184(Object *object) {
    u16 saved;
    if (object->field_cd != 0) {
        func_8012b2c4(object);
    } else if (func_80130470(object)) {
        func_8013047c(object);
    } else if ((u8)func_80130258(object)) {
        func_80130280(object);
    } else if ((u8)func_8012f7c0(object)) {
        func_8012f838(object);
    } else if ((u8)func_8012f618(object)) {
        object->field_07 = object->field_07 + 1;
        saved = object->field_38;
        func_80130678(object, (u8)object->field_3a + 0xd);
        object->field_38 = saved;
    } else if (func_8012f970(object)) {
        func_8012fe60(object);
    } else if ((s16)object->field_3a < 0) {
        func_801312b8(object);
    } else {
        object->field_0b = object->field_158;
        func_80130efc(object);
    }
}
