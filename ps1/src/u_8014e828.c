/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 data_801a6966[];

void loop_check_button_bits_call_handler(void) {
    while (1) {
        if ((data_801a6966[0] & 0x100) && (data_801a6966[2] & 0x800)) switch_to_next_table_entry_and_wait();
        request_sound_command_and_switch_thread(1);
    }
}
