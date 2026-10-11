/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8005187c_slot28[];
void func_80128370(void);
void func_80014854_slot28(Object *obj, int arg);

/* When the object's field_f0 is 0, sets the HUD state's field_60 to 600 and
   adds 1 to its field_52, calls func_80014854_slot28 with the object and 4,
   clears field_01 of three objects held in a table of the module, and calls
   func_80128370 (inferred). */
void func_800145f4_slot28(Object *obj) {
    HudState *h;

    if (obj->field_f0 == 0) {
        /* Stand-in, not recovered source: three nested do-while wrappers around the statement that loads the HUD pointer reproduce the original's choice of registers. Without it this function differs from the original in 11 instruction slots. */
        /* With two wrappers: 6 slots. Inferred: the compiler counts a reference inside a wrapper twice for each level, so the pointer outranks the count of field_52 and takes v0. */
        do { do { do { h = data_8018f5a0; } while (0); } while (0); } while (0);
        h->field_60 = 0x258;
        h->field_52++;
        func_80014854_slot28(obj, 4);
        data_8005187c_slot28[0]->field_01 = 0;
        data_8005187c_slot28[1]->field_01 = 0;
        data_8005187c_slot28[2]->field_01 = 0;
        func_80128370();
    }
}
