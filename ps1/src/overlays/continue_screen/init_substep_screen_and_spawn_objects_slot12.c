/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern int data_8002d578_slot12;
void play_sounds_at_countdown_timer_values_slot12(void);
void load_and_store_palette_rows_slot12(void);

void init_substep_screen_and_spawn_objects_slot12(void) {
    Rect r;
    Object *o;
    func_80157d00(0);
    init_sprite_tables();
    clear_arrays_and_init_pools_and_sprite_tables();
    copy_palette_rows_from_backup_bank(1, 0x20, 0);
    copy_palette_rows_from_backup_bank(2, 0x20, 0);
    copy_palette_rows_from_backup_bank(3, 0x20, 0);
    queue_image_upload_job(1, 0);
    queue_image_upload_job(2, 1);
    queue_image_upload_job(3, 2);
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
    load_and_store_palette_rows_slot12();
    game_state.loop_mode = 0;
    set_menu_command_by_code(1, 0x406);
    while (data_80190948.active_flag == 0 || game_state.wait_flag != 0) {
        update_blocks();
        update_active_slab172_entries();
        play_sounds_at_countdown_timer_values_slot12();
        request_sound_command_and_switch_thread(1);
    }
    data_8002d578_slot12 = 0;
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x7b;
        o->table_index = 1;
        o->spawn_x = 0;
        o->spawn_y = 0x1e0;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x7b;
        o->table_index = 2;
        o->spawn_x = 0;
        o->spawn_y = 0x1e0;
    }
}
