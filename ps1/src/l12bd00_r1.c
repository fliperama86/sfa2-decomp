/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012bd00(Object *object) {
    int v;
    object->field_15b = 0;
    if (object->kind == 0x14) {
        func_8012bf74(object);
        return;
    }
    if (game_state.config->field_4d == 0 && !func_8012f56c(object)) {
        v = object->field_46 - 1;
        object->field_46 = v;
        if ((s16)v != -1) {
            if ((u8)func_8012be70(object)) {
                func_80130efc(object);
                v = object->field_46 - 1;
                object->field_46 = v;
                if ((s16)v == -1) goto tail;
                if ((u8)func_8012bec0(object)) {
                    func_80130efc(object);
                    v = object->field_46 - 1;
                    object->field_46 = v;
                    if ((s16)v == -1) goto tail;
                }
            }
            if ((u8)func_8012bf00(object)) {
                func_80130efc(object);
                v = object->field_46 - 1;
                object->field_46 = v;
                if ((s16)v == -1) goto tail;
            }
            func_80130efc(object);
            return;
        }
    }
tail:
    object->field_07 = object->field_07 + 1;
}
