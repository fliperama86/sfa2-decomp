/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8013054c(u8 *out, Object *object);

int func_80141618(Object *object) {
    u8 buf[8];
    if ((game_state.config->field_4d | game_state.config->field_04) != 0 || (func_8012f56c(object) & 0xff) != 0 ||
        (object->field_134 & 0xfc) == 0) {
        object->field_254 = 0;
    } else {
        object->field_0b = object->field_158;
        func_8013054c(buf, object);
        if (object->field_12a != buf[0]) {
            object->field_254 = 0;
        } else {
            object->field_129 = buf[1];
            object->field_128 = 0;
            if (object->field_130 & 0x4000) object->field_128 = 2;
            object->field_16a = 0;
            func_80142c04(object);
            object->field_254 = 1;
            return 1;
        }
    }
    return 0;
}
