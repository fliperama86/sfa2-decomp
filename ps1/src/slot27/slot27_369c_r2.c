/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern int data_80190464[];
void func_80013cc4_slot27(Object *obj);

void func_80013874_slot27(Object *obj) {
    if (obj->field_22 & 0x8000) {
        obj->field_06 = 0;
        obj->field_07 = 0;
        obj->field_05 = obj->field_05 + 1;
    }
    func_80013cc4_slot27(obj);
}

void func_800138bc_slot27(void) {
    int *sel = (int *)&game_state.field_354;

    *sel = game_state.field_2e;
    if (game_state.field_06 == 0) {
        int *pad = data_80190464;

        *pad = ~game_state.field_358->field_c4;
        *pad = game_state.field_358->field_c2 & *pad;
        if (*pad & 0x8000) {
            *sel = 0;
        }
        if (*pad & 0x2000) {
            *sel = 1;
        }
    }
}

void func_80013930_slot27(Object *obj) {
    int side;
    u16 *h;

    obj->field_04 = obj->field_04 + 1;
    side = game_state.field_358->side;
    *(int *)&game_state.field_354 = side;
    h = &data_8018f5a0->field_52;
    *h |= 1 << side;
}

void func_80013974_slot27(Object *obj) {
    obj->field_04 = obj->field_04 + 1;
}

void func_80013988_slot27(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
