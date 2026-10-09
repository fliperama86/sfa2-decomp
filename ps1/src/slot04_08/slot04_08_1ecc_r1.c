/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c3c54_slot04_08[];

void func_801b1ecc_slot04_08(Object *obj) {
    u16 t;
    int i;
    int m = -0x100;
    t = obj->field_3a;
    if ((u8)t != 0) {
        if ((t & 0x80) == 0) {
            obj->field_3a = t & 0xff00;
            game_state.field_63 = 0x18;
            func_80146960(obj);
            func_80120554(obj, obj->side, 0x319);
        }
        obj->field_07++;
        i = obj->field_12a >> 1;
        if (obj->field_49 != 0) {
            i += 3;
        }
        func_80140cd8(obj, (s16)(data_801c3c54_slot04_08[i] | m), 0);
    }
    func_80130efc(obj);
}
