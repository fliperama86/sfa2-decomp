/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8012f898(Object *object);
u16 func_80130470(Object *object);
int func_80130258(Object *object);
int func_8012f618(Object *object);
int func_8012f7c0(Object *object);
void func_80130678(Object *object, int index);

void func_8012a3f4(Object *object) {
    u16 saved;
    if (game_state.field_4d != 0) {
        func_8012ff80(object);
    } else if (func_8012f56c(object)) {
        func_8012f59c(object);
    } else if (object->field_cd != 0) {
        if ((u8)func_8012f898(object)) {
            func_8012f8c4(object);
        } else if ((s16)object->field_3a < 0) {
            func_801312b8(object);
        } else {
            func_80130efc(object);
        }
    } else if (func_80130470(object)) {
        func_8013047c(object);
    } else if ((u8)func_80130258(object)) {
        func_80130280(object);
    } else if (func_8012f970(object)) {
        func_8012fe60(object);
    } else if (!(u8)func_8012f618(object)) {
        if ((u8)func_8012f7c0(object)) {
            func_8012f838(object);
        }
        if ((u8)func_8012f898(object)) {
            func_8012f8c4(object);
        }
        if ((s16)object->field_3a < 0) {
            func_801312b8(object);
        } else {
            func_80130efc(object);
        }
    } else {
        saved = object->field_38;
        object->field_07 = 0;
        func_80130678(object, (u8)object->field_3a + 7);
        object->field_38 = saved;
    }
}
