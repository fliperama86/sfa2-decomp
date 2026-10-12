/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
void func_801b2658_slot04_0e(Object *obj);
int func_801b20bc_slot04_0e(Object *obj);

int func_801b20bc_slot04_0e(Object *obj) {
    if (!func_801417cc(obj)) return 0;
    obj->field_15a = 3;
    obj->active_flag_3 = 0;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->variant_index = 0xff;
    if (!(obj->input_bits_2 & 0x2000)) {
        obj->variant_index = 1;
    }
    func_801b2658_slot04_0e(obj);
    return 1;
}
