/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_8002b710_slot12[16];
extern u16 data_8002bc30_slot12;
extern void (*table_800281f0_slot12[])(Object *);
int fade_palette_step_slot12(u8 a, int b, u16 *dst, u16 *src);
void queue_image_job_from_pooled_table_slot12(void);
void func_80013c74_slot12(void);

void call_handler_by_state_0_4_slot12(Object *obj) {
    table_800281f0_slot12[obj->state_0](obj);
}
