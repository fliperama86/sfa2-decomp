/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800e2348_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    int up = 0;
    int i;
    if (o->pos_x_frac & 0x5000) {
        if (o->pos_x_frac & 0x1000) {
            up = 1;
            obj->field_0b--;
            if (obj->field_0b < 0) {
                obj->field_0b = 2;
            }
        } else {
            obj->field_0b = obj->field_0b + 1;
            if (obj->field_0b >= 3) {
                obj->field_0b = 0;
            }
        }
        if (obj->field_0d != 0) {
            play_sound_at((Object *)0, 0, 0x204);
        }
    }
    if (obj->field_0b != 2) {
        if (obj->field_0d == 0) {
            obj->field_0b = 2;
        } else {
            for (i = obj->field_0b; i < 2; i++) {
                if (!(obj->field_0d & (i + 1))) {
                    if (up == 0) {
                        obj->field_0b = obj->field_0b + 1;
                    } else {
                        obj->field_0b--;
                    }
                } else {
                    i = 2;
                }
                if (obj->field_0b < 0) {
                    obj->field_0b = 2;
                }
            }
        }
    }
}
