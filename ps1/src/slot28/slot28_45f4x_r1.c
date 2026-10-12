/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8005187c_slot28[];
void start_fade_mode_1a01_and_wait(void);
void func_80014854_slot28(Object *obj, int arg);

/* When the object's field_f0 is 0, sets the HUD state's countdown_timer to 600 and
   adds 1 to its pass_index, calls func_80014854_slot28 with the object and 4,
   clears field_01 of three objects held in a table of the module, and calls
   start_fade_mode_1a01_and_wait (inferred). */
void advance_pass_and_hide_three_objects_slot28(Object *obj) {
    HudState *h;

    if (obj->field_f0 == 0) {
        /* Stand-in, not recovered source: three nested do-while wrappers around the statement that loads the HUD pointer reproduce the original's choice of registers. Without it this function differs from the original in 11 instruction slots. */
        /* With two wrappers: 6 slots. Inferred: the compiler counts a reference inside a wrapper twice for each level, so the pointer outranks the count of pass_index and takes v0. */
        do { do { do { h = data_8018f5a0; } while (0); } while (0); } while (0);
        h->countdown_timer = 0x258;
        h->pass_index++;
        func_80014854_slot28(obj, 4);
        data_8005187c_slot28[0]->field_01 = 0;
        data_8005187c_slot28[1]->field_01 = 0;
        data_8005187c_slot28[2]->field_01 = 0;
        start_fade_mode_1a01_and_wait();
    }
}
