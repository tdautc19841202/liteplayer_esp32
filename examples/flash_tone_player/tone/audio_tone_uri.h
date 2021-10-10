#ifndef __AUDIO_TONEURI_H__
#define __AUDIO_TONEURI_H__

extern const char* tone_uri[];

typedef enum {
    TONE_TYPE_DINGDONG,
    TONE_TYPE_NETWORK_CONNECTED,
    TONE_TYPE_SYSTEM_READY,
    TONE_TYPE_WAKEWORD,
    TONE_TYPE_MAX,
} tone_type_t;

int get_tone_uri_num();

#endif
