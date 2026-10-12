/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot28Rec51844 data_80051844_slot28[];
void init_records_from_sequence_data_words_slot28(Object *obj, SeqRec *rec);

void init_records_from_sequence_data_words_slot28(Object *obj, SeqRec *rec) {
    int i = 0;
    Slot28Rec51844 *r = data_80051844_slot28;
    u32 *src = (u32 *)rec->data;
    do {
        r->field_00 = 0;
        r->init_size_a = 8;
        r->init_size_b = 0x10;
        r->init_param = 1;
        r->init_byte = 0x10;
        if (obj->step_flags & 1) {
            r->init_x = 0x26;
            r->init_y = 0xba;
        } else {
            r->init_x = 0x26;
            r->init_y = i + 0xb2;
        }
        i += 0x10;
        r->data_word = *src;
        r++;
        src++;
    } while (*src != -1);
}
