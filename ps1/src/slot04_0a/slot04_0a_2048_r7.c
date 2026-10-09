/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c0688_slot04_0a[];
extern u8 data_801c068b_slot04_0a[];

void func_801b25fc_slot04_0a(Object *obj) {
    int i;

    obj->field_07++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    obj->field_29c = 2;
    obj->field_103 = 0xff;
    i = obj->field_12a >> 1;
    ((Slot04aObj *)obj)->field_104 = data_801c0688_slot04_0a[i];
    obj->field_46 = (s8)data_801c068b_slot04_0a[i];
    obj->field_225 = 1;
    obj->field_165 = 0;
    func_80120554(obj, obj->side, 0x31c);
    func_80145d20(obj);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x39);
}
