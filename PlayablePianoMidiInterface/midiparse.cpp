#include "interface.h"

#include "midifile/include/MidiFile.h"

#include <mutex> 

std::vector<std::thread> tracks;

float timescale = 1.13; //magic number lol

void Interface::MidiParse(int argc, char** argv)
{
	std::cout << "Interface::MidiParse" << std::endl;

	Options options;
	options.process(argc, argv);

	MidiFile midifile;
	midifile.read(options.getArg(1));

	std::cout << options.getArg(1) << std::endl;

	midifile.linkNotePairs();
	midifile.doTimeAnalysis();
	midifile.joinTracks();
	midifile.absoluteTicks();
	
	bool begin = false;

	for (int t = 0; t < midifile.getTrackCount(); t++)
	{
		//procedural track threads for multi-track midi file(non-joined) support
		//not ideal, better to queue up each event for each track and call once per tick on a single thread for audio sync
		std::thread track = std::thread([&, t]() {
			static int curtick = -1;
			static double lasttick = 0.0f;

			while (!begin)
				std::this_thread::sleep_for(std::chrono::milliseconds(15));

			for (int e = 0; e < midifile[t].size(); e++)
			{
				auto& event = midifile[t][e];

				if (curtick < 0)
					curtick = event.tick;

				if (event.tick != curtick)
				{
					int delta = (int)(((midifile.getTimeInSeconds(event.tick) - lasttick) / timescale) * 1000);
					std::this_thread::sleep_for(std::chrono::milliseconds(delta));

					curtick = event.tick;
				}

				lasttick = midifile.getTimeInSeconds(event.tick);

				if (keymap.find(event.getKeyNumber()) == keymap.end())
					continue;

				auto key = keymap[event.getKeyNumber()];

				if (event.isNoteOn() && event.getVelocity() > 0)
				{
					if (key.shift)
						KeyDown(0x10);

					KeyDown(key.scancode);

					if (key.shift)
						KeyUp(0x10);
				}

				if (event.isNoteOff())
					KeyUp(key.scancode);
			}
		});

		tracks.push_back(std::move(track));
	}

	for (auto& track : tracks)
		track.detach();

	begin = true;

	std::this_thread::sleep_for(std::chrono::seconds((int)midifile.getTotalTimeInSeconds() + 1)); //keep main thread alive for song duration
}