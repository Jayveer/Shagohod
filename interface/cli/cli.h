#pragma once
#include "../../mgs/archive/slot/slot.h"
#include "../../mgs/archive/slot/stage/stage.h"

class CLI {
public:
	CLI(int argc, char** argv);
	~CLI();

	void run(std::string programName, std::string version);
	void exit();
private:
	int argc;
	char** argv;
	int currentArg = 1;

	void printUsage();
	bool checkInput();
	void processArgs();
	void extractArchive(Archive& archive);

	bool isDat(const std::string& filepath);
	bool isSlot(const std::string& filepath);
	bool isStage(const std::string& filepath);


	const char* EXIT_MESSAGE = "Exiting\n";
	const char* USAGE_MESSAGE = "Usage:\t Shagohod.exe <SLOT.DAT | STAGE.DAT> [OUTPUTDIRECTORY] \n";
};