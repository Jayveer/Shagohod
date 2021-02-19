#include "cli.h"

CLI::CLI(int argc, char** argv) {
	this->argc = argc;
	this->argv = argv;
}

CLI::~CLI() {

}

bool CLI::isStage(const std::string& filepath) {
	if (filenameContainsString(filepath, "STAGE") || filenameContainsString(filepath, "stage"))
		return true;

	return false;
}

bool CLI::isSlot(const std::string& filepath) {
	if (filenameContainsString(filepath, "SLOT") || filenameContainsString(filepath, "slot"))
		return true;

	return false;
}

bool CLI::isDat(const std::string& filepath) {
	if (getExtension(filepath) == ".DAT" || getExtension(filepath) == ".dat")
		return true;

	printf("Unsupported file type\n");
	exit();
	return false;
}

void CLI::extractArchive(Archive& archive) {
	std::string output = "";
	if (currentArg == argc - 1) output = argv[currentArg];

	archive.open();
	archive.extractAll(output);
}

void CLI::processCommands() {
	while (currentArg < 2 && isCommand(argv[currentArg])) {
		setCommand(argv[currentArg]);
		currentArg++;
	}
}

void CLI::setCommand(char* arg) {

	if (!strcmp(arg, "-SUBSISTENCE") || !strcmp(arg, "-sub") || !strcmp(arg, "-SUB") || !strcmp(arg, "-subsistence")) {
		GAME = SUBSISTENCE;
		return;
	}

	printf("command not recognised\n");
}

void CLI::processArgs() {
	processCommands();
	processFile();
}

void CLI::processFile() {
	std::string input = argv[currentArg];	
	currentArg++;	

	if (!isDat(input)) return;

	if (isSlot(input)) {
		Slot slot = Slot(input);
		extractArchive(slot);
		return;
	}

	if (isStage(input)) {
		Stage stage = Stage(input);
		extractArchive(stage);
		return;
	}

	printf("Unsupported Archive\n");
	exit();
}

bool CLI::checkInput() {
	if (argc > 1 && argc < 5) return true;
	printUsage();
	return false;
}

void CLI::run(std::string programName, std::string version) {
	printf("Running %s v%s: Visit https://github.com/Jayveer/Shagohod for updates:\n", programName.c_str(), version.c_str());
	loadDictionary("dictionary.txt");
	if (!checkInput()) return;
	processArgs();
}

bool CLI::isCommand(char* arg) {
	return arg[0] == 0x2D;
}

void CLI::printUsage() {
	printf(this->USAGE_MESSAGE);
}

void CLI::exit() {
	printf(this->EXIT_MESSAGE);
}