/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4db8_slot04_0b(Object *obj, int index);
void func_801b4e0c_slot04_0b(Object *obj, int dx, int dy);

void func_801b4928_slot04_0b(Object *obj) {
    if (!(ref_other.p->step_flags & 1)) {
        obj->state_1++;
        if (obj->field_0b != 0) {
            obj->vel_x = 0xfff80000;
        } else {
            obj->vel_x = 0x80000;
        }
        obj->vel_y = 0x20000;
        obj->accel_x = 0;
        obj->accel_y = -0x4000;
        func_801b4e0c_slot04_0b(obj, -0x44, 0x38);
        func_801b4db8_slot04_0b(obj, 0x18);
    }
    func_8011ffdc(obj);
}

void func_801b49c0_slot04_0b(Object *obj) {
    GameState *g = &game_state;
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_x += obj->accel_x;
    obj->vel_y += obj->accel_y;
    if (obj->vel_y < 0) {
        if (!(obj->pos_y < ref_other.p->field_70)) {
            obj->state_0++;
        }
    }
    func_8011ff74(obj);
    if (g->field_1d & 1) {
        func_8011ffdc(obj);
    }
}
