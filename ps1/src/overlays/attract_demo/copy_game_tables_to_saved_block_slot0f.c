/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void copy_rece664_record_slot0f(Slot0fRece664 *src, Slot0fRece664 *dst);
void copy_six_rece5c4_records_slot0f(Slot0fRece5c4 *src, Slot0fRece5c4 *dst);
void copy_64_bytes_slot0f(u8 *src, u8 *dst);

extern u8 *data_800e8504_slot0f;

void copy_game_tables_to_saved_block_slot0f(void) {
    copy_rece664_record_slot0f((Slot0fRece664 *)table_8016e664, (Slot0fRece664 *)(data_800e8504_slot0f + 0x100));
    copy_six_rece5c4_records_slot0f((Slot0fRece5c4 *)score_rank_table, (Slot0fRece5c4 *)(data_800e8504_slot0f + 0x130));
    copy_six_rece5c4_records_slot0f((Slot0fRece5c4 *)streak_rank_table, (Slot0fRece5c4 *)(data_800e8504_slot0f + 0x190));
    copy_64_bytes_slot0f((u8 *)&game_state + 0x15e, data_800e8504_slot0f + 0x1f0);
    *(u16 *)(data_800e8504_slot0f + 0x230) = *(u16 *)((u8 *)&game_state + 0x21e);
}
