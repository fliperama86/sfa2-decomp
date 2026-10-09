/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern s32 data_8019046c;
extern s32 data_80190464;

u8 func_801416ec(Object *object) {
    if (object->field_7e != 0 && object->field_163 == 0 && *(s16 *)&object->field_5c >= 0 && object->field_45 == 0 &&
        (game_state.config->field_4d | game_state.config->field_4e | game_state.config->field_04) == 0 &&
        func_8012f56c(object) == 0) {
        return 1;
    }
    return 0;
}

int func_80141788(Object *object) {
    if (object->field_7e != 0) return 0;
    data_8019046c = 1;
    return func_80141848(object);
}

u8 func_801417cc(Object *object) {
    s32 *flag = &data_80190464;
    data_8019046c = 0;
    *flag = 0;
    if (*(u32 *)&object->field_04 == 0x20101) return func_80141a00(object);
    *flag = 1;
    if (object->field_45 != 0) return 0;
    return func_80141908(object);
}

u8 func_80141848(Object *object) {
    s32 *flag = &data_80190464;
    *flag = 0;
    if (*(u32 *)&object->field_04 == 0x20101) return func_80141a00(object);
    *flag = 1;
    if (object->field_45 != 0) return 0;
    return func_80141908(object);
}

int func_801418bc(Object *object) {
    game_state.field_35c = 1;
    if (*(u32 *)&object->field_04 == 0x3030001) {
        return 0;
    }
    return (u8)func_80141908(object);
}
