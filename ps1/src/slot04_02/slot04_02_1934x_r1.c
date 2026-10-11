/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c62b0_slot04_02[];

/* What it does (inferred): a state entry of a character. It sets field_17b and field_07 to 1, clears field_12c to field_12f, spends 9 points, calls func_80138ae8 with the game state and the object, copies four consecutive words of a table, selected by field_12a, into field_4c, field_50, field_54 and field_58, and starts sequence 0x1b (0x61 when field_49 is not 0) plus field_12a / 2. */
void func_801b1934_slot04_02(Object *obj) {
    u16 n;
    int idx;

    obj->field_17b = 1;
    obj->field_07 = 1;
    obj->field_12f = 0;
    obj->field_12e = 0;
    obj->field_12d = 0;
    obj->field_12c = 0;
    func_80141f28(obj, 9);
    func_80138ae8(&game_state, obj);
    /* Stand-in, not recovered source: two nested do-while wrappers around the statement that sets the table index reproduce the original's choice of registers. Without it this function differs from the original in 51 instruction slots. */
    do {
        do {
            idx = obj->field_12a * 2;
        } while (0);
    } while (0);
    obj->field_4c = data_801c62b0_slot04_02[(u8)idx];
    obj->field_50 = data_801c62b0_slot04_02[(u8)(idx | 1)];
    obj->field_54 = data_801c62b0_slot04_02[(u8)(idx + 2)];
    obj->field_58 = data_801c62b0_slot04_02[(u8)(idx + 3)];
    n = 0x1b;
    if (obj->field_49 != 0) {
        n = 0x61;
    }
    n = (obj->field_12a >> 1) + n;
    func_801307e0(obj, n);
}
