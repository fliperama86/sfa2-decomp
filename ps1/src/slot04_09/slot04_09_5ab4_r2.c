/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Block172 *func_8011f1e0(void);

void func_801b5ba0_slot04_09(Object *o) {
    Object *p;
    u16 y;
    if ((game_state.field_1d & 3) == 0) {
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0xb;
            p->field_08 = 0x20;
            p->field_03 = 0;
            p->field_0e = o->field_0e;
            p->field_3c = o;
            p->field_0b = o->field_0b;
            p->pos_x = (u16)o->pos_x;
            y = (u16)o->pos_y;
            p->field_44 = 1;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_98 = &data_80172a48;
            p->field_90 = (void *)0x800fb100;
            p->field_0d = 0;
            p->field_9c = &data_80173c9c;
            p->pos_y = y;
            p->field_66 = o->side;
        }
    }
}

void func_801b5c6c_slot04_09(Object *o) {
    Object *p;
    u16 y;
    if ((game_state.field_1d & 3) == 0) {
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0xb;
            p->field_08 = 0x20;
            p->field_03 = 4;
            p->field_0e = o->field_0e;
            p->field_3c = o;
            p->field_0b = o->field_0b;
            p->pos_x = (u16)o->pos_x;
            y = (u16)o->pos_y;
            p->field_44 = 1;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_98 = &data_80172a48;
            p->field_90 = (void *)0x800fb100;
            p->field_0d = 0;
            p->field_9c = &data_80173c9c;
            p->pos_y = y;
            p->field_66 = o->side;
        }
    }
}
