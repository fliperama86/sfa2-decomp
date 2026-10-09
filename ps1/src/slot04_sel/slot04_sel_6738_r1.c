/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b9c20_slot04_sel;
extern u8 data_801b9c28_slot04_sel[];
extern u8 *data_801b9cd8_slot04_sel[];
extern u8 data_801b9db0_slot04_sel;
extern u8 data_801b9db4_slot04_sel;
void func_801b68c4_slot04_sel(Object *obj, u8 *rec, int n);

void func_801b6738_slot04_sel(Object *obj, u8 *rec) {
    u8 **t = data_801b9cd8_slot04_sel;

    if (*rec == 0 && ((game_state.field_07 >> obj->side) & 1) != 0) {
        data_801b9db0_slot04_sel = data_801b9c20_slot04_sel;
        data_801b9db4_slot04_sel = *rec;
        func_801b68c4_slot04_sel(obj, rec, 5);
        data_801b9c20_slot04_sel = data_801b9db0_slot04_sel;
        *rec = data_801b9db4_slot04_sel;
        if (*rec != 0) {
            func_80120554(0, obj->side, 1);
        }
    }
    t[3][data_801b9c28_slot04_sel[0]] = 0x1a;
    t[3][data_801b9c28_slot04_sel[1]] = 0x1a;
    t[3][data_801b9c28_slot04_sel[2]] = 0x1a;
    t[3][data_801b9c28_slot04_sel[3]] = 0x1a;
    t[3][data_801b9c28_slot04_sel[4]] = 0x1a;
    t[3][data_801b9c28_slot04_sel[5]] = 0x1a;
    t[3][data_801b9c28_slot04_sel[data_801b9c20_slot04_sel]] = 0x10;
}
