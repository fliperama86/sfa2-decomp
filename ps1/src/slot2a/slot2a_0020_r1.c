/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801e1c8c_slot2a[])(void);
extern Block172 *data_801e25c8_slot2a[4];
void func_801e05b4_slot2a(Object *obj);
void func_801e1244_slot2a(Object *obj);

void func_801e0020_slot2a(void) {
    data_801e1c8c_slot2a[data_8018f5a0->field_52]();
    if (data_8018f5a0->field_52 >= 3) {
        if (*(u8 *)*data_801e25c8_slot2a != 0) {
            func_801e05b4_slot2a((Object *)*data_801e25c8_slot2a);
        }
        if (*(u8 *)data_801e25c8_slot2a[1] != 0) {
            func_801e05b4_slot2a((Object *)data_801e25c8_slot2a[1]);
        }
        if (*(u8 *)data_801e25c8_slot2a[2] != 0) {
            func_801e1244_slot2a((Object *)data_801e25c8_slot2a[2]);
        }
        if (*(u8 *)data_801e25c8_slot2a[3] != 0) {
            func_801e1244_slot2a((Object *)data_801e25c8_slot2a[3]);
        }
    }
}
