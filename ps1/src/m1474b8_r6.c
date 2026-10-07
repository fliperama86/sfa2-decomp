/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80148d48(Object *object) {
    if ((s16)object->field_3a < 0) {
        object->field_04 = object->field_04 + 1;
    } else {
        s32 *wobble = (s32 *)data_8019045c;

        object->field_46 = object->field_46 + 1;
        *(s32 *)&object->field_14 = *(s32 *)&object->field_14 - object->field_50;
        object->field_50 = object->field_50 + object->field_58;
        *(s32 *)&object->field_10 = object->field_4c;
        object->field_54 = object->field_54 + 0xc000;
        *wobble = (object->field_54 & 0xffff0000) | (s16)object->field_46;
        *(u8 *)wobble = *(u8 *)wobble + game_state.field_24;
        if ((*(u8 *)wobble & 1) != 0) {
            *wobble = -*wobble;
        }
        *(s32 *)&object->field_10 = *(s32 *)&object->field_10 + *wobble;
        func_80131094(object);
        func_80120028(object);
    }
}
