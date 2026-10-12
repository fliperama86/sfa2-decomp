/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void count_down_or_skip_to_next_step_2(Object *object) {
    if ((u8)is_any_enabled_pad_button_pressed() != 0) {
        data_8018f5a0->step_index_2 += 2;
        object->phase_state = 1;
    } else {
        int n = data_8018f5a0->countdown_timer - 1;
        data_8018f5a0->countdown_timer = n;
        if ((s16)n < 0) {
            data_8018f5a0->countdown_timer = 0x3f;
            data_8018f5a0->step_index_2++;
            scale_palette_rows_and_queue_four_uploads(object);
        }
    }
}

void scale_palette_rows_and_queue_four_uploads(Object *object) {
    scale_palette_rows_from_backup_bank(0, 4, 0, 0x15);
    scale_palette_rows_from_backup_bank(0, 4, 0, 0x15);
    scale_palette_rows_from_backup_bank(0, 7, 0x10, 0);
    scale_palette_rows_from_backup_bank(1, 0x20, 0x10, 0);
    scale_palette_rows_from_backup_bank(2, 0x20, 0x10, 0);
    scale_palette_rows_from_backup_bank(3, 0x20, 0x10, 0);
    queue_image_upload_job(0, 6);
    queue_image_upload_job(1, 0);
    queue_image_upload_job(2, 1);
    queue_image_upload_job(3, 2);
}

void advance_step_when_countdown_expires(Object *object) {
    int n;
    if ((u8)is_any_enabled_pad_button_pressed() != 0 || (n = data_8018f5a0->countdown_timer - 1, data_8018f5a0->countdown_timer = n, (s16)n < 0)) {
        data_8018f5a0->step_index_2++;
        data_80190a40 = 1;
        fill_row_columns_white_once();
        object->phase_state = 1;
    }
}

void advance_step_index_2_and_restore_then_clear_rows(Object *unused) {
    data_8018f5a0->step_index_2++;
    restore_rows_from_backup_rows();
    clear_row_columns_once_and_update_jobs();
}

void advance_step_index_2_and_set_countdown_timer(Object *object) {
    data_8018f5a0->step_index_2++;
    data_80190a40 = 0;
    restore_rows_from_backup_rows();
    data_8018f5a0->countdown_timer = 0x78;
    object->team_id = 0;
}

void wait_countdown_then_set_phase_state_2(Object *object) {
    int n;
    if ((u8)is_any_enabled_pad_button_pressed() != 0) {
        set_menu_command_by_code(6, 3);
    }
    if ((u8)is_any_enabled_pad_button_pressed() != 0 || (n = data_8018f5a0->countdown_timer - 1, data_8018f5a0->countdown_timer = n, (s16)n < 0)) {
        data_8018f5a0->step_index_2++;
        data_8018f5a0->countdown_timer = 0x1c;
        object->phase_state = 2;
    }
}
