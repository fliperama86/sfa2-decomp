/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void queue_two_digit_number_text(Actor *a) {
    u8 d[2];
    int i;
    u8 c;
    unsigned n;
    TextObj *t;
    if (a->number_value >= 100) {
        a->number_value = 99;
    }
    t = table_801803c4[a->side];
    n = a->number_value;
    i = 0;
    do {
        d[i] = n % 10;
        n /= 10;
        i++;
    } while (i < 2);
    n = d[1];
    if (n != 0) {
        t->buf[0] = n + 0x30;
    } else {
        t->buf[0] = 0x20;
    }
    t->buf[1] = d[0] + 0x30;
    add_item_to_table(t);
}
