/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudBig *data_8018f5a0;
void func_80123d64(GameState *g);

void func_80123534(Entity *entity) {
    Object *s1;
    Object *s2;
    if (entity->field_2f == 0) {
        func_80123d64((GameState *)entity);
        data_8018f5a0->field_4c++;
        entity->field_4f = 1;
        player_left.field_a5 = 0;
        player_right.field_a5 = 0;
        s1 = entity->field_78;
        s2 = entity->field_7c;
        func_80123bd4(entity, s1, s2);
        func_80123b4c(entity, s1);
        entity->field_114 = s1->kind;
        func_801259a8(entity, s1, s2);
        if (entity->field_90 != 0) {
            func_801239cc(entity, s1);
        } else {
            if (s2->side == 0) {
                entity->field_1b &= 0xfe;
            } else if (s2->side == 1) {
                entity->field_1b &= 0xfd;
            }
            if (s1->field_cd != 0) {
                func_8012370c(s2, entity);
            } else {
                s1->field_a5 = 2;
                s2->field_cd = 1;
            }
        }
    } else {
        data_8018f5a0->field_4a = 1;
        data_8018f5a0->field_4c = 0;
        data_8018f5a0->field_4e = 0;
        data_8018f5a0->field_50 = 0;
        entity->field_1a = 1;
        player_left.field_a5 = 0;
        player_right.field_a5 = 0;
        if (player_left.kind == 0x13) {
            player_left.kind = 0x11;
        }
        if (player_right.kind == 0x13) {
            player_right.kind = 0x11;
        }
        entity->field_1b = 0;
        entity->field_07 = 3;
        entity->field_17 = 3;
        entity->field_64 = 0;
        data_80190568 = 0;
        func_8011ee64();
        entity->field_4f = 1;
    }
}
