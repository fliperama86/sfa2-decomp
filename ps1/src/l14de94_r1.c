/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int roll_random_bit_check_from_kind_table(Object *object) {
    u32 *table;
    Object *other;
    if (object->script_counter != 0) return 0;
    if (object->field_221 != 0) return 0;
    other = object->other;
    if (other->link_flags == 0 && other->active_flag_3 == 0) return 0;
    if (other->frame->field_0c == 0xff) return 0;
    if (*(u16 *)&object->state_0 != 1) return 0;
    if (object->state_2 == 5 || object->state_2 == 7 || object->state_2 == 8 || object->state_2 == 9) return 0;
    if (player_left.kind == 0xd && player_right.kind == player_left.kind) {
        table = data_8017d524;
    } else if (player_left.kind == 0x10 && player_right.kind == player_left.kind) {
        table = data_8017d528;
    } else if (player_left.kind == 0x11 && player_right.kind == player_left.kind) {
        table = data_8017d52c;
    } else if (player_left.kind == 0x13 && player_right.kind == player_left.kind) {
        table = data_8017d530;
    } else if (object->side == 0) {
        table = data_8017d4d0[object->kind];
    } else {
        table = data_8017d534[object->kind];
    }
    return test_random_bit_in_table(object, table) != 0;
}
