/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80028474_slot12[])(Object *);
extern u8 data_8002bc78_slot12;
extern u8 data_8002bc7c_slot12[];
extern u8 data_8002bc8c_slot12[];
extern Slot12Quad data_8002bc94_slot12[];
extern Slot12Cell data_8002bdd4_slot12[];
extern ObjectRef data_80190468;
void func_80014300_slot12(Object *obj, void *cell);

void func_8001505c_slot12(Object *obj) {
    int i;
    int k;
    data_80190468.p = (Object *)&game_state;
    data_80028474_slot12[obj->field_04](obj);
    k = obj->field_03;
    if (k == 5) {
        i = 0;
        if (data_8002bc78_slot12 == 1) {
            func_801519b4((Object *)data_8002bc7c_slot12);
        }
        for (i = 0; i < 5; i++) {
            if (data_8002bc8c_slot12[i] == 1) {
                func_801519b4((Object *)&data_8002bc94_slot12[i].cells[0]);
                func_801519b4((Object *)&data_8002bc94_slot12[i].cells[1]);
                func_801519b4((Object *)&data_8002bc94_slot12[i].cells[2]);
                if (game_state.field_2bd == 1) {
                    func_801519b4((Object *)&data_8002bc94_slot12[i].cells[3]);
                }
            }
        }
    } else if (data_8002bc8c_slot12[k] == 1) {
        func_80014300_slot12(obj, data_8002bdd4_slot12 + k * 4 + data_801a27d0 * 2);
    }
}
