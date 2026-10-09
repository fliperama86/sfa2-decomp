/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80012a90_slot27(Object *obj) {
    obj->pos_x += (u16)obj->field_4c;
    if (obj->field_50 == obj->pos_x) {
        obj->field_06++;
    }
}
