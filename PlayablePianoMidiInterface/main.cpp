#include "interface.h"

int main(int argc, char** argv)
{
	Options options;
	options.process(argc, argv);

	if (options.getArgCount() > 0)
		Interface::MidiParse(argc, argv);
	else
		Interface::MidiIn();

	system("pause");
}