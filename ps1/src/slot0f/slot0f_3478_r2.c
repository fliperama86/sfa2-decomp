/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Cell16 data_800e928c_slot0f;
extern Cell16 data_800e929c_slot0f[];
extern Cell16 data_800e953c_slot0f[];
extern Cell16 data_800e945c_slot0f[];
extern int data_800f0200_slot0f;
extern int data_800f0208_slot0f;
extern s8 data_800f8554_slot0f[];
void func_800e3828_slot0f(void);

void func_800e3794_slot0f(void) {
}

void func_800e379c_slot0f(Object *obj) {
}

void func_800e37a4_slot0f(void) {
    int i;
    func_801519b4(&data_800e928c_slot0f);
    for (i = 0; i < 11; i++) {
        func_801519b4(&data_800e929c_slot0f[i]);
    }
    for (i = 0; i < 26; i++) {
        func_801519b4(&data_800e953c_slot0f[i]);
    }
    func_800e3828_slot0f();
}

void func_800e3828_slot0f(void) {
    data_800f0200_slot0f++;
    if (data_800f0200_slot0f == 0x78) {
        s8 *q = &data_800f8554_slot0f[0xb];
        data_800f0200_slot0f = 0;
        *q = (data_800f0208_slot0f < *q + 1) ? 0 : *q + 1;
    }
    func_801519b4(&data_800e945c_slot0f[data_800f8554_slot0f[0xb]]);
}
