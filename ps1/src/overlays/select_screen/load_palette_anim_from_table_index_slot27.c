/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u16 *data_80026c30_slot27[];
extern Slot27Rec9328 data_80029328_slot27[];

void load_palette_anim_from_table_index_slot27(Object *obj) {
    u16 *src;
    int i;
    int base;
    int a;

    src = data_80026c30_slot27[*(int *)&game_state.field_354];
    data_80029328_slot27[game_state.other_object->side].cur = src;
    a = ((s16 *)src)[1];
    data_80029328_slot27[game_state.other_object->side].count = a;
    data_80029328_slot27[game_state.other_object->side].current_word = src[0];
    i = 0;
    base = (data_80029328_slot27[game_state.other_object->side].current_word & 0x1f) << 5;
    obj->y_scale = data_80029328_slot27[game_state.other_object->side].current_word & 0x8000;
    for (; i < 16; i++) {
        data_801a2b84[game_state.other_object->side * 16 + i] = *(u16 *)((u8 *)0x800ed800 + base + i * 2);
        data_801a2b84[0xa00 + game_state.other_object->side * 16 + i] = *(u16 *)((u8 *)0x800ed800 + base + i * 2);
    }
    queue_image_upload_job(0, 0);
}
