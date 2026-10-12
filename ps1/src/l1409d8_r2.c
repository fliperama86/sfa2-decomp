/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_801412a4(Object *object) {
    BytePair dir;
    int a;
    if ((game_state.config->mode_flag_b | game_state.config->mode_flag_a) != 0 || (func_8012f56c(object) & 0xff) != 0 ||
        object->active_flag == 0 || (object->pressed_bits & 0xfc) == 0) {
        return 0;
    }
    a = (int)func_801414dc(object);
    dir = func_8013054c(object);
    if (object->field_49 == 0) {
        if (object->kind_timer == 0) return 0;
        if (func_801415c0(object, a, dir) == 0) return 0;
        if (func_80141534(object, dir) == 0) return 0;
    }
    object->direction_index = dir.first;
    object->input_dir_second = dir.second;
    object->input_dir_0 = 0;
    if (object->input_bits_2 & 0x4000) object->input_dir_0 = 2;
    func_8014147c(object, dir);
    object->dir_first = dir.first;
    object->dir_second = dir.second;
    object->field_17e = object->input_dir_0;
    func_80138c78((GameState *)game_state.config, object);
    func_80142c04(object);
    return 1;
}
