/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80147584(Object *object) {
    func_80131094(object);
    if (game_state.field_64 != 0) {
        if (*(u8 *)&object->field_3a != 0) return;
        if (object->field_a2 == 0) game_state.field_dc = 0;
        if (data_8018945c >= 0x10) {
            object->field_a0 = 2;
            object->field_a1 = 0x17;
            data_8018945c = 0;
        }
        func_801477ac(object, object->field_a0, data_8018945c + object->field_a1);
        data_80189458 = data_80189458 + 1;
        if (data_80189458 >= 6) {
            data_8018945c = data_8018945c + 1;
            if (object->field_a0 == 2 && data_8018945c >= 5) data_8018945c = 4;
            data_80189458 = 0;
        }
    } else {
        game_state.field_dc = data_80189454;
        object->field_04 = 2;
    }
}
