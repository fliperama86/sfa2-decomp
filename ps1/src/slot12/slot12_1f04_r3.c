/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8002d56c_slot12;
extern int data_80029348_slot12;
extern int data_8002d578_slot12;
extern int data_8002a390_slot12;
extern SequenceStep *data_80022c08_slot12[];
int fade_palette_toward_target_slot12(void);
void func_800122cc_slot12(Object *obj);

void update_palette_fade_and_check_flags_slot12(Object *obj) {
    if ((game_state.loop_counter & 2) && data_8002a390_slot12 != 0) {
        data_8002a390_slot12 = fade_palette_toward_target_slot12();
    }
    if (data_8002d578_slot12 == 0) {
        func_800122cc_slot12(obj);
    }
}

void load_velocities_from_table_slot12(Object *obj) {
    func_800122cc_slot12(obj);
}

void noop_slot00_2_slot12(Object *obj) {
}

void func_800121cc_slot12(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void set_sequence_and_copy_to_shared_object_slot12(Object *obj, int a) {
    set_sequence_from_table_5(obj, (s16)(a + data_80029348_slot12 * 3), data_80022c08_slot12);
    data_8002d56c_slot12->sequence = obj->sequence;
}
