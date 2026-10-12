/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot0fRec80a4 data_800f80a4_slot0f[];
int strcmp(const char *a, const char *b);
int check_memory_card_status_slot0f(int chan);
void list_card_files_into_table_slot0f(int card);

int find_named_file_on_memory_cards_slot0f(int mask, char *name) {
    u8 z = 0;
    int i;
    int status;
    int result;
    u8 *p = (u8 *)data_800f80a4_slot0f;
    u8 *q;

    i = 0x4b0;
    do {
        *p++ = z;
        i--;
    } while (i != 0);
    result = z;
    if (mask & 1) {
        status = check_memory_card_status_slot0f(0);
        if (status == 0 || status == 2) {
            list_card_files_into_table_slot0f(0);
            q = (u8 *)data_800f80a4_slot0f;
            do {
                p = q;
                if (strcmp((char *)p, name) == 0) {
                    result |= 1;
                    break;
                }
                i++;
                q = p + 0x28;
            } while (i < 0x10);
        }
    }
    if (mask & 2) {
        status = check_memory_card_status_slot0f(0x10);
        if (status == 0 || status == 2) {
            list_card_files_into_table_slot0f(1);
            i = 0;
            p = (u8 *)data_800f80a4_slot0f + 0x258;
            do {
                if (strcmp((char *)p, name) == 0) {
                    result |= 2;
                    break;
                }
                i++;
                p += 0x28;
            } while (i < 0x10);
        }
    }
    return result;
}
