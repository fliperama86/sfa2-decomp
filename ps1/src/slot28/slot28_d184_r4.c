/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001d984_slot28(Object *obj) {
    *(int *)&obj->field_10 = *(int *)&obj->field_10 - 0x80000;
    if (obj->pos_x < 0xe1) {
        obj->pos_x = 0xe8;
        obj->field_05 = obj->field_05 + 1;
    }
}
