/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void clear_sixteen_slot_words_slot04_0a(Object *obj) {
    int i;
    u32 z = 0;
    u32 *p = (u32 *)obj->slots;

    for (i = 0xf; i >= 0; i--) {
        *p++ = z;
    }
}

void store_image_rows_for_object_slot04_0a(Object *obj) {
    Rect r;
    u8 *p;
    u8 *src;

    src = table_801a27e4 + (obj->image_row << 5);
    p = table_801a27e4;
    r.x = (obj->side << 5) + 0x110;
    r.y = (obj->field_d4 << 2) + 0x1e8;
    r.w = 0x10;
    r.h = 4;
    func_80158028(&r, src);
    p += 0x1400;
    func_80158028(&r, p + (obj->image_row << 5));
    queue_image_upload_job(0, 6);
    store_image_rows_by_mode_and_queue_job(obj, 2);
}

void clear_kind10_flag_and_call_helper_slot04_0a(Object *obj) {
    obj->kind10_flag = 0;
    call_side_func_then_queue_job(obj);
}

