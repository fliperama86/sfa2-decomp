/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_8002d56c_slot12;
extern int data_80029348_slot12;
extern int data_8002934c_slot12;
extern int data_8002d570_slot12;
extern int data_8002d578_slot12;
extern int data_8002a390_slot12;
extern void *data_80021b34_slot12[][2];
void set_sequence_and_copy_to_shared_object_slot12(Object *obj, int a);
void copy_object_state_to_shared_object_slot12(Object *obj);

void init_object_from_shared_kind_and_set_sequence_slot12(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[16];
    int a;
    void *p;
    int k;
    u8 one;

    p = (void *)0x8004b344;
    a = 0;
    k = data_8002d56c_slot12->kind;
    one = 1;

    obj->pos_x = 0x6c;
    obj->pos_y = 0xb0;
    obj->spawn_x = 0x10;
    data_80029348_slot12 = k;
    data_8002d570_slot12 = k;
    obj->scroll_factor = 0;
    obj->field_1c = 0;
    obj->field_90 = p;
    obj->field_01 = one;
    obj->field_0c = one;
    obj->image_row = 0;
    obj->field_0e = 0;
    obj->ot_depth = 9;
    obj->field_0f = 0;
    obj->facing_side = one;
    obj->state_0 = obj->state_0 + 1;
    obj->spawn_y = 0x1e0;
    obj->data_ptr_a = data_80021b34_slot12[data_80029348_slot12][0];
    data_8002a390_slot12 = 1;
    obj->data_ptr_b = data_80021b34_slot12[data_80029348_slot12][1];
    data_8002934c_slot12 = 2;
    if (data_8002d578_slot12 != 0) {
        obj->state_1 = one;
        a = 2;
    }
    set_sequence_and_copy_to_shared_object_slot12(obj, a);
    copy_object_state_to_shared_object_slot12(obj);
}
