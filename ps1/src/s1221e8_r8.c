/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012370c(Object *a, Entity *b) {
    a->field_cd = 1;
    ((HudBig *)data_8018f5a0)->field_4c = 3;
    b->field_27 = 1;
    b->field_4f = 1;
    b->field_1b = 0;
    player_left.field_a5 = 0;
    player_right.field_a5 = 0;
}

void func_80123748(Entity *entity) {
    func_80123bd4(entity, &player_left, &player_left + 1);
    ((HudBig *)data_8018f5a0)->field_4a++;
    ((HudBig *)data_8018f5a0)->field_4c = 0;
    entity->field_c6 = 0;
    ((HudBig *)data_8018f5a0)->field_4e = 0;
    entity->field_c8 = 0;
    ((HudBig *)data_8018f5a0)->field_50 = 0;
    entity->field_ca = 0;
    ((HudBig *)data_8018f5a0)->field_52 = 0;
    entity->field_cc = 0;
    entity->field_77 = 0;
    entity->field_80 = 0;
    entity->field_27 = 1;
    entity->field_4f = 1;
    entity->field_1b = 0;
    player_left.field_01 = 0;
    player_right.field_01 = 0;
    player_left.field_a5 = 0;
    player_right.field_a5 = 0;
    player_left.field_cd = 1;
    player_right.field_cd = 1;
    data_80190568 = 0;
}
