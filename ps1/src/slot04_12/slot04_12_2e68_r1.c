/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Block172 *func_8011f1e0(void);

void func_801b2e68_slot04_12(Object *obj) {
    Object *p;
    s16 t;
    u16 f;

    p = (Object *)obj->field_28;
    if ((s16)p->step_flags & 0x8000) {
        t = obj->step_flags;
        if (t & 0x8000) {
            obj->state_3++;
            func_801307e0(obj, obj->field_12a + 0x5b);
            return;
        }
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            p = (Object *)func_8011f1e0();
            if (p != 0) {
                p->field_00 = 1;
                p->field_02 = 0xf;
                p->field_03 = 1;
                p->ot_depth = 2;
                p->field_0e = obj->field_0e;
                p->field_0b = obj->field_0b;
                p->field_26 = obj->field_26;
                p->field_0c = obj->field_0c;
                p->field_0d = obj->field_0d;
                p->pos_x = obj->pos_x;
                p->pos_y = obj->pos_y;
                p->ground_y = obj->ground_y;
                p->field_1c = obj->field_1c;
                p->spawn_x = obj->spawn_x;
                p->spawn_y = obj->spawn_y;
                p->field_90 = obj->field_90;
                p->field_98 = obj->field_98;
                p->field_9c = obj->field_9c;
                p->field_08 = 0x20;
                p->field_66 = obj->field_66;
                p->field_3c = obj;
                p = (Object *)func_8011f1e0();
                if (p != 0) {
                    p->field_00 = 1;
                    p->field_02 = 0xf;
                    p->field_03 = 0;
                    p->ot_depth = 0;
                    p->field_0e = obj->field_0e;
                    p->field_0b = obj->field_0b;
                    p->field_26 = obj->field_26;
                    p->pos_x = obj->pos_x;
                    p->pos_y = obj->pos_y;
                    p->ground_y = obj->ground_y;
                    f = obj->field_1c;
                    p->spawn_x = 0x60;
                    p->spawn_y = 0x1e0;
                    p->field_90 = (void *)0x800fb100;
                    p->field_98 = (void *)data_80172a48;
                    p->field_9c = (void *)data_80173c9c;
                    p->field_08 = 0x20;
                    p->field_1c = f;
                    p->field_66 = obj->field_66;
                    p->field_3c = obj;
                }
            }
        }
        obj->sequence = obj->sequence + 1;
        obj->step_timer = obj->sequence->duration;
        obj->step_flags = obj->sequence->flags;
        obj->frame = obj->frames + obj->sequence->frame_index;
    }
}
