/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"




void func_801299f8(Object *object) {
    func_80129bb8(object);
    func_80129c08(object);
    if (object->field_6b != 0) {
        object->field_6b--;
        if (object->kind == 4) func_80132070(object);
        if (object->kind == 0x12) func_80132070(object);
    } else {
        func_80129b68(object);
        func_80129c40(object);
        func_80129b90(object);
        func_80129c68(object);
        if (object->field_cd != 0 && object->field_256 == 0 &&
            (game_state.config->field_4e | game_state.config->field_04) == 0 &&
            object->field_06 != 9) {
            if (*(u16 *)&object->field_04 != 1 || object->field_06 != 7 ||
                ((object->kind != 6 && object->kind != 0xf) || object->field_15a != 6)) {
                func_8014a1ec(object);
            }
        }
        handler_table_1960[object->field_06](object);
    }
    func_80130b10(object);
}
