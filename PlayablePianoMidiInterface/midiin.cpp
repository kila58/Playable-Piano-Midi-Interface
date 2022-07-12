#include "interface.h"

#include "rtmidi/RtMidi.h"

std::vector<std::vector<unsigned char>> messages;

void MidiCallback(double timeStamp, std::vector<unsigned char>* message, void* userData)
{
    if (message->empty())
        return;

    messages.push_back(std::vector<unsigned char>(*message));
}

void Interface::MidiIn()
{
    std::cout << "Interface::MidiIn\n";

    auto midi = new RtMidiIn();
    midi->setCallback(&MidiCallback);

    if (!midi->getPortCount())
        return;

    try
    {
        midi->openPort(0);
    }
    catch (RtMidiError& error)
    {
        std::cout << error.getMessage();
        return;
    }

    std::thread loop([midi] {
        while (true)
        {
            if (messages.empty())
                continue;

            for (auto message : messages)
            {
                auto command = message[0];
                auto note = message[1];
                auto velocity = message[2];

                if (keymap.find(note) == keymap.end())
                    continue;

                auto key = keymap[note];

                command = command & 0xF0;
                if (command == 0x90 && velocity > 0)
                {
                    if (key.shift)
                        KeyDown(0x10);

                    KeyDown(key.scancode);

                    if (key.shift)
                        KeyUp(0x10);
                }
                else if (command == 0x80 || (command == 0x90 && velocity == 0))
                    KeyUp(key.scancode);
            }

            messages.clear();

            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
    });

    loop.join();
}