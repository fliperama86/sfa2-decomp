/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 data_801a6966;


void func_80131c7c(Object *object) {
    if (input_log.index >= 0xffd) {
        object->input_bits = 0;
        object->input_bits_2 = 0;
        object->input_word = 0;
        data_80171b36 = 0;
    } else {
        u16 t = input_log.buf[input_log.index];
        s16 count = data_80171b36;
        if (count == 0) {
            data_80185ffc = input_log.buf[input_log.index + 1];
            data_80171b36 = count + 1;
        }
        object->input_bits = t & 0xfdff;
        object->input_bits_2 = t & 0xfdff;
        object->input_word = t & 0xfdff;
        if (t & 0x200) {
            int n = data_80185ffc - 1;
            data_80185ffc = n;
            if ((s16)n < 0) {
                data_80171b36 = 0;
                input_log.index += 2;
            }
        } else {
            data_80171b36 = 0;
            input_log.index++;
        }
    }
}

void func_80131d68(Object *object) {
    int i;
    for (i = 0; i < 4; i++) {
        int d;
        if (!func_80148e84(object)) break;
        d = table_80171b38[i];
        if (object->facing_side != 0) d = -d;
        game_state.other_object->pos_x = game_state.other_object->pos_x + d;
    }
}

void func_80131df8(Object *object) {
    int i;
    for (i = 0; i < 4; i++) {
        int d;
        if (!func_80148ea8(object)) break;
        d = table_80171b40[i];
        if (object->facing_side != 0) d = -d;
        game_state.other_object->pos_x = game_state.other_object->pos_x + d;
    }
}
