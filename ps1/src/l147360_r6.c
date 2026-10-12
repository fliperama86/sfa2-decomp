/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014a1ec(Object *object) {
    if (game_state.field_4e == 0) {
        if (object->countdown_3 >= 0x100) object->countdown_3 = 0xff;
        if (object->script_mode == 0) {
            if (object->init_done_flag == 0) {
                object->init_done_flag = object->init_done_flag + 1;
                init_script_from_table_index(object);
            } else if (object->init_done_flag != 1) return;
            start_or_run_script_mode_0(object);
        } else if (object->script_mode == 1) {
            if (object->init_done_flag == 0) {
                object->init_done_flag = object->init_done_flag + 1;
                start_random_script_from_table_index(object);
            } else if (object->init_done_flag != 1) return;
            start_or_run_script_mode_1(object);
        } else if (object->script_mode == 2) {
            if (object->init_done_flag == 0) {
                object->init_done_flag = object->init_done_flag + 1;
                start_random_script_from_other_frame(object);
            } else if (object->init_done_flag != 1) return;
            start_or_run_script_mode_2(object);
        } else if (object->script_mode == 3) {
            if (object->init_done_flag == 0) {
                object->init_done_flag = object->init_done_flag + 1;
                init_script_entry_by_script_index(object);
            } else if (object->init_done_flag != 1) return;
            start_or_run_script_mode_3(object);
        }
    }
}
