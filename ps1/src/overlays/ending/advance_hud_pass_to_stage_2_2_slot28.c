/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051c50_slot28[];
extern ObjectRef data_80051c54_slot28;
void release_active_objects_in_table_14_slot28(void);
void set_sequence_of_fourth_hud_object_16_slot28(Object *obj, int arg);

/* The local n holds the byte read from the object and then the half counter of the hud. Written with one local for each, this function differs from the original in 6 instruction slots. The local c holds the constant that is stored in the hud; written with the literal at the store, the function differs in 7. */
void advance_hud_pass_to_stage_2_2_slot28(Object *obj) {
    int c = 0x258;
    u16 n;
    HudState *h;
    n = obj->field_f0;
    if (n == 0) {
        h = data_8018f5a0;
        h->countdown_timer = c;
        n = h->pass_index + 1;
        h->pass_index = n;
        release_active_objects_in_table_14_slot28();
        data_80051c50_slot28[0]->field_01 = 0;
        data_80051c54_slot28.p->field_01 = 0;
        set_sequence_of_fourth_hud_object_16_slot28(obj, 2);
        start_fade_mode_1a01_and_wait();
    }
}
