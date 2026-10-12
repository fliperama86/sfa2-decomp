/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_cursor_record_countdown_slot06_0e(Slot06Cursor *cur);

/* Declared int and returns nothing. With void this compiler fills the delay slot of a branch to the exit with a write to v0 and two instruction slots differ. The form is compatible with the original's bytes; what the original source declared is not known. */
int update_cursors_by_scroll_x_range_slot06_0e(Object *obj) {
    s16 x;
    if (game_state.field_65 != 0 || game_state.field_74 != 0 || ((Slot06Layer *)cam_obj)->field_8b != 0) return;
    if (((Slot06Layer *)data_801aa544)->state_index == 2) {
        obj->state_1++;
    }
    x = ((Slot06Layer *)data_801aa5d4)->scroll_x;
    if (x < 0x180) {
        advance_cursor_record_countdown_slot06_0e((Slot06Cursor *)((u8 *)obj + 0x30));
        advance_cursor_record_countdown_slot06_0e((Slot06Cursor *)((u8 *)obj + 0x40));
    } else if (x < 0x200) {
        advance_cursor_record_countdown_slot06_0e((Slot06Cursor *)((u8 *)obj + 0x40));
        advance_cursor_record_countdown_slot06_0e((Slot06Cursor *)((u8 *)obj + 0x50));
    } else {
        advance_cursor_record_countdown_slot06_0e((Slot06Cursor *)((u8 *)obj + 0x50));
        advance_cursor_record_countdown_slot06_0e((Slot06Cursor *)((u8 *)obj + 0x6c));
    }
}
