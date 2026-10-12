/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Slot28Rec50ce4 data_80050ce4_slot28[];
extern s16 data_8005175c_slot28[];

void update_cells_from_entry_table_slot28(Object *o) {
    Slot28Rec50ce4Obj *obj = (Slot28Rec50ce4Obj *)o;
    int i;
    int x;
    s16 h;
    Slot28Rec50ce4 *e;
    Slot28Rec51e94 *c;
    i = 0;
    if (obj->entry->field_00 != 0xff) {
        h = obj->field_06;
        obj->entry = &data_80050ce4_slot28[((h & 0xff00) >> 4) + ((u32)(h & 0xff) >> 4)];
    }
    x = 8 - ((u16)obj->field_06 & 0xf);
    e = obj->entry;
    do {
        c = &obj->cells[i];
        if (e->field_00 != 0xff) {
            c->pos_x = e->field_00;
            c->pos_y = x;
            c->entry_byte = e->entry_byte;
            c->data_word = e->data_word;
            e++;
            add_item_to_table(c);
        }
        x += 0x10;
        i++;
    } while (i < 0x10);
}

void call_menu_code_from_table_by_state_index_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    set_menu_command_one_and_wait_pending(data_8005175c_slot28[obj->state_index]);
}
