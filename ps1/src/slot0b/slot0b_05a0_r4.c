/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801e46b0_slot0b[];
extern u8 data_801e4728_slot0b[];
ModObj *func_8011f32c(void);
ModPool *func_8011f4a4(void);
void func_801e0970_slot0b(void);

void func_801e08d8_slot0b(ModObj *obj) {
    Object *p = obj->field_3c;
    ModObj *c = func_8011f32c();
    u8 side;
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 0x1c;
        c->field_03 = 0;
        side = p->side;
        c->field_66 = 0;
        c->field_58 = (s32)data_801e46b0_slot0b;
        c->field_4c = 0;
        c->field_50 = 0;
        c->field_0b = 0;
        c->field_3c = (Object *)obj;
        c->field_48 = side;
        obj->field_30 = c;
    }
    if (p->side != 1) {
        func_801e0970_slot0b();
    }
}

void func_801e0970_slot0b(void) {
    ModPool *p = func_8011f4a4();
    Rect r;
    if (p != 0) {
        r.x = 0x140;
        r.y = 0x1e6;
        r.w = 0x10;
        r.h = 1;
        p->field_00 = 1;
        p->field_02 = 2;
        p->rect = r;
        p->field_04 = (u32)data_801e4728_slot0b;
    }
}
