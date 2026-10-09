/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u16 *func_801414dc(Object *object);
BytePair func_8013054c(Object *object);
void func_80138c78(GameState *state, Object *object);

int func_801412a4(Object *object) {
    BytePair dir;
    int a;
    if ((game_state.config->field_4d | game_state.config->field_04) != 0 || (func_8012f56c(object) & 0xff) != 0 ||
        object->field_67 == 0 || (object->field_134 & 0xfc) == 0) {
        return 0;
    }
    a = (int)func_801414dc(object);
    dir = func_8013054c(object);
    if (object->field_49 == 0) {
        if (object->field_296 == 0) return 0;
        if (func_801415c0(object, a, dir) == 0) return 0;
        if (func_80141534(object, dir) == 0) return 0;
    }
    object->field_12a = dir.first;
    object->field_129 = dir.second;
    object->field_128 = 0;
    if (object->field_130 & 0x4000) object->field_128 = 2;
    func_8014147c(object, dir);
    object->field_17d = dir.first;
    object->field_17c = dir.second;
    object->field_17e = object->field_128;
    func_80138c78((GameState *)game_state.config, object);
    func_80142c04(object);
    return 1;
}
