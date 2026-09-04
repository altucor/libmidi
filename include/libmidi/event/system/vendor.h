#ifndef MIDI_SYSEX_VENDOR_ID
#define MIDI_SYSEX_VENDOR_ID

#include <stdint.h>
#include <assert.h>

// https://midi.org/sysexidtable

typedef enum _sysex_vendor_id_e
{
    /* American Group (00H - 1FH) */
    SYSEX_ID_EXTENSION = 0x00,
    SYSEX_ID_SEQUENTIAL_CIRCUITS,
    SYSEX_ID_IDP,
    SYSEX_ID_VOYETRA_TURTLE_BEACH,
    SYSEX_ID_MOOG,
    SYSEX_ID_PASSPORT_DESIGNS,
    SYSEX_ID_LEXICON,
    SYSEX_ID_KURZWEIL,
    SYSEX_ID_FENDER,
    SYSEX_ID_MIDI9,
    SYSEX_ID_AKG,
    SYSEX_ID_VOYCE_MUSIC,
    SYSEX_ID_WAVEFRAME,
    SYSEX_ID_ADA_SIGNAL,
    SYSEX_ID_GARFIELD,
    SYSEX_ID_ENSONIQ,
    SYSEX_ID_OBERHEIM,
    SYSEX_ID_APPLE,
    SYSEX_ID_GREY_MATTER,
    SYSEX_ID_DIGIDESIGN,
    SYSEX_ID_PALMTREE,
    SYSEX_ID_JLCOOPER,
    SYSEX_ID_LOWREY,
    SYSEX_ID_ADAMS_SMITH,
    SYSEX_ID_EMU,
    SYSEX_ID_HARMONY_SYSTEMS,
    SYSEX_ID_ART,
    SYSEX_ID_BALDWIN,
    SYSEX_ID_EVENTIDE,
    SYSEX_ID_INVENTRONICS,
    SYSEX_ID_KEY_CONCEPTS,
    SYSEX_ID_CLARITY,

    /* European Group (20H - 3FH) */
    SYSEX_ID_PASSAC = 0x20,
    SYSEX_ID_PROEL_LABS,
    SYSEX_ID_SYNTHAXE,
    SYSEX_ID_STEPP,
    SYSEX_ID_HOHNER,
    SYSEX_ID_TWISTER,
    SYSEX_ID_KETRON,
    SYSEX_ID_JELLINGHAUS,
    SYSEX_ID_SOUTHWORTH,
    SYSEX_ID_PPG,
    SYSEX_ID_CESYG,
    SYSEX_ID_SSL,
    SYSEX_ID_AUDIO_VERITRIEB,
    SYSEX_ID_NEVE,
    SYSEX_ID_SOUNDTRACS,
    SYSEX_ID_ELKA,
    SYSEX_ID_DYNACORD,
    SYSEX_ID_VISCOUNT,
    SYSEX_ID_DRAWMER,
    SYSEX_ID_CLAVIA,
    SYSEX_ID_AUDIO_ARCHITECTURE,
    SYSEX_ID_GENERALMUSIC,
    SYSEX_ID_CHEETAH,
    SYSEX_ID_CTM,
    SYSEX_ID_SIMMONS,
    SYSEX_ID_SOUNDCRAFT,
    SYSEX_ID_STEINBERG,
    SYSEX_ID_WERSI,
    SYSEX_ID_AVAB,
    SYSEX_ID_DIGIGRAM,
    SYSEX_ID_WALDORF,
    SYSEX_ID_QUASIMIDI,

    /* Japanese Group (40H - 5FH) */
    SYSEX_ID_KAWAI = 0x40,
    SYSEX_ID_ROLAND,
    SYSEX_ID_KORG,
    SYSEX_ID_YAMAHA,
    SYSEX_ID_CASIO,
    SYSEX_ID_RESERVED_45,
    SYSEX_ID_KAMIYA_STUDIO,
    SYSEX_ID_AKAI,
    SYSEX_ID_VICTOR,
    SYSEX_ID_RESERVED_49,
    SYSEX_ID_RESERVED_4A,
    SYSEX_ID_FUJITSU,
    SYSEX_ID_SONY,
    SYSEX_ID_RESERVED_4D,
    SYSEX_ID_TEAC,
    SYSEX_ID_RESERVED_4F,
    SYSEX_ID_MATSUSHITA,
    SYSEX_ID_FOSTEX,
    SYSEX_ID_ZOOM,
    SYSEX_ID_RESERVED_53,
    SYSEX_ID_MATSUSHITA_COMM,
    SYSEX_ID_SUZUKI,
    SYSEX_ID_FUJI_SOUND,
    SYSEX_ID_ACOUSTIC_TECH,
    SYSEX_ID_RESERVED_58,
    SYSEX_ID_FAITH,
    SYSEX_ID_INTERNET_CORP,
    SYSEX_ID_RESERVED_5B,
    SYSEX_ID_SEEKERS,
    SYSEX_ID_RESERVED_5D,
    SYSEX_ID_RESERVED_5E,
    SYSEX_ID_SD_CARD_ASSOCIATION,

    /* Special / Universal (7DH - 7FH) */
    SYSEX_ID_EDUCATIONAL = 0x7D,
    SYSEX_ID_UNIVERSAL_NON_REALTIME,
    SYSEX_ID_UNIVERSAL_REALTIME,
} sysex_vendor_id_e;

typedef union _midi_vendor_t
{
    // Raw data
    uint32_t raw;

    // Raw halfs
    uint16_t half[2];

    // Raw array
    uint8_t arr[4];

    struct
    {
        uint8_t b1;
        uint8_t b2;
        uint8_t b3;
        uint8_t b4;
    };
} midi_vendor_t;

static_assert(sizeof(uint32_t) == sizeof(midi_vendor_t), "Invalid size of \"midi_vendor_t\"");

static inline int midi_vendor_unmarshal(midi_vendor_t* ctx, const uint8_t* data, const uint32_t size)
{
    uint32_t iterator = 0;

    if (!size)
    {
        return -1;
    }

    // First byte zero means 3 byte id
    // Otherwise if first byte non zero, ID is single byte
    if (data[0] != 0x00)
    {
        ctx->b4 = data[0];
        iterator = 1;
    }

    if (size < 3)
    {
        return -1;
    }

    // Skip byte at pos zero because it always zero
    // Example: 00H 20H 32H	Behringer GmbH
    ctx->half[1] = *(uint16_t*)&data[1];
    iterator = 3;

    return iterator;
}

static inline int midi_vendor_marshal(midi_vendor_t* ctx)
{
    //

    return -1;
}

#endif // MIDI_SYSEX_VENDOR_ID
