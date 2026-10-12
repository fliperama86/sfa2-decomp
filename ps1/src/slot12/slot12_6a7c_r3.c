/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudSlot data_8002c4c8_slot12[4];
void init_hud_slot_two_digit_text_slot12_slot12(Object *obj, HudSlot *c, s16 i);

void init_four_two_digit_hud_slots_slot12_slot12(Object *obj) {
    int i;

    for (i = 0; i < 4; i++) {
        init_hud_slot_two_digit_text_slot12_slot12(obj, &data_8002c4c8_slot12[i], i);
    }
}
