/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801c5500_slot04_0f[][0x10];

void call_side_routine_and_set_side_flag_slot04_0f(Object *obj);
void queue_16_word_row_upload_job_slot04_0f(Object *obj, u16 *src);

/* What it does (inferred, not an original name): a character object holds a
   wanted state byte in mode and the state it last acted on in field_339.
   When they differ it copies mode to field_339. If bit 7 of the new value
   is set it calls func_801b22ec (one argument, the object). Otherwise it
   calls func_801b2340 with the object and a row of 16 halfwords of a table,
   data_801c5500_slot04_0f, chosen by field_339 plus 5 times the object's
   field_d4. When the two bytes are equal it does nothing. */
void update_mode_change_select_row_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u16 (*p)[0x10];

    if (obj->mode != obj->field_339) {
        obj->field_339 = obj->mode;
        if (obj->field_339 & 0x80) {
            call_side_routine_and_set_side_flag_slot04_0f(o);
        } else {
            /* A pointer to a row is set to the table's first row and then stepped by the row's number, in two statements. With the row taken by its index in one expression (data_801c5500_slot04_0f[obj->field_339 + o->field_d4 * 5]) this function differs from the original in 8 instruction slots. */
            p = data_801c5500_slot04_0f;
            p += obj->field_339 + o->field_d4 * 5;
            queue_16_word_row_upload_job_slot04_0f(o, *p);
        }
    }
}
