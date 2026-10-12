/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Count data_800f7b74_slot0f[4];
void count_records_with_high_bit_slot0f(Object *obj);
void count_record_bytes_in_range_c_to_f_slot0f(Object *obj);
void count_records_with_type_16_or_more_slot0f(Object *obj);
void func_800e6d44_slot0f(Object *obj);

void copy_string_with_prefix_and_terminator_slot0f(char *src, char *dst) {
    if (*src != 0) {
        *dst++ = 0x78;
        do {
            *dst++ = *src++;
        } while (*src != 0);
    }
    *dst++ = 0x40;
    *dst = 0x40;
    dst[1] = 0;
}

void count_record_bytes_into_four_counters_slot0f(Object *obj) {
    count_records_with_high_bit_slot0f(obj);
    count_record_bytes_in_range_c_to_f_slot0f(obj);
    count_records_with_type_16_or_more_slot0f(obj);
    func_800e6d44_slot0f(obj);
}

void count_records_with_high_bit_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    u8 *p;
    int n;
    int i;

    data_800f7b74_slot0f[0].value = 0;
    p = &data_80190468.p->accum_count;
    i = obj->record_offset;
    n = 0;
    if (*p != 0xff) {
        do {
            if (*p++ & 0x80) {
                n++;
            }
            i--;
        } while (i >= 0);
    }
    data_800f7b74_slot0f[0].value = n;
}

void count_record_bytes_in_range_c_to_f_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    u8 *p;
    int n;
    int i;
    int v;

    data_800f7b74_slot0f[1].value = 0;
    p = &data_80190468.p->accum_count;
    i = obj->record_offset;
    n = 0;
    if (*p != 0xff) {
        do {
            v = *p++ & 0x7f;
            if ((unsigned)(v - 0xc) < 2 | (unsigned)(v - 0xe) < 2) {
                n++;
            }
            i--;
        } while (i >= 0);
    }
    data_800f7b74_slot0f[1].value = n;
}
