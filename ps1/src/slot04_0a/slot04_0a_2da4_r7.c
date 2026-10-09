/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3e68_slot04_0a(Object *obj);

void func_801b3528_slot04_0a(Object *obj) {
    Object *o;
    int dx;
    int dy;

    if (((s16)obj->field_3a & 0x8000) == 0) {
        func_80130efc(obj);
    } else {
        o = obj->other;
        dx = obj->pos_x - o->pos_x;
        if (dx < 0) {
            dx = -dx;
        }
        dy = obj->pos_y - o->pos_y;
        if (dy < 0) {
            dy = -dy;
        }
        if (dx >= 0xa1 || dy >= 0x61 || obj->field_241 != 0) {
            func_801b3e68_slot04_0a(obj);
            obj->field_07 = 3;
            obj->field_27b = 8;
            func_801307e0(obj, 0x4a);
        } else {
            obj->field_27b = 0xf;
            obj->field_07++;
            func_801204f4(obj, obj->side, 0xf);
            func_801307e0(obj, 0x49);
        }
    }
}
