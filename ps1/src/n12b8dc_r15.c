/* Reconstruction. Names/roles inferred, not original symbols. */
/* Residual: not exact. Same instructions except that the original returns 0
   for 0x6a with a branch straight to the final return. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 table_8017ab88[];

u8 func_8013d210(Object *object) {
    return *((u8 *)&object->field_134 + 1) & 1;
}
