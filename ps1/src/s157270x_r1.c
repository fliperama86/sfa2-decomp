/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80157270(Object *obj) {
    Object *other = obj->field_3c;
    u32 w;

    if (game_state.field_65 == 0) {
        int t = obj->field_46 - 1;
        obj->field_46 = t;
        if (t & 0x8000) {
            obj->field_46 = 0;
            w = *(u32 *)&other->state_0;
            if ((w & 0xffffff) != 0x70001 && (w & 0xffff) != 0x701) {
                obj->state_0 = 2;
                return;
            }
        }
    }
    if (other->field_15a == 8 || game_state.field_13d == 0) {
        if ((s16)obj->field_46 <= 0) {
            func_80120028(obj);
        }
    }
    if (other->field_15a != 8 || game_state.field_13d != 0) {
        obj->field_01 = 0;
        obj->state_0 = 2;
    }
}
