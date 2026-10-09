/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Cell16 data_800e929c_slot0f[];
extern int data_800f0200_slot0f;
extern int data_800f0204_slot0f;
extern int data_800f0208_slot0f;
extern int data_800f8578_slot0f;
void func_800e3478_slot0f(void);
void func_800e3794_slot0f(void);
void func_800e3a14_slot0f(void);

void func_800e27ec_slot0f(void) {
    HudState *h = data_8018f5a0;
    int i;
    data_800f0200_slot0f = 0;
    data_800f0204_slot0f = 0;
    data_800f0208_slot0f = 1;
    data_800f8578_slot0f = 0;
    h->field_4c = h->field_4c + 1;
    func_800e3478_slot0f();
    func_800e3794_slot0f();
    func_800e3a14_slot0f();
    for (i = 10; i >= 0; i--) {
        data_800e929c_slot0f[i].field_0b = 0x1a;
    }
    data_800e929c_slot0f[0].field_0b = 0x10;
}
