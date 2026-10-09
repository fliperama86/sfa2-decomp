/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_8002ac7c_slot28[])(Object *);

void func_80013e58_slot28(Object *obj) {
    data_8002ac7c_slot28[data_8018f5a0->field_50](obj);
}
