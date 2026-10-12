/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_800e9724_slot0f[])(void);
extern int data_800f8578_slot0f;
extern Menu data_800f8574_slot0f;
void apply_menu_options_to_game_state_slot0f(void);
void add_option_menu_items_to_table_slot0f(void);
void update_eleven_entry_menu_cursor_slot0f(void);
void call_handler_by_substate_index_slot0f(void);

void update_options_menu_or_apply_on_button_slot0f(void) {
    HudState *h;
    if (data_800f8578_slot0f != 0) {
        call_handler_by_substate_index_slot0f();
    } else {
        data_800e9724_slot0f[data_8018f5a0->substep_index]();
        update_eleven_entry_menu_cursor_slot0f();
        add_option_menu_items_to_table_slot0f();
        if ((data_801a696a | data_801a6976) & 0x800) {
            apply_menu_options_to_game_state_slot0f();
            h = data_8018f5a0;
            data_800f8574_slot0f.mode_index = 3;
            h->phase_index = 1;
            h->step_index = 0;
            game_state.countdown_2 = 0;
            h->substep_index = 0;
            game_state.timer_or_bits = 0;
            h->step_index_2 = 0;
            game_state.countdown_timer_2 = 0;
        }
    }
}
