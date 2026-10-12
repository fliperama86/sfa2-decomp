/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_800f728c_slot0f[16];
extern u16 data_800f77ac_slot0f;
int fade_palette_step_slot0f(u8 a, int b, u16 *dst, u16 *src);
void queue_image_job_from_pooled_table_slot0f(void);
void func_800e52e0_slot0f(void);
void advance_hud_state_index_and_reset_slot0f(Slot0fObj *obj);
void reset_phase_step_and_object_fields_slot0f(Slot0fObj *obj, int flag);

u8 fade_palette_rows_out_with_jobs_slot0f(void) {
    u16 *dst;
    u16 *src;
    int i;
    u16 *p;
    u16 *base;
    int r;
    i = 0;
    base = (u16 *)data_800f728c_slot0f + 21 * 16;
    src = base - 0x140;
    dst = base;
    for (; i < 0x14; i++) {
        fade_palette_step_slot0f(0, -1, dst, src);
        src += 16;
        dst += 16;
    }
    queue_image_job_from_pooled_table_slot0f();
    p = (u16 *)data_800f728c_slot0f;
    data_800f77ac_slot0f += 0x14;
    r = fade_palette_step_slot0f(1, -1, p, p - 16) & 0xff;
    func_800e52e0_slot0f();
    return r;
}
