/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot0fRec11c data_800df11c_slot0f;
extern Slot0fRec80a4 data_800f80a4_slot0f[];
extern u8 data_800df0f0_slot0f[];
extern u8 data_800df0f8_slot0f[];
Slot0fRec80a4 *firstfile(u8 *name, Slot0fRec80a4 *entry);
Slot0fRec80a4 *nextfile(Slot0fRec80a4 *entry);
char *strcpy(char *dest, const char *src);
long format(u8 *name);
int func_800e0da8_slot0f(int chan);

int func_800e1014_slot0f(int card) {
    u8 *name;
    int status;

    name = data_800df0f8_slot0f;
    if (card == 0) {
        name = data_800df0f0_slot0f;
    }
    format(name);
    status = func_800e0da8_slot0f(card);
    if (status == 0 || status == 2) {
        return 0;
    }
    return 1;
}
