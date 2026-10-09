/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_800518fc_slot28[];
extern ObjectRef data_80051900_slot28;
void func_80016e7c_slot28(void);
void func_80016e28_slot28(Object *obj, int arg);
void func_80128370(void);

/* The local n holds the byte read from the object and then the half counter of the hud; c holds the constant stored in the hud. Written with one local for both, this function differs from the original in 6 instruction slots. */
void func_80016828_slot28(Object *obj) {
    int c = 0x258;
    u16 n;
    HudState *h;
    n = obj->field_f0;
    if (n == 0) {
        h = data_8018f5a0;
        h->field_60 = c;
        n = h->field_52;
        n++;
        h->field_52 = n;
        func_80016e7c_slot28();
        data_800518fc_slot28[0]->field_01 = 0;
        data_80051900_slot28.p->field_01 = 0;
        func_80016e28_slot28(obj, 2);
        func_80128370();
    }
}
