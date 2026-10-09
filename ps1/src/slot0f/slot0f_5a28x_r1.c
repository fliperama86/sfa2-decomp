/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_800f728c_slot0f[16];
extern u16 data_800f77ac_slot0f;
int func_800e5ddc_slot0f(u8 a, int b, u16 *dst, u16 *src);
void func_800e5364_slot0f(void);
void func_800e52e0_slot0f(void);
void func_800e5ae4_slot0f(Slot0fObj *obj);
void func_800e5a9c_slot0f(Slot0fObj *obj, int flag);

/* The call is written once with 1 and once with 0 in the two branches of the test: written as one call with the test as its argument, this function differs from the original in 8 instruction slots. */
void func_800e5a28_slot0f(Object *obj) {
    if (((Slot0fObj *)obj)->field_01 != 0) {
        func_800e5ae4_slot0f((Slot0fObj *)obj);
    } else {
        data_8018f5a0->field_4a++;
        if (game_state.field_2bd == 0) {
            func_800e5a9c_slot0f((Slot0fObj *)obj, 1);
        } else {
            func_800e5a9c_slot0f((Slot0fObj *)obj, 0);
        }
    }
}
