/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012b8dc(Object *object) {
    if (game_state.field_4d != 0) {
        func_8012ff80(object);
    } else if (func_8012f56c(object)) {
        func_8012f59c(object);
    } else if (object->field_cd != 0) {
        if (!func_80149d48(object)) {
            func_80131468(object);
        } else {
            ref_other.p = object->other;
            if (ref_other.p->field_157 == 0) goto reset;
            if (ref_other.p->field_45 == 0) goto copy;
            goto reset;
        }
    } else if (func_80130470(object)) {
        func_8013047c(object);
    } else if ((u8)func_80130258(object)) {
        func_80130280(object);
    } else if (!func_8012f970(object)) {
        object->field_07++;
        object->field_0b = object->field_158;
        func_80130678(object, 0x1a);
    } else if ((u8)func_8012f618(object)) {
        goto copy;
    } else {
reset:
        object->field_0b = object->field_158;
        object->field_07 = 0;
        object->field_157 = 0;
        func_80130678(object, 0x16);
    }
    return;
copy:
    object->field_0b = object->field_158;
    func_80130efc(object);
}
