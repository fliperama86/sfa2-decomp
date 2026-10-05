/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudBig *data_8018f5a0;
u8 func_80125394(void);

void func_8012318c(Entity *entity) {
    if (entity->field_a9 == 0) {
        if (func_80125394() == 0) {
            int v = entity->field_ca - 1;
            entity->field_ca = v;
            if ((s16)v >= 0) {
                func_80156094();
                return;
            }
        }
        data_8018f5a0->field_50 = 5;
    } else {
        data_8018f5a0->field_50++;
    }
}
