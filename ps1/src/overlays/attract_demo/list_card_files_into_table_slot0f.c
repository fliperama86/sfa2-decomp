/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

/* Inferred declarations; the headers lack them. */
extern Slot0fRec11c data_800df11c_slot0f;
extern Slot0fRec80a4 data_800f80a4_slot0f[];
Slot0fRec80a4 *firstfile(u8 *name, Slot0fRec80a4 *entry);
Slot0fRec80a4 *nextfile(Slot0fRec80a4 *entry);
char *strcpy(char *dest, const char *src);

/* What it does (inferred, not an original name): lists the files of one memory card. The device name is chosen by the card number (card 0 takes the first name of data_800df11c_slot0f, any other card the second). firstfile is asked for the first directory entry; while the library returns the entry buffer, its name is copied with strcpy into a table of 0x28-byte slots at data_800f80a4_slot0f + card * 0x258, and nextfile is asked for the next entry. */
void list_card_files_into_table_slot0f(int card) {
    /* The local holds a constant that no instruction reads: the original sets a register to this value here and never uses it. It is a stand-in for a variable of the original source, not recovered source. The name is inferred. */
    /* The original's instruction: move s2,zero at 800e0f90; nothing reads s2 after it. With 0 written at the local's use this function differs from the original in 5 instruction slots. */
    u8 fine = 0;
    Slot0fRec80a4 entry;
    Slot0fRec11c names = data_800df11c_slot0f;
    int base;
    int off;
    u8 *name;

    /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
    do {
        name = names.first;
        if (card != 0) {
            name = names.second;
        }
    } while (0);
    if (firstfile(name, &entry) == &entry) {
        base = card * 0x258;
        off = 0;
/* The loop is a goto; written as a do-while loop (or a for loop) this function differs from the original in 22 instruction slots. */
again:
        strcpy(base + (char *)data_800f80a4_slot0f + off + fine, (char *)&entry);
        off += 0x28;
        if (nextfile(&entry) == &entry) {
            goto again;
        }
    }
}
