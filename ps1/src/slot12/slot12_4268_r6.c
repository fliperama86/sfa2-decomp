/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_8002b710_slot12[16];
extern u16 data_8002bc30_slot12;
extern void (*table_800281f0_slot12[])(Object *);
int func_800144f8_slot12(u8 a, int b, u16 *dst, u16 *src);
void func_80013cf8_slot12(void);
void func_80013c74_slot12(void);

void func_800148b4_slot12(Object *obj) {
    table_800281f0_slot12[obj->field_04](obj);
}
