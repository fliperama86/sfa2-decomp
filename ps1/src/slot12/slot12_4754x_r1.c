/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_8002b710_slot12[16];
extern u16 data_8002bc30_slot12;
int fade_palette_step_slot12(u8 a, int b, u16 *dst, u16 *src);
void queue_image_job_from_pooled_table_slot12(void);
void func_80013c74_slot12(void);

u8 fade_palette_rows_in_with_jobs_slot12(Object *obj) {
    u16 *dst;
    u16 *src;
    int i;
    u16 *p;
    u16 *base;
    int r;
    i = 0;
    base = (u16 *)data_8002b710_slot12 + 21 * 16;
    src = base - 0x140;
    dst = base;
    for (; i < 0x14; i++) {
        fade_palette_step_slot12(1, 1, dst, src);
        src += 16;
        dst += 16;
    }
    queue_image_job_from_pooled_table_slot12();
    p = (u16 *)data_8002b710_slot12;
    data_8002bc30_slot12 -= 0x14;
    r = fade_palette_step_slot12(1, 1, p, p - 16) & 0xff;
    func_80013c74_slot12();
    return r;
}
