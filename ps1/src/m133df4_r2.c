/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80135ef0(ObjectView *object) {
    u32 v;
    u8 f = object->field_63;
    if (f & 1) {
        v = f >> 2;
        if (v > 0x10) {
            v = 0x10;
        }
    } else {
        v = 0;
    }
    object->field_92 = v;
    data_8017262c[game_state.field_40]();
    data_801aa544[0].field_8a = data_801725f0[object->field_40 * 3 + 0];
    data_801aa544[1].field_8a = data_801725f0[object->field_40 * 3 + 1];
    data_801aa544[2].field_8a = data_801725f0[object->field_40 * 3 + 2];
    data_80190562[0] = data_801725f0[object->field_40 * 3 + 1];
}
