/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Img data_800ebc94_slot0f[];

void reset_phase_step_and_object_fields_slot0f(Slot0fObj *obj, int flag) {
    if (flag == 0) {
        data_8018f5a0->phase_index++;
    }
    data_8018f5a0->phase_index = 0;
    data_8018f5a0->step_index = 0;
    data_8018f5a0->substep_index = 0;
    obj->field_ee = 0;
    obj->field_f0 = 0;
}

void advance_hud_state_index_and_reset_slot0f(Slot0fObj *obj) {
    HudState *h = data_8018f5a0;
    h->state_index++;
    h->phase_index = 0;
    h->step_index = 0;
    h->substep_index = 0;
    obj->field_ee = 0;
    obj->field_f0 = 0;
    clear_players_arrays_state_and_init_pools();
    clear_arrays_and_init_pools_and_sprite_tables();
}

void queue_image_job_from_slot0f_table_entry_slot0f(int idx) {
    Rect rect;
    Slot12Img *img = &data_800ebc94_slot0f[idx];
    Job *job = (Job *)alloc_pooled_b();
    if (job != 0) {
        rect.x = img->rect.x;
        rect.y = img->rect.y;
        rect.w = img->rect.w;
        rect.h = img->rect.h;
        job->kind = 1;
        job->mode = 2;
        job->rect = rect;
        job->src = img->src;
    }
}
