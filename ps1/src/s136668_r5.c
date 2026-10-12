/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void call_handler_by_kind_index_a(void) {
    table_80172500[game_state.kind_index]();
}

void call_kind_handler_if_flag_set(void) {
    if (data_80190568 != 0) {
        table_801725a0[game_state.kind_index]();
    }
}

void init_draw_mode_prim_3(Tx *tx) {
    func_80158a2c(tx, 0, 0, 0, 0);
    tx->mode_id = 3;
    tx->prim_code = 0x7c;
    func_8015c23c(tx, &tx->prim_data);
}

void init_draw_mode_prim_4(Tx *tx) {
    func_80158a2c(tx, 0, 0, 0, 0);
    tx->mode_id = 4;
    tx->prim_code = 0x64;
    func_8015c23c(tx, &tx->prim_data);
}

void store_all_slot_image_rects_and_queue_jobs(void) {
    store_image_rects_and_queue_upload_jobs();
    store_image_rect_d0_1e0_slot_1();
    store_image_rect_e0_1e0_slot_2();
    store_image_rect_f0_1e0_slot_3();
}

void store_image_rects_and_queue_upload_jobs(void) {
    store_image_rect_b0_1f0_to_table();
    store_image_rect_c0_1e0_slot_0();
    store_image_rect_160_1f0_slot_4();
    call_left_and_right_func_pair();
    store_image_pair_to_table(4, 0, 0xb0, 0x1e0, 0xe);
    queue_image_upload_job(0, 6);
    queue_image_upload_job(4, 7);
}

void call_left_and_right_func_pair(void) {
    store_player_left_image_rows_from_field();
    store_player_right_image_rows_from_field();
}

void store_player_left_image_rows_from_field(void) {
    store_image_rows_to_table(&player_left, 0x10, 0x100, player_left.field_d4 * 5 + 0x1e0);
}

void store_player_right_image_rows_from_field(void) {
    store_image_rows_to_table(&player_right, 0xb, 0x120, player_right.field_d4 * 5 + 0x1e0);
}

void store_image_rows_to_table(Object *object, short index, int w, int h) {
    Rect rect;

    object->field_f4 = index;
    rect.x = w;
    rect.y = h;
    rect.w = 0x10;
    rect.h = 5;
    func_80158028(&rect, table_801a27e4 + (index << 5));
    func_80158028(&rect, table_801a27e4 + 0x1400 + (index << 5));
    data_801aa4dc[0] = 0x1f;
}
