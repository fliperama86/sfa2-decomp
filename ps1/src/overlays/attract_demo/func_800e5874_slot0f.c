/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u16 data_800f77ac_slot0f;
extern s16 data_800f728c_slot0f[16];
int fade_palette_step_slot0f(u8 a, int b, u16 *dst, u16 *src);
void queue_image_job_from_pooled_table_slot0f(void);
void func_800e52e0_slot0f(void);
void reset_phase_step_and_object_fields_slot0f(Slot0fObj *obj, int flag);

void func_800e5874_slot0f(Object *obj) {
    if (obj->field_01 == 0) {
        if ((pad_1_pressed | pad_2_pressed) & 0x800) {
            reset_phase_step_and_object_fields_slot0f((Slot0fObj *)obj, 0);
        }
    }
}

u8 fade_palette_rows_in_with_jobs_slot0f(Object *obj) {
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
        fade_palette_step_slot0f(1, 1, dst, src);
        src += 16;
        dst += 16;
    }
    queue_image_job_from_pooled_table_slot0f();
    p = (u16 *)data_800f728c_slot0f;
    data_800f77ac_slot0f -= 0x14;
    r = fade_palette_step_slot0f(1, 1, p, p - 16) & 0xff;
    func_800e52e0_slot0f();
    return r;
}
