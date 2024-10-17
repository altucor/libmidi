#include <gtest/gtest.h>

#include "mtrk.h"

#include <array>

TEST(mtrk, mtrk_full_check)
{
    // Create midi_input_device_t
    using unique_midi_input_device_t = std::unique_ptr<midi_input_device_t, decltype(&midi_input_device_free)>;
    unique_midi_input_device_t input_device_ctx(midi_input_device_new(true), &midi_input_device_free);

    // Create mtrk_t
    using unique_mtrk_t = std::unique_ptr<mtrk_t, decltype(&mtrk_free)>;
    unique_mtrk_t mtrk_ctx(mtrk_new(input_device_ctx.get()), &mtrk_free);

    constexpr std::array<uint8_t, 79> data = {0x4D, 0x54, 0x72, 0x6B, 0x00, 0x00, 0x00, 0x47, 0x00, 0xFF, 0x51, 0x03, 0x07, 0x90, 0xFC, 0x00,
                                              0xFF, 0x58, 0x04, 0x04, 0x02, 0x18, 0x08, 0x00, 0xFF, 0x03, 0x07, 0x33, 0x78, 0x4F, 0x73, 0x63,
                                              0x20, 0x31, 0x00, 0x90, 0x3C, 0x64, 0x60, 0x80, 0x3C, 0x40, 0x00, 0x90, 0x38, 0x64, 0x30, 0x80,
                                              0x38, 0x40, 0x00, 0x90, 0x3C, 0x64, 0x30, 0x80, 0x3C, 0x40, 0x00, 0x90, 0x37, 0x64, 0x81, 0x70,
                                              0x80, 0x37, 0x40, 0x00, 0x90, 0x38, 0x64, 0x30, 0x80, 0x38, 0x40, 0x40, 0xFF, 0x2F, 0x00};

    EXPECT_EQ(mtrk_unmarshal(mtrk_ctx.get(), data.data(), data.size()), data.size());

    EXPECT_TRUE(0 == std::memcmp(mtrk_ctx->mtrk, mtrk_header_reference, MTRK_MARKER_SIZE));
    EXPECT_EQ(mtrk_ctx->size, 0x47);
    const uint32_t events_count = mtrk_get_events_count(mtrk_ctx.get());
    EXPECT_EQ(events_count, 14);

    midi_event_smf_t *event = nullptr;
    uint32_t event_iter = 0;

    event = mtrk_get_event(mtrk_ctx.get(), event_iter++);
    EXPECT_TRUE(event->message.new_msg);
    EXPECT_EQ(event->message.status, MIDI_STATUS_SYSTEM);
    EXPECT_EQ(event->message.system, MIDI_STATUS_SYSTEM_RESET_OR_META);
    EXPECT_EQ(event->message_meta, MIDI_META_EVENT_TEMPO);
    EXPECT_EQ(event->predelay, 0);
    EXPECT_EQ(event->event.meta.tempo.val, 120);

    event = mtrk_get_event(mtrk_ctx.get(), event_iter++);
    EXPECT_TRUE(event->message.new_msg);
    EXPECT_EQ(event->message.status, MIDI_STATUS_SYSTEM);
    EXPECT_EQ(event->message.system, MIDI_STATUS_SYSTEM_RESET_OR_META);
    EXPECT_EQ(event->message_meta, MIDI_META_EVENT_TIME_SIGNATURE);
    EXPECT_EQ(event->predelay, 0);
    EXPECT_EQ(event->event.meta.time_signature.numerator, 4);
    EXPECT_EQ(event->event.meta.time_signature.denominator, 2);
    EXPECT_EQ(event->event.meta.time_signature.ticks_per_click, 24);
    EXPECT_EQ(event->event.meta.time_signature.thirty_second_notes_per_crotchet, 8);

    event = mtrk_get_event(mtrk_ctx.get(), event_iter++);
    EXPECT_TRUE(event->message.new_msg);
    EXPECT_EQ(event->message.status, MIDI_STATUS_SYSTEM);
    EXPECT_EQ(event->message.system, MIDI_STATUS_SYSTEM_RESET_OR_META);
    EXPECT_EQ(event->message_meta, MIDI_META_EVENT_TRACK_NAME);
    EXPECT_EQ(event->predelay, 0);
    EXPECT_STREQ(event->event.meta.text.val, "3xOsc 1");

    event = mtrk_get_event(mtrk_ctx.get(), event_iter++);
    EXPECT_TRUE(event->message.new_msg);
    EXPECT_EQ(event->message.status, MIDI_STATUS_NOTE_ON);
    EXPECT_EQ(event->message.channel, 0);
    EXPECT_EQ(event->message_meta, 0);
    EXPECT_EQ(event->predelay, 0);
    EXPECT_TRUE(event->event.note.on);
    EXPECT_EQ(event->event.note.channel, 0);
    EXPECT_EQ(event->event.note.pitch, 60);
    EXPECT_EQ(event->event.note.velocity, 100);

    event = mtrk_get_event(mtrk_ctx.get(), event_iter++);
    EXPECT_TRUE(event->message.new_msg);
    EXPECT_EQ(event->message.status, MIDI_STATUS_NOTE_OFF);
    EXPECT_EQ(event->message.channel, 0);
    EXPECT_EQ(event->message_meta, 0);
    EXPECT_EQ(event->predelay, 96);
    EXPECT_FALSE(event->event.note.on);
    EXPECT_EQ(event->event.note.channel, 0);
    EXPECT_EQ(event->event.note.pitch, 60);
    EXPECT_EQ(event->event.note.velocity, 64);

    event = mtrk_get_event(mtrk_ctx.get(), event_iter++);
    EXPECT_TRUE(event->message.new_msg);
    EXPECT_EQ(event->message.status, MIDI_STATUS_NOTE_ON);
    EXPECT_EQ(event->message.channel, 0);
    EXPECT_EQ(event->message_meta, 0);
    EXPECT_EQ(event->predelay, 0);
    EXPECT_TRUE(event->event.note.on);
    EXPECT_EQ(event->event.note.channel, 0);
    EXPECT_EQ(event->event.note.pitch, 56);
    EXPECT_EQ(event->event.note.velocity, 100);

    event = mtrk_get_event(mtrk_ctx.get(), event_iter++);
    EXPECT_TRUE(event->message.new_msg);
    EXPECT_EQ(event->message.status, MIDI_STATUS_NOTE_OFF);
    EXPECT_EQ(event->message.channel, 0);
    EXPECT_EQ(event->message_meta, 0);
    EXPECT_EQ(event->predelay, 48);
    EXPECT_FALSE(event->event.note.on);
    EXPECT_EQ(event->event.note.channel, 0);
    EXPECT_EQ(event->event.note.pitch, 56);
    EXPECT_EQ(event->event.note.velocity, 64);

    event = mtrk_get_event(mtrk_ctx.get(), event_iter++);
    EXPECT_TRUE(event->message.new_msg);
    EXPECT_EQ(event->message.status, MIDI_STATUS_NOTE_ON);
    EXPECT_EQ(event->message.channel, 0);
    EXPECT_EQ(event->message_meta, 0);
    EXPECT_EQ(event->predelay, 0);
    EXPECT_TRUE(event->event.note.on);
    EXPECT_EQ(event->event.note.channel, 0);
    EXPECT_EQ(event->event.note.pitch, 60);
    EXPECT_EQ(event->event.note.velocity, 100);

    event = mtrk_get_event(mtrk_ctx.get(), event_iter++);
    EXPECT_TRUE(event->message.new_msg);
    EXPECT_EQ(event->message.status, MIDI_STATUS_NOTE_OFF);
    EXPECT_EQ(event->message.channel, 0);
    EXPECT_EQ(event->message_meta, 0);
    EXPECT_EQ(event->predelay, 48);
    EXPECT_FALSE(event->event.note.on);
    EXPECT_EQ(event->event.note.channel, 0);
    EXPECT_EQ(event->event.note.pitch, 60);
    EXPECT_EQ(event->event.note.velocity, 64);

    event = mtrk_get_event(mtrk_ctx.get(), event_iter++);
    EXPECT_TRUE(event->message.new_msg);
    EXPECT_EQ(event->message.status, MIDI_STATUS_NOTE_ON);
    EXPECT_EQ(event->message.channel, 0);
    EXPECT_EQ(event->message_meta, 0);
    EXPECT_EQ(event->predelay, 0);
    EXPECT_TRUE(event->event.note.on);
    EXPECT_EQ(event->event.note.channel, 0);
    EXPECT_EQ(event->event.note.pitch, 55);
    EXPECT_EQ(event->event.note.velocity, 100);

    event = mtrk_get_event(mtrk_ctx.get(), event_iter++);
    EXPECT_TRUE(event->message.new_msg);
    EXPECT_EQ(event->message.status, MIDI_STATUS_NOTE_OFF);
    EXPECT_EQ(event->message.channel, 0);
    EXPECT_EQ(event->message_meta, 0);
    EXPECT_EQ(event->predelay, 240);
    EXPECT_FALSE(event->event.note.on);
    EXPECT_EQ(event->event.note.channel, 0);
    EXPECT_EQ(event->event.note.pitch, 55);
    EXPECT_EQ(event->event.note.velocity, 64);

    event = mtrk_get_event(mtrk_ctx.get(), event_iter++);
    EXPECT_TRUE(event->message.new_msg);
    EXPECT_EQ(event->message.status, MIDI_STATUS_NOTE_ON);
    EXPECT_EQ(event->message.channel, 0);
    EXPECT_EQ(event->message_meta, 0);
    EXPECT_EQ(event->predelay, 0);
    EXPECT_TRUE(event->event.note.on);
    EXPECT_EQ(event->event.note.channel, 0);
    EXPECT_EQ(event->event.note.pitch, 56);
    EXPECT_EQ(event->event.note.velocity, 100);

    event = mtrk_get_event(mtrk_ctx.get(), event_iter++);
    EXPECT_TRUE(event->message.new_msg);
    EXPECT_EQ(event->message.status, MIDI_STATUS_NOTE_OFF);
    EXPECT_EQ(event->message.channel, 0);
    EXPECT_EQ(event->message_meta, 0);
    EXPECT_EQ(event->predelay, 48);
    EXPECT_FALSE(event->event.note.on);
    EXPECT_EQ(event->event.note.channel, 0);
    EXPECT_EQ(event->event.note.pitch, 56);
    EXPECT_EQ(event->event.note.velocity, 64);

    event = mtrk_get_event(mtrk_ctx.get(), event_iter++);
    EXPECT_TRUE(event->message.new_msg);
    EXPECT_EQ(event->message.status, MIDI_STATUS_SYSTEM);
    EXPECT_EQ(event->message.system, MIDI_STATUS_SYSTEM_RESET_OR_META);
    EXPECT_EQ(event->message_meta, MIDI_META_EVENT_TRACK_END);
    EXPECT_EQ(event->predelay, 64);
}