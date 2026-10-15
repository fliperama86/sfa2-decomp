/* PS1 opening soundtrack adaptation. This is not a native QSound driver. */
#ifndef PORT_INTRO_AUDIO_H
#define PORT_INTRO_AUDIO_H
#include <stddef.h>
#include <stdint.h>
#define INTRO_AUDIO_SECTORS 956u
#define INTRO_AUDIO_RAW_SIZE (INTRO_AUDIO_SECTORS*2352u)
struct intro_audio {
    int16_t *samples;
    size_t frames,position;
    unsigned gain; /* 0..65536, host's exit fade. */
};
/* Decode the selected movie's contiguous gathered XA sectors, already verified
 * against the pinned input digest by the host. Caller starts with zero state.
 * Failure leaves the previous object unchanged. */
int intro_audio_decode(struct intro_audio *,const uint8_t *,size_t);
void intro_audio_free(struct intro_audio *);
size_t intro_audio_pull(struct intro_audio *,int16_t *,size_t);
#endif
