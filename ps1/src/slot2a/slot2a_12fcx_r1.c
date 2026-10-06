/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot2aBank data_801e2fdc_slot2a[];
extern u32 data_801e515c_slot2a;
extern void (*data_801e2300_slot2a[])(Object *);
extern void (*data_801e230c_slot2a[])(Object *);
void func_801e12fc_slot2a(Object *obj);
void func_801e16a8_slot2a(Object *obj);

void func_801e12fc_slot2a(Object *obj) {
    data_801e230c_slot2a[obj->field_05](obj);
    if ((s16)obj->field_5c != 0) {
        func_801e16a8_slot2a(obj);
    }
}
