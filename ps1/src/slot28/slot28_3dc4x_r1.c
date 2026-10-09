/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot28Rec51844 data_80051844_slot28[];
void func_80013dc4_slot28(Object *obj, SeqRec *rec);

void func_80013dc4_slot28(Object *obj, SeqRec *rec) {
    int i = 0;
    Slot28Rec51844 *r = data_80051844_slot28;
    u32 *src = (u32 *)rec->data;
    do {
        r->field_00 = 0;
        r->field_08 = 8;
        r->field_09 = 0x10;
        r->field_0a = 1;
        r->field_0b = 0x10;
        if (obj->field_3a & 1) {
            r->field_04 = 0x26;
            r->field_06 = 0xba;
        } else {
            r->field_04 = 0x26;
            r->field_06 = i + 0xb2;
        }
        i += 0x10;
        r->field_0c = *src;
        r++;
        src++;
    } while (*src != -1);
}
