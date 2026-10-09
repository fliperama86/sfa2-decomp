/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c0564_slot04_00[];

void func_801b31a4_slot04_00(Object *obj) {
    if (obj->field_ac != 4 || obj->field_af == 0) {
        func_80137b64(obj);
    } else {
        obj->field_af--;
        if (obj->field_af != 0) {
            func_80137b64(obj);
        } else {
            obj->field_0d--;
            func_80138070(obj, 7);
        }
    }
}

void func_801b3220_slot04_00(Object *obj) {
    if ((game_state.config->field_a8 | game_state.config->field_65) != 0) {
        func_8011ffdc(obj);
    } else {
        data_801c0564_slot04_00[obj->field_06](obj);
        func_8011ffdc(obj);
    }
}
