/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_801bdb28_slot04_0c[];
extern s8 data_801bdb48_slot04_0c[];
extern s8 data_801bdb68_slot04_0c[];

void set_sequence_by_side(Object *object, int arg);

void func_801b3600_slot04_0c(Object *object) {
    int a;

    object->state_2 = object->state_2 + 1;
    object->field_46 = 0x3c;
    if (game_state.field_76 == 0) {
        game_state.field_76 = 0x1e;
    }
    if (object->other->kind == 0xb) {
        if (game_state.field_90 != 0) {
            set_sequence_by_side(object, 0x26);
        } else {
            set_sequence_by_side(object, (s8)func_80125734(object, (s8)data_801bdb68_slot04_0c[get_random_byte() & 0x1f]) + 0x23);
        }
    } else if (((Slot04bObj *)object)->field_5c >= 0x90 && (get_random_byte() & 0x3f) != 0) {
        set_sequence_by_side(object, (s8)func_80125734(object, 2) + 0x23);
    } else {
        a = get_random_byte() & 0x1f;
        if (((Slot04bObj *)object)->field_5c >= 0x80) a = data_801bdb48_slot04_0c[a]; else a = data_801bdb28_slot04_0c[a];
        set_sequence_by_side(object, (s8)func_80125734(object, (s8)a) + 0x23);
    }
}
