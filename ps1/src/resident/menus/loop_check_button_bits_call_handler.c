/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

extern u16 pad_1_held[];

void loop_check_button_bits_call_handler(void) {
    while (1) {
        if ((pad_1_held[0] & 0x100) && (pad_1_held[2] & 0x800)) switch_to_next_table_entry_and_wait();
        request_sound_command_and_switch_thread(1);
    }
}
