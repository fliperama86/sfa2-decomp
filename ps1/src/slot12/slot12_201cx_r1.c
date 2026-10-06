/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8002d56c_slot12;
extern int data_8002d578_slot12;
extern int data_8002a390_slot12;
extern ObjectFn data_80022e20_slot12[];
void func_800121ec_slot12(Object *obj, int a);
void func_800124fc_slot12(Object *obj, FrameRecord *f);

void func_8001201c_slot12(Object *obj) {
    Object *p;
    FrameRecord *f;

    data_80022e20_slot12[obj->field_05](obj);
    func_80131094(obj);
    data_8002d56c_slot12->sequence = obj->sequence;
    p = data_8002d56c_slot12;
    if (p->side != 0) {
        f = frames_right;
    } else {
        f = frames_left;
    }
    func_800124fc_slot12(p, f);
    func_80120028(obj);
}
