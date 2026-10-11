/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079a2c_slot2b[];
extern s16 data_8007ef2c_slot2b;
void func_80078554_slot2b(Object *o);

void func_80078338_slot2b(Object *o) {
    func_80078554_slot2b(o);
    if (data_8007ef2c_slot2b >= o->pos_y) {
        func_80131094(o);
    } else {
        o->state_2++;
        o->pos_y_frac = 0;
        o->vel_y = 0;
        o->accel_y = 0;
        o->pos_y = data_8007ef2c_slot2b;
        func_80138070(o, 5);
    }
}
