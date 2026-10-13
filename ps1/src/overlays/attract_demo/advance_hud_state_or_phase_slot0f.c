/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern s16 data_800f728c_slot0f[16];
extern u16 data_800f77ac_slot0f;
int fade_palette_step_slot0f(u8 a, int b, u16 *dst, u16 *src);
void queue_image_job_from_pooled_table_slot0f(void);
void func_800e52e0_slot0f(void);
void advance_hud_state_index_and_reset_slot0f(Slot0fObj *obj);
void reset_phase_step_and_object_fields_slot0f(Slot0fObj *obj, int flag);

/* The call is written once with 1 and once with 0 in the two branches of the test: written as one call with the test as its argument, this function differs from the original in 8 instruction slots. */
void advance_hud_state_or_phase_slot0f(Object *obj) {
    if (((Slot0fObj *)obj)->field_01 != 0) {
        advance_hud_state_index_and_reset_slot0f((Slot0fObj *)obj);
    } else {
        screen_state->phase_index++;
        if (game_state.mode_flag_2 == 0) {
            reset_phase_step_and_object_fields_slot0f((Slot0fObj *)obj, 1);
        } else {
            reset_phase_step_and_object_fields_slot0f((Slot0fObj *)obj, 0);
        }
    }
}
