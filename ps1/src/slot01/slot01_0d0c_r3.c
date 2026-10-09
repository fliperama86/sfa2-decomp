/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;

void func_80011048_slot01(void) {
    Rect r;
    Object *o;
    if (data_80197f10 >= 5) {
        r.x = 0x70;
        r.y = 0x1e0;
        r.w = 0x10;
        r.h = 0x16;
        func_80157fc4(&r, (u8 *)0x80071148);
        func_80157d9c(0);
        r.x = 0x20;
        r.y = 0x1e0;
        r.w = 0x10;
        r.h = 1;
        func_80157fc4(&r, (u8 *)0x80071108);
        func_80157d9c(0);
        r.x = 0x20;
        r.y = 0x1e8;
        r.w = 0x10;
        r.h = 1;
        func_80157fc4(&r, (u8 *)0x80071128);
        func_80157d9c(0);
        r.x = 0x10;
        r.y = 0x1e0;
        r.w = 0x10;
        r.h = 8;
        func_80157fc4(&r, (u8 *)0x80070ea8);
        func_80157d9c(0);
        r.x = 0x280;
        r.y = 0x100;
        r.w = 0x40;
        r.h = 0x100;
        func_80157fc4(&r, (u8 *)0x80065478);
        func_80157d9c(0);
        r.x = 0x10;
        r.y = 0x1f0;
        r.w = 0x10;
        r.h = 0xb;
        func_80157fc4(&r, (u8 *)0x80070fa8);
        func_80157d9c(0);
        o = (Object *)func_8011f1e0();
        if (o != 0) {
            o->field_00 = 1;
            o->field_02 = 0x1a;
            o->field_03 = 0;
        }
        o = (Object *)func_8011f1e0();
        if (o != 0) {
            o->field_00 = 1;
            o->field_02 = 0xe;
            o->field_3c = game_state.field_78;
        }
    }
    data_8018f5a0->field_4e++;
}
