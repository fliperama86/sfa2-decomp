/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_801b7c34_slot04_sel[])(Object *);
extern void (*data_801b7c44_slot04_sel[])(Object *, Slot04SelRec9d38 *);

void call_handler_by_substep_index_2_slot04_sel(Object *obj) {
    data_801b7c34_slot04_sel[data_8018f5a0->substep_index](obj);
}

/* The call of data_801b7c44_slot04_sel passes no argument although its entries take two: the original does not set the argument registers before it. Written with two zero arguments, this function differs from the original in 2 instruction slots. */
void call_handler_by_step_index_2_2_slot04_sel(Object *unused) {
    ((void (*)(void))data_801b7c44_slot04_sel[data_8018f5a0->step_index_2])();
}
