/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* The original loads data_801a6989 with a signed load; the shared declaration
   is unsigned, so each read says so. */

void func_80129468(Object *object) {
    if (game_state.versus_flag != 0 && game_state.mode == object->side + 1) {
        if (*(s8 *)&data_801a6989 == 0 && object->field_165 == 0 && object->state_2 != 8 && object->field_7e == 0 && (s16)object->meter < 0x30) {
            object->meter = (s16)object->meter + 1;
        }
        if (*(s8 *)&data_801a6989 == 1 && object->field_165 == 0 && object->state_2 != 8 && object->field_7e == 0 && (s16)object->meter < 0x60) {
            object->meter = (s16)object->meter + 1;
        }
        if (*(s8 *)&data_801a6989 == 2 && object->field_165 == 0 && object->state_2 != 8 && object->field_7e == 0 && (s16)object->meter < 0x90) {
            object->meter = (s16)object->meter + 1;
        }
    }
    if (object->mode_flag_2 != 0) {
        if ((u8)find_matching_script_condition_bit(object)) {
            init_object_mode_3(object);
        }
    } else if (game_state.versus_flag == 0 || game_state.mode == object->side + 1) {
        func_8013060c(object);
        enter_state_1_8_if_check_passes(object);
    }
    count_down_damage_countdown_and_recover(object);
    func_801299d0(object);
    table_8017193c[object->state_1](object);
}
