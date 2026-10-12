/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void func_800e004c_slot0f(Object *obj);
void queue_menu_items_with_highlight_slot0f(int a);

void update_menu_mode_selection_slot0f(GameState *state, Menu *menu) {
    u16 buttons;
    func_800e004c_slot0f((Object *)menu);
    buttons = data_801a696a | data_801a6976;
    if ((buttons & 0x5000) != 0) {
        if ((buttons & 0x1000) != 0) {
            menu->mode_index = (s8)menu->mode_index - 1;
            if ((s8)menu->mode_index < 0) {
                menu->mode_index = 5;
            }
            if ((s8)menu->command_state != 3 && (s8)menu->mode_index == 1) {
                menu->mode_index = (s8)menu->mode_index - 1;
            }
        } else {
            menu->mode_index = menu->mode_index + 1;
            if ((s8)menu->mode_index >= 6) {
                menu->mode_index = 0;
            }
            if ((s8)menu->command_state != 3 && (s8)menu->mode_index == 1) {
                menu->mode_index = (s8)menu->mode_index + 1;
            }
        }
        play_sound_at(0, 0, 0x204);
    }
    if ((s8)menu->command_state != (s8)menu->fade_mode) {
        if (((s8)menu->command_state & 1) == 0) {
            if ((s8)menu->mode_index == 2) {
                menu->mode_index = 0;
            }
        }
        if ((s8)menu->command_state != 3 && (s8)menu->mode_index == 1) {
            menu->mode_index = 0;
        }
    }
    menu->active_flag = 0;
    if ((buttons & 0x820) != 0) {
        if ((data_801a696a & 0x820) != 0) {
            menu->active_flag = 1;
        } else {
            menu->active_flag = 2;
        }
        data_8018f5a0->phase_index++;
        play_sound_at(0, 0, 0x205);
    }
    queue_menu_items_with_highlight_slot0f((s8)menu->mode_index);
}
