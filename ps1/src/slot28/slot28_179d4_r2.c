/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8005146c_slot28[];
extern u8 data_80051528_slot28[];
extern BoxTables data_800516dc_slot28;
extern ObjectRef data_80051df0_slot28;
Block172 *func_8011f1e0(void);

void func_80027b18_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    Object *p;
    int i = 0;
    switch (obj->field_70) {
    case 3:
        break;
    case 2:
    case 0x14:
        for (i = 0; i < 2; i++) {
            p = (Object *)func_8011f1e0();
            if (p != 0) {
                p->field_00 = 1;
                p->field_02 = 0xaf;
                p->field_98 = data_8005146c_slot28;
                p->field_9c = data_80051528_slot28;
                p->field_7a = 0x60;
                p->field_7c = 0x1e0;
                p->field_09 = 4;
                p->field_03 = i;
                p->field_90 = (void *)0x80078000;
                p->field_0d = 0;
                p->box_tables = &data_800516dc_slot28;
            }
        }
        break;
    default:
        for (i = 0; i < 6; i++) {
            p = (Object *)func_8011f1e0();
            if (p != 0) {
                p->field_00 = 1;
                p->field_02 = 0xae;
                p->field_98 = data_8005146c_slot28;
                p->field_9c = data_80051528_slot28;
                p->field_7a = 0x60;
                p->field_7c = 0x1e0;
                p->field_09 = 4;
                p->field_03 = i;
                p->field_90 = (void *)0x80078000;
                p->field_0d = 0;
                p->box_tables = &data_800516dc_slot28;
            }
        }
        break;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        p->field_00 = 1;
        p->field_02 = 0xb0;
        p->field_98 = data_8005146c_slot28;
        p->field_9c = data_80051528_slot28;
        p->pos_x = 0xb8;
        p->pos_y = 0xf30;
        p->field_7a = 0x60;
        p->field_7c = 0x1e0;
        p->field_09 = 3;
        p->field_03 = 0;
        p->field_90 = (void *)0x80078000;
        p->field_0d = 0;
        p->box_tables = &data_800516dc_slot28;
        p->field_48 = 0;
    }
    data_80051df0_slot28.p = p;
}
