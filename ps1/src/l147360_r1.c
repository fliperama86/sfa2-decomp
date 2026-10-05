/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80147360(Object *object) {
    u8 a;
    u8 b;
    func_80147968(object);
    if (game_state.field_64 != 0) {
        if (*(u8 *)&object->field_3a != 0) return;
        game_state.field_dc = 0;
        if (data_8018945c != 0) {
            int v = object->field_45 >> 1;
            if (v == 0) {
                a = 0;
                b = 0;
            } else if (v == 1) {
                a = 0;
                b = 0;
            } else if (v == 2) {
                a = 0;
                b = 0x10;
            } else if (v == 3) {
                a = 1;
                b = 0x10;
            }
            func_801477ac(object, a, data_80189458 + b);
            data_80189458 = data_80189458 + 1;
            if (data_80189458 == 0x10) data_80189458 = 0;
        }
    } else {
        game_state.field_dc = data_80189454;
        object->field_04 = object->field_04 + 1;
    }
    data_8018945c = (data_8018945c + 1) & 1;
}
