/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_800518c8_slot28;
extern Object *data_800518bc_slot28[];
extern ObjectRef data_80051890_slot28;

void blink_object_by_countdown_timer_slot28(void);
void set_sequence_of_fourth_hud_object_2_slot28(Object *obj, int arg);

void start_countdown_120_when_field_negative_slot28(Object *obj) {
    if (((Slot28Obj *)data_800518c8_slot28.p)->field_3a < 0) {
        data_8018f5a0->countdown_timer = 0x78;
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
    }
}

void update_hud_countdown_and_pass_index_slot28(Object *obj) {
    HudState *h;
    int t;
    int n;
    blink_object_by_countdown_timer_slot28();
    h = data_8018f5a0;
    t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t == 0) {
        n = h->pass_index;
        h->countdown_timer = 8;
        h->pass_index = n + 1;
        data_800518bc_slot28[0]->field_01 = 0;
    }
}

void blink_object_by_countdown_timer_slot28(void) {
    HudState *h = data_8018f5a0;
    s16 x = h->countdown_timer;
    Object *o = data_800518bc_slot28[0];
    int m = (x < 100) ? 3 : 0;
    if (x < 60) {
        m = 1;
    }
    o->field_01 = 1;
    if (m != 0) {
        if ((x & m) == 0) {
            o->field_01 = 0;
        }
    }
}

void advance_pass_index_on_countdown_timeout_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    int t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t == 0) {
        h->countdown_timer = 0xf0;
        h->pass_index = h->pass_index + 1;
        data_80051890_slot28.p->variant_index = 0xff;
        set_sequence_of_fourth_hud_object_2_slot28(obj, 1);
    }
}
