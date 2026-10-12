/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

extern u16 data_801a6966;

void noop_empty_function_7(void) {
}

int get_input_mapped_value_or_default(Object *object, int arg) {
    u8 a = arg;
    u8 i;
    if (object->mode_flag_2 == 0) {
        for (i = 0; i < 8; i++) {
            if (object->input_bits_2 & 0x100) {
                if ((object->input_bits_2 & data_8016ea60[i]) == data_8016ea60[i]) {
                    return data_8016e9b8[(u8)(object->kind * 8 + i)];
                }
            }
        }
    }
    return a;
}

void noop_empty_function_8(void) {
}
