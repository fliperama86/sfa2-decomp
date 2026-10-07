/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800f012c_slot0f[])(Object *);
extern u8 data_800f7d34_slot0f;
extern u8 data_800f7d38_slot0f[];
extern u8 data_800f7d48_slot0f[];
extern Slot12Quad data_800f7d50_slot0f[];
extern Slot12Cell data_800f7e90_slot0f[];
extern ObjectRef data_80190468;
void func_800e5be4_slot0f(Object *obj, void *cell);

void func_800e7064_slot0f(Object *obj) {
    int i;
    int k;
    data_80190468.p = (Object *)&game_state;
    data_800f012c_slot0f[obj->field_04](obj);
    k = obj->field_03;
    if (k == 5) {
        i = 0;
        if (data_800f7d34_slot0f == 1) {
            func_801519b4((Object *)data_800f7d38_slot0f);
        }
        for (i = 0; i < 5; i++) {
            if (data_800f7d48_slot0f[i] == 1) {
                func_801519b4((Object *)&data_800f7d50_slot0f[i].cells[0]);
                func_801519b4((Object *)&data_800f7d50_slot0f[i].cells[1]);
                func_801519b4((Object *)&data_800f7d50_slot0f[i].cells[2]);
                if (game_state.field_2bd == 1) {
                    func_801519b4((Object *)&data_800f7d50_slot0f[i].cells[3]);
                }
            }
        }
    } else if (data_800f7d48_slot0f[k] == 1) {
        func_800e5be4_slot0f(obj, data_800f7e90_slot0f + k * 4 + data_801a27d0 * 2);
    }
}
