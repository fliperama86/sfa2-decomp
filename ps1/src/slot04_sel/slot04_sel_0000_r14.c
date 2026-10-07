/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_801b8c98_slot04_sel[])(Object *);
extern void (*data_801b8ca8_slot04_sel[])(Object *, Slot04SelRec9d78 *);

void func_801b4a88_slot04_sel(Object *obj) {
    data_801b8c98_slot04_sel[data_8018f5a0->field_4e](obj);
}

/* The call of data_801b8ca8_slot04_sel passes no argument although its entries take two: the original does not set the argument registers before it. Written with two zero arguments, this function differs from the original in 2 instruction slots. */
void func_801b4ad0_slot04_sel(Object *unused) {
    ((void (*)(void))data_801b8ca8_slot04_sel[data_8018f5a0->field_50])();
}
