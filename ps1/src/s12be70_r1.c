/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



int is_object_pressed_bits_high_nibble_set(Object *object) {
    if (object->mode_flag_2 == 0) {
        if ((s16)object->pressed_bits & 0xf000) return 1;
    } else {
        func_8012bf4c(object);
    }
    return 0;
}

int is_prev_input_low_byte_zero(Object *object) {
    int result;
    if (object->mode_flag_2 == 0) {
        result = *(u8 *)&object->prev_input_bits_2 == 0;
    } else {
        func_8012bf4c(object);
        result = 0;
    }
    return result;
}

int func_8012bf00(Object *object) {
    int result;
    if (object->mode_flag_2 == 0) {
        result = ((object->pressed_bits >> 8) & 0xfc) != 0;
    } else {
        func_8012bf4c(object);
        result = 0;
    }
    return result;
}

void func_8012bf4c(Object *object) {
    test_random_bit_in_table(object, data_8017d818);
}
