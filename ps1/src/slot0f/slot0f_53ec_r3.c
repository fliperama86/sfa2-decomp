/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800ebc68_slot0f[])(Object *);
void queue_image_job_from_slot0f_table_entry_slot0f(int flag);
void set_phase_two_clear_step_slot0f(Object *obj);
void func_800e5874_slot0f(Object *obj);

void call_image_job_with_index_0_slot0f(Object *obj) {
    queue_image_job_from_slot0f_table_entry_slot0f(0);
}

void call_step_handler_and_update_blocks_slot0f(Object *obj) {
    u16 *timer = &data_801903ca;
    HudState *hud;
    *timer = *timer - 1;
    hud = data_8018f5a0;
    data_800ebc68_slot0f[hud->step_index](obj);
    update_blocks();
    update_active_slab172_entries();
    func_800e5874_slot0f(obj);
}

void advance_step_when_state_is_1_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    if (data_801903c4[0] == 1) {
        data_8018f5a0->step_index++;
        if (obj->field_ee != 0) {
            set_phase_two_clear_step_slot0f(o);
        } else {
            start_fade_mode_c01_and_wait();
        }
    }
}
