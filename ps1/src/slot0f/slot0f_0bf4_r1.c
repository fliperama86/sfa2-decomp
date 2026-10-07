/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot0fRec80a4 data_800f80a4_slot0f[];
int strcmp(const char *a, const char *b);
int func_800e0da8_slot0f(int chan);
void func_800e0f2c_slot0f(int card);

int func_800e0bf4_slot0f(int mask, char *name) {
    u8 z = 0;
    int i;
    int status;
    int result;
    u8 *p = (u8 *)data_800f80a4_slot0f;
    u8 *q;

    i = 0x4b0;
    do {
        *p++ = z;
        i--;
    } while (i != 0);
    result = z;
    if (mask & 1) {
        status = func_800e0da8_slot0f(0);
        if (status == 0 || status == 2) {
            func_800e0f2c_slot0f(0);
            q = (u8 *)data_800f80a4_slot0f;
            do {
                p = q;
                if (strcmp((char *)p, name) == 0) {
                    result |= 1;
                    break;
                }
                i++;
                q = p + 0x28;
            } while (i < 0x10);
        }
    }
    if (mask & 2) {
        status = func_800e0da8_slot0f(0x10);
        if (status == 0 || status == 2) {
            func_800e0f2c_slot0f(1);
            i = 0;
            p = (u8 *)data_800f80a4_slot0f + 0x258;
            do {
                if (strcmp((char *)p, name) == 0) {
                    result |= 2;
                    break;
                }
                i++;
                p += 0x28;
            } while (i < 0x10);
        }
    }
    return result;
}
