/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_801e52c8_slot2a;
extern int data_801e52c4_slot2a;
extern void (*data_801e2250_slot2a[])(Object *);

void reset_pass_state_clear_banks_and_queue_jobs_slot2a(void);

void advance_pass_or_call_end_by_high_bit_slot2a(void) {
    u16 *p = data_801e52c8_slot2a;
    if (!(p[0] & 0x8000)) {
        data_8018f5a0->pass_index++;
        game_state.status_flags = 0;
        game_state.field_cc = p[1] & 0x7fff;
    } else {
        reset_pass_state_clear_banks_and_queue_jobs_slot2a();
    }
}

void count_down_field_cc_advance_pass_slot2a(void) {
    u16 *c = &game_state.field_cc;
    int t = *c - 1; *c = t;
    if ((s16)*c < 0xdc) {
        data_8018f5a0->pass_index++;
    }
}

void wait_countdown_or_button_then_set_pass_3_slot2a(void) {
    if (is_any_enabled_pad_button_pressed() == 0) {
        u16 *c = &game_state.field_cc;
        int t = *c - 1; *c = t;
        if ((s16)*c >= 0) {
            return;
        }
    }
    data_8018f5a0->pass_index = 3;
    game_state.status_flags = 0xff;
    data_801e52c8_slot2a += 3;
}

void reset_pass_state_clear_banks_and_queue_jobs_slot2a(void) {
    set_menu_command_by_code(6, 2);
    data_8018f5a0->pass_index = 0;
    data_801e52c4_slot2a = 0;
    game_state.state_2c_flag = 0;
    game_state.field_84++;
    copy_palette_rows_from_backup_bank(0, 0x20, 0);
    copy_palette_rows_from_backup_bank(1, 0x20, 0);
    copy_palette_rows_from_backup_bank(2, 0x20, 0);
    copy_palette_rows_from_backup_bank(3, 0x20, 0);
    queue_five_jobs();
}

void call_handler_by_state_0_slot2a(Object *obj) {
    data_801e2250_slot2a[obj->state_0](obj);
}
