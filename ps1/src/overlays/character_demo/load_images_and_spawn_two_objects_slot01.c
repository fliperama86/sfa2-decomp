/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void load_images_and_spawn_two_objects_slot01(void) {
    Rect r;
    Object *o;
    if (free_block_count >= 5) {
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
        o = (Object *)pop_block_from_pool();
        if (o != 0) {
            o->active_count = 1;
            o->field_02 = 0x1a;
            o->table_index = 0;
        }
        o = (Object *)pop_block_from_pool();
        if (o != 0) {
            o->active_count = 1;
            o->field_02 = 0xe;
            o->parent = game_state.focus_object;
        }
    }
    screen_state->substep_index++;
}
