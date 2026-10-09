/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801443b0(Object *object) {
    Config *config;
    if ((s16)object->field_3a < 0) {
        config = game_state.config;
        if (config->field_a6 != 0) {
            func_8014448c(object);
        } else {
            if (*(Object **)&config->field_78 != 0) {
                ref_first.p = *(Object **)&config->field_78;
                if (ref_first.p->field_cd != 0) {
                    func_80120554(0, 0, 0x209);
                } else {
                    func_80120554(0, 0, 0x208);
                }
            }
            if ((u8)func_801441c8(object)) {
                func_80144090(object);
            } else {
                func_80144e90(object);
            }
        }
    }
}
