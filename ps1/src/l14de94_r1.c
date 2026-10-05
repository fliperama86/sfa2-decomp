/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_8014de94(Object *object) {
    u32 *table;
    Object *other;
    if (object->field_253 != 0) return 0;
    if (object->field_221 != 0) return 0;
    other = object->other;
    if (other->field_240 == 0 && other->field_159 == 0) return 0;
    if (other->frame->field_0c == 0xff) return 0;
    if (*(u16 *)&object->field_04 != 1) return 0;
    if (object->field_06 == 5 || object->field_06 == 7 || object->field_06 == 8 || object->field_06 == 9) return 0;
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
    return func_8014a170(object, table) != 0;
}
