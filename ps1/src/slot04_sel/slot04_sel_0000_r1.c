/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_801b6d50_slot04_sel[])(Object *);
extern void (*data_801b6d60_slot04_sel[])(Object *, Slot04SelRec *);

void func_801b0000_slot04_sel(Object *obj) {
    data_801b6d50_slot04_sel[data_8018f5a0->field_4e](obj);
}

/* The call of data_801b6d60_slot04_sel passes no argument although its entries take two: the original does not set the argument registers before it. Written with two zero arguments, this function differs from the original in 2 instruction slots. */
void func_801b0048_slot04_sel(Object *unused) {
    ((void (*)(void))data_801b6d60_slot04_sel[data_8018f5a0->field_50])();
}
