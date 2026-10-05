/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8016a3b0(void);

void func_801503f8(Stream *stream, StreamEntry *entry) {
    Rect rect;
    rect.x = entry->field_0c;
    rect.y = entry->field_0e;
    rect.w = entry->field_10;
    rect.h = entry->field_12;
    func_80157fc4(&rect, (u8 *)0x801e0000 + (stream->field_17 << 11));
    func_80157d9c(0);
}

void func_80150460(Stream *stream, StreamEntry *entry) {
    Rect rect;
    rect.x = entry->field_0c;
    rect.y = entry->field_0e;
    rect.w = 0x40;
    rect.h = 0x10;
    func_80157fc4(&rect, (u8 *)0x801e0000 + (stream->field_17 << 11));
    func_80157d9c(0);
}

void func_801504bc(Stream *stream, StreamEntry *entry) {
    int r;
    if (entry->field_0c == 0xffff) {
        entry->field_0c = 0;
    } else {
        func_801691ac(1);
    }
    r = func_8015fe50((u8 *)0x801e0000 + (stream->field_17 << 11), entry->field_04, (s16)entry->field_0e);
    stream->field_48 = stream->field_48 - entry->field_04;
    if (stream->field_48 == 0) {
        if ((s16)r == entry->field_0e) {
            func_801691ac(1);
            stream->field_1c = 0;
        }
    }
    if ((s16)r == -1) {
        stream->field_14 = 2;
    }
}
