/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80123bd4(Entity *entity, Object *a, Object *b) {
    int t;
    a->field_e8 = a->field_b8;
    b->field_e8 = b->field_b8;
    if (entity->field_1b == 0) {
        func_80123d0c(entity, &player_left);
        func_80123d0c(entity, &player_left + 1);
    } else if (entity->field_1b == 3) {
        t = a->field_b8;
        a->field_b8 = a->field_e4;
        if (a->field_e0 < t) {
            a->field_e0 = t;
        }
        t = b->field_b8;
        b->field_11b = entity->field_40;
        b->field_b8 = b->field_e4;
        if (b->field_e0 < t) {
            b->field_e0 = t;
        }
    } else {
        if (a->field_cd != 0) {
            func_80123d0c(entity, b);
        } else {
            if (a->field_c0 < 0x63) {
                a->field_c0++;
            }
            t = func_80156018(a->field_e8, a->field_e4);
            a->field_e8 = t;
            a->field_e4 = a->field_b8;
        }
    }
    func_80123d0c(entity, b);
}
