/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Exact. The first argument register is never read, so it is an unused int;
   the fill loop is written with goto so the constants stay inside the loop. */

extern u16 data_801a6966;

void func_80156d28(int unused, NameEntry *o) {
    s16 i;
    u16 raw;
    s16 pad;
    int j;
    raw = data_801a6966;
    if (o->side) {
        raw = data_801a6972;
    }
    pad = raw;
    if (pad == 0) {
        o->field_b2 = -0x1000;
        return;
    }
    o->field_b0 = 0x258;
    o->field_b2 = o->field_b2 + 0x1000;
    if (o->field_b2 != 0) {
        return;
    }
    i = o->cursor;
    if (raw & 0x40) {
        o->chars[i] = 0x60;
        if (o->cursor == 3) {
            o->chars[i] = 0x20;
        }
        o->cursor = o->cursor - 1;
        if (o->cursor & 0x80) {
            o->cursor = 0;
        }
    } else if (raw & 0x20) {
        if (o->chars[i] == 0x5f) {
fill:
            o->chars[i] = 0x20;
            o->cursor = o->cursor + 1;
            i++;
            if (o->cursor != 4) goto fill;
            o->chars[3] = 0x20;
        } else if (o->chars[i] == 0x5e) {
            o->chars[i] = 0x60;
            if (o->cursor == 3) {
                o->chars[i] = 0x20;
            }
            o->cursor = o->cursor - 1;
            if (o->cursor & 0x80) {
                o->cursor = 0;
                o->chars[i] = 0x5e;
            }
        } else {
            o->chars[i + 1] = o->chars[i];
            o->cursor = o->cursor + 1;
            if (o->cursor == 3) {
                o->chars[i + 1] = 0x5f;
            }
        }
    } else if (raw & 0x800) {
        o->field_b4 = 0;
        for (j = 0; j < 4; j++) {
            if (o->chars[j] == 0x5e) {
                o->chars[j] = 0x20;
            } else if (o->chars[j] == 0x5f) {
                o->chars[j] = 0x20;
            }
        }
    } else if (pad & 0x8000) {
        if (o->cursor == 3) {
            if (o->chars[i] == 0x5e) {
                o->chars[i] = 0x5f;
            } else if (o->chars[i] == 0x5f) {
                o->chars[i] = 0x5e;
            }
        } else {
            if (o->chars[i] == 0x41) {
                o->chars[i] = 0x5f;
            } else if (o->chars[i] == 0x30) {
                o->chars[i] = 0x5a;
            } else if (o->chars[i] == 0x5b) {
                o->chars[i] = 0x3f;
            } else if (o->chars[i] == 0x20) {
                o->chars[i] = 0x60;
            } else if (o->chars[i] == 0x60) {
                o->chars[i] = 0x5d;
            } else if (o->chars[i] == 0x5e) {
                o->chars[i] = 0x20;
            } else {
                o->chars[i] = o->chars[i] - 1;
            }
        }
    } else if (raw & 0x2000) {
        if (o->cursor == 3) {
            if (o->chars[i] == 0x5e) {
                o->chars[i] = 0x5f;
            } else if (o->chars[i] == 0x5f) {
                o->chars[i] = 0x5e;
            }
        } else {
            if (o->chars[i] == 0x60) {
                o->chars[i] = 0x20;
            } else if (o->chars[i] == 0x5a) {
                o->chars[i] = 0x30;
            } else if (o->chars[i] == 0x3f) {
                o->chars[i] = 0x5b;
            } else if (o->chars[i] == 0x5d) {
                o->chars[i] = 0x60;
            } else if (o->chars[i] == 0x20) {
                o->chars[i] = 0x5e;
            } else if (o->chars[i] == 0x5f) {
                o->chars[i] = 0x41;
            } else {
                o->chars[i] = o->chars[i] + 1;
            }
        }
    }
}
