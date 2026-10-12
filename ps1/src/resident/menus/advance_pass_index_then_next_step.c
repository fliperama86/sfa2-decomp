/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void advance_pass_index_then_next_step(Object *object) {
    Entity *entity = (Entity *)object;
    int v;
    switch (data_8018f5a0->pass_index) {
    case 0:
        v = data_8018f5a0->sub_timer - 1;
        data_8018f5a0->sub_timer = v;
        if ((s16)v < 0) {
            data_8018f5a0->sub_timer = 4;
            data_8018f5a0->pass_index++;
            init_sprite_tables();
        }
        break;
    case 1:
        v = data_8018f5a0->sub_timer - 1;
        data_8018f5a0->sub_timer = v;
        if ((s16)v < 0) {
            data_8018f5a0->sub_timer = 0;
            data_8018f5a0->pass_index++;
        }
        break;
    case 2:
        data_8018f5a0->step_index++;
        data_8018f5a0->substep_index = 0;
        data_8018f5a0->step_index_2 = 0;
        data_8018f5a0->pass_index = 0;
        entity->field_ab = 0;
        if (entity->table_index != 0) {
            data_8018f5a0->step_index = 5;
        }
        entity->active_flag = 1;
        init_hud_primitives();
        if (entity->mode_flag_b != 0) {
            data_80197f1c = 0xff;
        }
        break;
    }
}
