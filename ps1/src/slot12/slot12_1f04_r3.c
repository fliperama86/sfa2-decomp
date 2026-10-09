/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8002d56c_slot12;
extern int data_80029348_slot12;
extern int data_8002d578_slot12;
extern int data_8002a390_slot12;
extern SequenceStep *data_80022c08_slot12[];
int func_80011688_slot12(void);
void func_800122cc_slot12(Object *obj);

void func_80012130_slot12(Object *obj) {
    if ((game_state.field_33 & 2) && data_8002a390_slot12 != 0) {
        data_8002a390_slot12 = func_80011688_slot12();
    }
    if (data_8002d578_slot12 == 0) {
        func_800122cc_slot12(obj);
    }
}

void func_800121a4_slot12(Object *obj) {
    func_800122cc_slot12(obj);
}

void func_800121c4_slot12(Object *obj) {
}

void func_800121cc_slot12(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_800121ec_slot12(Object *obj, int a) {
    func_80130768(obj, (s16)(a + data_80029348_slot12 * 3), data_80022c08_slot12);
    data_8002d56c_slot12->sequence = obj->sequence;
}
