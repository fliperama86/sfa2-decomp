/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void apply_damage_to_other_and_play_sound(Object *object, s16 a, s16 b) {
    s8 hi;
    int d;
    u16 w;
    data_80189430 = a;
    if ((s16)ref_other.p->health >= 0) {
        hi = a >> 8;
        if (hi == -2) return;
        d = compute_damage_to_other(object, a, b);
        if (*(u32 *)&game_state.config->defeat_side_bits & 0xffff00) goto tail;
        ref_other.p->health = ref_other.p->health - d;
        if ((s16)ref_other.p->health >= 0) goto tail;
        if (hi < 0) {
            ref_other.p->health = 0;
            goto tail;
        }
    }
    *(s16 *)&ref_other.p->health = -1;
    object->field_167 = 3;
    game_state.config->defeated_kind = object->kind;
    ref_other.p->field_15b = 1;
    game_state.config->defeat_side_bits |= 1 << object->side;
tail:
    if (object->mode_flag_2 == 0) {
        w = data_80189430;
        if ((s8)(w >> 8) != -2) {
            object->sound_index = w & 0x1f;
            object->field_bf = 0xff;
            add_table_bcd_score_to_player(table_80181510[object->sound_index], (s8)object->side);
        }
    }
}
