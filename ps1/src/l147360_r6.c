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
                func_8014d9ac(object);
            } else if (object->init_done_flag != 1) return;
            func_8014a364(object);
        } else if (object->script_mode == 1) {
            if (object->init_done_flag == 0) {
                object->init_done_flag = object->init_done_flag + 1;
                func_8014da18(object);
            } else if (object->init_done_flag != 1) return;
            func_8014a3c8(object);
        } else if (object->script_mode == 2) {
            if (object->init_done_flag == 0) {
                object->init_done_flag = object->init_done_flag + 1;
                func_8014dae8(object);
            } else if (object->init_done_flag != 1) return;
            func_8014a42c(object);
        } else if (object->script_mode == 3) {
            if (object->init_done_flag == 0) {
                object->init_done_flag = object->init_done_flag + 1;
                func_8014dbc8(object);
            } else if (object->init_done_flag != 1) return;
            func_8014a490(object);
        }
    }
}
