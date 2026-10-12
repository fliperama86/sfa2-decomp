/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b41c4_slot04_07(Object *obj);

void func_801b01ec_slot04_07(Object *obj) {
    u16 t = obj->step_flags;
    int i;
    Object *c;

    if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        for (i = 0; i < 3; i++) {
            c = (Object *)func_8011f1e0();
            if (c != 0) {
                c->field_00 = 1;
                c->field_02 = 7;
                c->other = obj;
                c->field_03 = i;
                c->spawn_x = obj->spawn_x;
                c->spawn_y = obj->spawn_y;
                c->field_0d = obj->field_0d;
                c->field_08 = 0x20;
                c->field_66 = obj->field_66;
                c->field_90 = obj->field_90;
                c->field_98 = obj->field_98;
                c->field_9c = obj->field_9c;
            }
        }
    }
    func_801b41c4_slot04_07(obj);
    if (obj->vel_y >= 0 || (i = obj->ground_y, obj->pos_y < i)) {
        update_sequence(obj);
    } else {
        obj->state_2 += 1;
        obj->pos_y = i;
        if (obj->side == 0) {
            obj->pos_x = 0x220;
        } else {
            obj->pos_x = 0x2e0;
        }
        obj->vel_x = 0;
        obj->vel_y = 0;
        obj->accel_x = 0;
        obj->accel_y = 0;
        obj->pos_x_frac = 0;
        obj->pos_y_frac = 0;
    }
}
