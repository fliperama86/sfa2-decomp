/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;

void func_801218fc(Object *object) {
    int i;
    Object *o;
    if (object->field_06 == 0) {
        data_8018f5a0->field_50++;
        data_8018f5a0->field_60 = 0x20;
        object->field_09 = 0;
        object->field_2c = 0;
        object->field_ab = 0;
        for (i = 0; i < 2; i++) {
            o = (Object *)func_8011f1e0();
            if (o != 0) {
                o->field_00 = 1;
                o->field_02 = 0x31;
                o->field_03 = i;
            }
        }
        for (i = 0; i < 2; i++) {
            o = (Object *)func_8011f1e0();
            if (o != 0) {
                o->field_00 = 1;
                o->field_02 = 0x32;
                o->field_03 = 0;
                o->field_48 = i;
                if (i == 0) {
                    o->field_3c = &player_left;
                } else {
                    o->field_3c = &player_right;
                }
            }
        }
        for (i = 0; i < 2; i++) {
            o = (Object *)func_8011f1e0();
            if (o != 0) {
                o->field_00 = 1;
                o->field_02 = 0x17;
                o->field_03 = 1;
                o->field_48 = i;
                if (i == 0) {
                    o->field_3c = &player_left;
                } else {
                    o->field_3c = &player_right;
                }
            }
        }
    }
}
