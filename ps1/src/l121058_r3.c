/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80121dc4(Object *object) {
    Entity *entity = (Entity *)object;
    int v;
    switch (data_8018f5a0->field_52) {
    case 0:
        v = data_8018f5a0->field_62 - 1;
        data_8018f5a0->field_62 = v;
        if ((s16)v < 0) {
            data_8018f5a0->field_62 = 4;
            data_8018f5a0->field_52++;
            func_8011a744();
        }
        break;
    case 1:
        v = data_8018f5a0->field_62 - 1;
        data_8018f5a0->field_62 = v;
        if ((s16)v < 0) {
            data_8018f5a0->field_62 = 0;
            data_8018f5a0->field_52++;
        }
        break;
    case 2:
        data_8018f5a0->field_4c++;
        data_8018f5a0->field_4e = 0;
        data_8018f5a0->field_50 = 0;
        data_8018f5a0->field_52 = 0;
        entity->field_ab = 0;
        if (entity->field_80 != 0) {
            data_8018f5a0->field_4c = 5;
        }
        entity->field_09 = 1;
        func_8013245c();
        if (entity->field_30 != 0) {
            data_80197f1c = 0xff;
        }
        break;
    }
}
