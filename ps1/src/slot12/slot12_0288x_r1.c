/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern int data_8002d578_slot12;
void func_8001129c_slot12(void);
void func_800113e4_slot12(void);

void func_80010288_slot12(void) {
    Rect r;
    Object *o;
    func_80157d00(0);
    func_8011a744();
    func_8011eae4();
    func_801260ac(1, 0x20, 0);
    func_801260ac(2, 0x20, 0);
    func_801260ac(3, 0x20, 0);
    func_80137220(1, 0);
    func_80137220(2, 1);
    func_80137220(3, 2);
    data_8018f5a0->substep_index++;
    game_state.status_flags = 0;
    game_state.field_4f = 0;
    game_state.field_65 = 0;
    game_state.cam_x = 0;
    game_state.cam_y = 0;
    game_state.field_d2 = 0x40;
    game_state.field_d4 = 0x100;
    game_state.cam_param_a = 0;
    game_state.cam_param_b = 0;
    game_state.field_da = 0x100;
    r.x = 0x180;
    r.y = 0x100;
    r.w = 0x40;
    r.h = 0x100;
    func_80157fc4(&r, (u8 *)0x80043344);
    func_80157d9c(0);
    func_800113e4_slot12();
    game_state.loop_mode = 0;
    func_8014f4d4(1, 0x406);
    while (data_80190948.active_flag == 0 || game_state.wait_flag != 0) {
        func_80138164();
        func_8011abe4();
        func_8001129c_slot12();
        func_801192bc(1);
    }
    data_8002d578_slot12 = 0;
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x7b;
        o->table_index = 1;
        o->spawn_x = 0;
        o->spawn_y = 0x1e0;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x7b;
        o->table_index = 2;
        o->spawn_x = 0;
        o->spawn_y = 0x1e0;
    }
}
