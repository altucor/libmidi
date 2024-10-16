#include <gtest/gtest.h>

#include "midi_input_device.h"

#include <array>
#include <iomanip>
#include <stack>
#include <utility>

static bool s_should_check = false;
static midi_cmd_t s_msg = {.status = MIDI_STATUS_NOTE_OFF, .channel = 0};
static uint8_t s_message_meta = 0;
static midi_event_t s_event = {0};

void my_error_handler(void *null_ctx, midi_error_e err)
{
    std::cout << " --- midi_error: " << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << ((int64_t)err) << std::dec;
}

void my_handle_event(void *null_ctx, midi_cmd_t msg, uint8_t message_meta, midi_event_t event)
{
    std::cout << " -- callback called" << std::endl;
    s_should_check = true;
    s_msg = msg;
    s_message_meta = message_meta;
    s_event = event;
}

TEST(midi_input_device, midi_input_device_full_check)
{
    using unique_midi_input_device_t = std::unique_ptr<midi_input_device_t, decltype(&midi_input_device_free)>;
    unique_midi_input_device_t input_device_ctx(midi_input_device_new(true), &midi_input_device_free);

    midi_device_callback_data_t cb = {0};
    cb.handle = 0;
    cb.event = (midi_cb_event_f *)&my_handle_event;
    cb.error = (midi_cb_error_f *)&my_error_handler;
    midi_input_device_set_listener(input_device_ctx.get(), &cb);

    EXPECT_EQ(&my_handle_event, input_device_ctx->listener->event);

    std::stack<midi_status_e> expectedStatuses;
    expectedStatuses.push(MIDI_STATUS_NOTE_OFF); // note off
    expectedStatuses.push(MIDI_STATUS_NOTE_ON);  // note on
    expectedStatuses.push(MIDI_STATUS_NOTE_OFF); // note off
    expectedStatuses.push(MIDI_STATUS_NOTE_ON);  // note on
    expectedStatuses.push(MIDI_STATUS_SYSTEM);   // track name
    expectedStatuses.push(MIDI_STATUS_SYSTEM);   // time signature
    expectedStatuses.push(MIDI_STATUS_SYSTEM);   // tempo

    std::stack<uint8_t> expectedSubCmd;
    expectedSubCmd.push(0);
    expectedSubCmd.push(0);
    expectedSubCmd.push(0);
    expectedSubCmd.push(0);
    expectedSubCmd.push(MIDI_STATUS_SYSTEM_RESET_OR_META);
    expectedSubCmd.push(MIDI_STATUS_SYSTEM_RESET_OR_META);
    expectedSubCmd.push(MIDI_STATUS_SYSTEM_RESET_OR_META);

    std::stack<uint8_t> expectedMessagesMeta;
    expectedMessagesMeta.push(MIDI_META_EVENT_SEQUENCE_NUMBER);
    expectedMessagesMeta.push(MIDI_META_EVENT_SEQUENCE_NUMBER);
    expectedMessagesMeta.push(MIDI_META_EVENT_SEQUENCE_NUMBER);
    expectedMessagesMeta.push(MIDI_META_EVENT_SEQUENCE_NUMBER);
    expectedMessagesMeta.push(MIDI_META_EVENT_TRACK_NAME);
    expectedMessagesMeta.push(MIDI_META_EVENT_TIME_SIGNATURE);
    expectedMessagesMeta.push(MIDI_META_EVENT_TEMPO);

    midi_event_t tempEventFiller = {0};
    std::stack<midi_event_t> expectedEvents;

    tempEventFiller.note.on = false;
    tempEventFiller.note.channel = 0;
    tempEventFiller.note.pitch = 56;
    tempEventFiller.note.velocity = 64;
    expectedEvents.push(tempEventFiller);

    tempEventFiller.note.on = true;
    tempEventFiller.note.channel = 0;
    tempEventFiller.note.pitch = 56;
    tempEventFiller.note.velocity = 100;
    expectedEvents.push(tempEventFiller);

    tempEventFiller.note.on = false;
    tempEventFiller.note.channel = 0;
    tempEventFiller.note.pitch = 60;
    tempEventFiller.note.velocity = 64;
    expectedEvents.push(tempEventFiller);

    tempEventFiller.note.on = true;
    tempEventFiller.note.channel = 0;
    tempEventFiller.note.pitch = 60;
    tempEventFiller.note.velocity = 100;
    expectedEvents.push(tempEventFiller);

    const char *trackName = "3xOsc 1";
    tempEventFiller.meta.text.val = (char *)trackName;
    expectedEvents.push(tempEventFiller);

    tempEventFiller.meta.time_signature.numerator = 4;
    tempEventFiller.meta.time_signature.denominator = 2;
    tempEventFiller.meta.time_signature.ticks_per_click = 24;
    tempEventFiller.meta.time_signature.thirty_second_notes_per_crotchet = 8;
    expectedEvents.push(tempEventFiller);

    tempEventFiller.meta.tempo.val = 120;
    expectedEvents.push(tempEventFiller);

    constexpr std::array<uint8_t, 42> data = {0x00, 0xFF, 0x51, 0x03, 0x07, 0x90, 0xFC, 0x00, 0xFF, 0x58, 0x04, 0x04, 0x02, 0x18,
                                              0x08, 0x00, 0xFF, 0x03, 0x07, 0x33, 0x78, 0x4F, 0x73, 0x63, 0x20, 0x31, 0x00, 0x90,
                                              0x3C, 0x64, 0x60, 0x80, 0x3C, 0x40, 0x00, 0x90, 0x38, 0x64, 0x30, 0x80, 0x38, 0x40};

    sizeof(midi_cmd_t);

    std::for_each(data.begin(), data.end(), [&](const uint8_t &b) {
        std::cout << "feeding = 0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << ((int64_t)b) << std::dec;
        midi_input_device_feed(input_device_ctx.get(), b);
        std::cout << " s_msg.status = " << std::to_string(s_msg.status) << std::endl;
        if (s_should_check)
        {
            s_should_check = false;
            EXPECT_FALSE(expectedStatuses.empty());
            EXPECT_EQ(s_msg.status, expectedStatuses.top());
            expectedStatuses.pop();

            // EXPECT_FALSE(expectedSubCmd.empty());
            // EXPECT_EQ(s_msg.system, expectedSubCmd.top());
            // expectedSubCmd.pop();

            // EXPECT_FALSE(expectedMessagesMeta.empty());
            // uint8_t meta_msg = expectedMessagesMeta.top();
            // EXPECT_EQ(s_message_meta, meta_msg);
            // expectedMessagesMeta.pop();

            // EXPECT_FALSE(expectedEvents.empty());
            // midi_event_t outEvent = {0};
            // outEvent = expectedEvents.top();
            // expectedEvents.pop();
            // switch (meta_msg)
            // {
            // case MIDI_META_EVENT_TEMPO:
            //     EXPECT_EQ(s_event.meta.tempo.val, outEvent.meta.tempo.val);
            //     break;

            // case MIDI_META_EVENT_TIME_SIGNATURE:
            //     EXPECT_EQ(s_event.meta.time_signature.numerator, outEvent.meta.time_signature.numerator);
            //     EXPECT_EQ(s_event.meta.time_signature.denominator, outEvent.meta.time_signature.denominator);
            //     EXPECT_EQ(s_event.meta.time_signature.ticks_per_click, outEvent.meta.time_signature.ticks_per_click);
            //     EXPECT_EQ(s_event.meta.time_signature.thirty_second_notes_per_crotchet, outEvent.meta.time_signature.thirty_second_notes_per_crotchet);
            //     break;

            // case MIDI_META_EVENT_TRACK_NAME:
            //     EXPECT_STREQ(s_event.meta.text.val, outEvent.meta.text.val);
            //     break;

            // case MIDI_META_EVENT_SEQUENCE_NUMBER:
            //     EXPECT_EQ(s_event.note.on, outEvent.note.on);
            //     EXPECT_EQ(s_event.note.channel, outEvent.note.channel);
            //     EXPECT_EQ(s_event.note.pitch, outEvent.note.pitch);
            //     EXPECT_EQ(s_event.note.velocity, outEvent.note.velocity);
            //     break;

            // default:
            //     break;
            // }
        }
    });
}