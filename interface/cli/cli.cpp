#include "cli.h"

CLI::CLI(int argc, char** argv) {
	this->argc = argc;
	this->argv = argv;
}

CLI::~CLI() {

}

bool CLI::isStage(const std::string& filepath) {
	return filenameContainsString(filepath, "STAGE");
}

bool CLI::isSlot(const std::string& filepath) {
	return filenameContainsString(filepath, "SLOT");
}

bool CLI::isDat(const std::string& filepath) {
	if (getExtension(filepath) == ".DAT")  return true;
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

void CLI::processArgs() {
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
	if (argc > 1 && argc < 4) return true;
	printUsage();
	return false;
}

void CLI::run(std::string programName, std::string version) {
	printf("Running %s v%s: Visit https://github.com/Jayveer/Shagohod for updates:\n", programName.c_str(), version.c_str());
	loadDictionary("dictionary.txt");
	if (!checkInput()) return;
	processArgs();
}

void CLI::printUsage() {
	printf(this->USAGE_MESSAGE);
}

void CLI::exit() {
	printf(this->EXIT_MESSAGE);
}