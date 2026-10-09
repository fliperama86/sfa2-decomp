/* Memory cards: none in this port yet. The game is told what a console with empty slots tells it.
 *
 * The request calls (_card_info, _card_load, _card_clear) are accepted: they return 1, what PSY-Q's
 * return for an accepted request, and the answer comes at once as the event PSY-Q documents for "no
 * card": the software card event (class 0xf4000001) with spec EvSpTIMOUT (0x0100), delivered to
 * the event the game opened for it, so that its next TestEvent sees it. The game's own polling code
 * (slot0f, func_800e0da8 / func_800e1074) tests the four software events in the order IOE (0x4),
 * ERROR (0x8000), TIMOUT (0x100), NEW (0x2000) and takes the index of the first that is ready: a
 * time-out ends in its "no card" result after 20 tries, which is the sequence this answers.
 * The file routines (open, read, write, close, format, firstfile, nextfile) answer as with no card: the
 * error result PSY-Q documents (-1 for the descriptor calls, 0 for the file list and the format), nothing
 * stored. InitCARD, StartCARD and _bu_init (the game's start-up) are accepted. */
#include "port.h"

#define SW_CARD 0xf4000001u
#define EV_SP_TIMOUT 0x0100u

/* _card_info(chan) (A0 0xab in the listing at 0x8016d890), _card_load(chan) (A0 0xac, 0x8016d880): 1, accepted;
 * no card answers with the time-out event. */
long port_h_card_request(long chan);
long port_h_card_request(long chan)
{
    (void)chan;
    port_deliver_event(SW_CARD, EV_SP_TIMOUT);
    return 1;
}

/* _card_clear(chan): the game's library function at 0x8016d8a0 is C that calls B0 0x50 (_new_card) and
 * B0 0x4e (_card_write, chan, 0x3f); the same request, answered the same. */
long port_h_card_clear(long chan);
long port_h_card_clear(long chan)
{
    return port_h_card_request(chan);
}

/* InitCARD, _bu_init: void; the game ignores them. */
void port_h_card_ok(void);
void port_h_card_ok(void)
{
}

/* StartCARD (B0 0x4b): 1. */
int port_h_StartCARD(void);
int port_h_StartCARD(void)
{
    return 1;
}

/* open, read, write, close (B0 0x32, 0x34, 0x35, 0x36): -1, the error result with no card. */
int port_h_card_fail(void);
int port_h_card_fail(void)
{
    return -1;
}

/* firstfile, nextfile (B0 0x42, 0x43), format (B0 0x41): 0, no file found / not formatted. */
int port_h_card_none(void);
int port_h_card_none(void)
{
    return 0;
}

#define NOTE "no memory card in this port yet"

const struct port_library port_card_library[] = {
    { "InitCARD", (void *)port_h_card_ok, NOTE },
    { "StartCARD", (void *)port_h_StartCARD, NOTE },
    { "_bu_init", (void *)port_h_card_ok, NOTE },
    { "_card_info", (void *)port_h_card_request, NOTE },
    { "_card_load", (void *)port_h_card_request, NOTE },
    { "@0x8016d8a0", (void *)port_h_card_clear, NOTE },
    { "open", (void *)port_h_card_fail, NOTE },
    { "read", (void *)port_h_card_fail, NOTE },
    { "write", (void *)port_h_card_fail, NOTE },
    { "close", (void *)port_h_card_fail, NOTE },
    { "firstfile", (void *)port_h_card_none, NOTE },
    { "nextfile", (void *)port_h_card_none, NOTE },
    { "format", (void *)port_h_card_none, NOTE },
    { NULL, NULL, NULL }
};
