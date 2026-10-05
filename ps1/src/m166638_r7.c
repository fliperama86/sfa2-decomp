/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern int data_801831e8;

/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: pointer and value swap v0/v1 in the three flag edits (layout and size match). */
void func_8016cd84(int a) {
    int t;
    func_8015efc0(a);
    func_8016b788(a);
    func_8016ce4c();
    data_801831a4.field_00 = 0;
    data_801831a4.field_04 = 0;
    data_801831a4.field_06 = 0;
    data_801831a4.field_08 = 0;
    data_801831a4.field_0c = 0;
    t = table_8018360c[data_801831a4.field_00];
    data_80183198 = 0;
    data_8018319c = 0;
    data_801831a4.field_10 = t;
    func_8016c940(0xd1, t, 0);
    data_80183194 = 0;
    data_80183150 = 0;
    data_801831e8 = 0;
    data_801831f4 = 0;
    data_801831f8 = 0;
}
