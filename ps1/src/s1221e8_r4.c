/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudBig *data_8018f5a0;

void func_80122d90(Entity *entity) {
    if (entity->field_6c == 0) {
        if (entity->field_30 == 0) {
            data_8018f5a0->field_4e++;
            entity->field_c8 = 0;
            entity->field_ca = 0;
            data_8018f5a0->field_52 = 0;
            entity->field_cc = 0;
            if (entity->field_a6 != 0 || entity->field_78->field_cd != 0) {
                data_8018f5a0->field_50 = 5;
            } else {
                data_8018f5a0->field_50 = 0;
                entity->field_ac = 0xff;
            }
        } else {
            int v = entity->field_ca - 1;
            entity->field_ca = v;
            if ((s16)v == 0) {
                func_80122e54(entity);
            }
        }
    }
}
