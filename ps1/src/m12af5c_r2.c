/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

unsigned short func_80130470(Object *object);
unsigned char func_80130258(Object *object);
unsigned char func_8012f800(Object *object);
void func_80130678(Object *object, int index);

void func_8012b7b8(Object *object) {
    if (object->field_cd != 0) {
        if ((s16)object->field_3a < 0) {
            func_801312b8(object);
            return;
        }
    } else {
        if (func_80130470(object) != 0) {
            func_8013047c(object);
            return;
        }
        if (func_80130258(object) != 0) {
            func_80130280(object);
            return;
        }
        if (func_8012f970(object) != 0) {
            object->field_07 = 0;
            object->field_157 = 0;
            object->field_0b = object->field_158;
            func_80130678(object, 0x16);
            return;
        }
        if (func_8012f800(object) != 0) {
            func_8012f838(object);
            return;
        }
        if ((s16)object->field_3a < 0) {
            func_801312b8(object);
            return;
        }
    }
    object->field_0b = object->field_158;
    func_80130efc(object);
}
