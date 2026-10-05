/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern void set_side_field_0d(Object *object);

void func_8012cca8(Object *object) {
    int k = object->field_60;
    int v;
    if (k == 3) {
        set_side_field_0d(object);
    } else if (k == 5 || k == 1) {
        set_side_field_0d(object);
    }
    v = object->field_6b + 0xff;
    object->field_6b = v;
    if (v & 0x80) {
        object->field_06++;
        object->field_07 = 0;
        if (object->field_24e) {
            object->field_24e = 0;
        }
        object->field_6b = 0;
        func_8012ce5c(object);
        return;
    }
    ref_other.p = object->other;
    if (ref_other.p->field_7e) {
        func_8012f0e0(object);
    }
    if (object->field_45 == 0) {
        v = bytes_801700b0[object->field_6b];
        if (object->field_72 == 0) {
            v = -v;
        }
        object->field_16c = v;
    }
    if (object->field_24e && object->field_d8) {
        if (object->field_60 == 3 ? (game_state.config->field_32 & 1)
                                  : (object->field_60 == 1 && (game_state.config->field_32 & 1))) {
            object->field_0c = 0xff;
            object->field_0d += 4;
            object->field_80++;
            return;
        }
    }
    func_801376b8(object);
}
