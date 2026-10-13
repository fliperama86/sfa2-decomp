/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

int try_spawn_pooled_object_once_with_arg_slot01(u8 a);

void store_five_images_then_next_substep_slot01(void) {
    Rect r;
    r.x = 0;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, (u8 *)data_801a2be4);
    func_80157d9c(0);
    r.x = 0x10;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, (u8 *)data_801a2be4 + 0x400);
    func_80157d9c(0);
    r.x = 0x20;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, (u8 *)data_801a2be4 + 0x800);
    func_80157d9c(0);
    r.x = 0x60;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, (u8 *)data_801a2be4 - 0x400);
    func_80157d9c(0);
    r.x = 0x70;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, (u8 *)data_801a2be4 + 0xc00);
    func_80157d9c(0);
    if (try_spawn_pooled_object_once_with_arg_slot01(3) != 0) {
        screen_state->substep_index++;
        game_state.busy_flag = 0xff;
    }
}
