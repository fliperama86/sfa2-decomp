/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e9bf0_slot06_0e(Slot06Cursor *cur);

/* Declared int and returns nothing. With void this compiler fills the delay slot of a branch to the exit with a write to v0 and two instruction slots differ. The form is compatible with the original's bytes; what the original source declared is not known. */
int func_801e9af8_slot06_0e(Object *obj) {
    s16 x;
    if (game_state.field_65 != 0 || game_state.field_74 != 0 || ((Slot06Layer *)cam_obj)->field_8b != 0) return;
    if (((Slot06Layer *)data_801aa544)->field_05 == 2) {
        obj->field_05++;
    }
    x = ((Slot06Layer *)data_801aa5d4)->field_12;
    if (x < 0x180) {
        func_801e9bf0_slot06_0e((Slot06Cursor *)((u8 *)obj + 0x30));
        func_801e9bf0_slot06_0e((Slot06Cursor *)((u8 *)obj + 0x40));
    } else if (x < 0x200) {
        func_801e9bf0_slot06_0e((Slot06Cursor *)((u8 *)obj + 0x40));
        func_801e9bf0_slot06_0e((Slot06Cursor *)((u8 *)obj + 0x50));
    } else {
        func_801e9bf0_slot06_0e((Slot06Cursor *)((u8 *)obj + 0x50));
        func_801e9bf0_slot06_0e((Slot06Cursor *)((u8 *)obj + 0x6c));
    }
}
