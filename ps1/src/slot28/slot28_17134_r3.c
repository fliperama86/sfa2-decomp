/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot28Rec50ca4 data_80050ca4_slot28[];
extern int data_80051ce4_slot28[];
extern void (*data_8005174c_slot28[])(int *);
void spawn_objects_by_state_index_slot28(Object *o);
void copy_table_to_rows_zero_and_five_slot28(void);
void func_80027adc_slot28(Object *o);
void init_sixteen_records_to_eight_slot28(int *p);
void scroll_by_step_and_end_on_variant_ff_slot28(int *p);

void set_random_x_top_position_slot28(Object *obj) {
    obj->pos_x = (u8)get_random_byte() + 0x40;
    obj->pos_y = 0x10;
}

void init_random_vertical_motion_and_facing_slot28(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    int unused[2];
    int i = get_random_byte() & 7;
    obj->table_index = 0;
    obj->vel_y = data_80050ca4_slot28[i].vel_y;
    obj->accel_y = data_80050ca4_slot28[i].accel_y;
    obj->facing_side = get_random_byte() & 1;
}

void call_handler_by_step_index_2_slot28(Object *obj) {
    data_8005174c_slot28[data_8018f5a0->step_index_2](data_80051ce4_slot28);
}

void func_800276b8_slot28(int *p) {
    GameState *g = &game_state;
    data_8018f5a0->step_index_2++;
    p[1] = 0;
    p[0] = 0x8000;
    spawn_objects_by_state_index_slot28((Object *)g);
    copy_table_to_rows_zero_and_five_slot28();
    func_80027adc_slot28((Object *)g);
    init_sixteen_records_to_eight_slot28(p);
    scroll_by_step_and_end_on_variant_ff_slot28(p);
    data_80190568 = 1;
    start_fade_mode_c01_and_wait();
}
