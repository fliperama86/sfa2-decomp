/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8012f6d8(Object *object);
u16 func_80130470(Object *object);
int func_80130258(Object *object);
void func_80130678(Object *object, int a);

void func_8012b628(Object *object) {
    int t;
    if (game_state.field_4d != 0) {
        func_8012ff80(object);
        return;
    }
    if (func_8012f56c(object)) {
        func_8012f59c(object);
        return;
    }
    if (object->field_cd != 0) {
        if (!func_80149d48(object)) {
            func_801312b8(object);
            return;
        }
        ref_other.p = object->other;
        if (ref_other.p->field_45 != 0) {
            goto skip;
        }
        t = ref_other.p->field_157;
        goto test;
    } else {
        if (func_80130470(object)) {
            func_8013047c(object);
            return;
        }
        if ((u8)func_80130258(object)) {
            func_80130280(object);
            return;
        }
        if (!func_8012f970(object)) {
            object->field_07 = object->field_07 + 1;
            object->field_0b = object->field_158;
            func_80130678(object, 0x17);
            return;
        }
        t = (u8)func_8012f6d8(object);
test:
        if (t) {
            object->field_07 = 2;
            object->field_157 = 1;
            object->field_0b = object->field_158;
            func_80130678(object, 0x19);
            return;
        }
    }
skip:
    object->field_0b = object->field_158;
    func_80130efc(object);
}
