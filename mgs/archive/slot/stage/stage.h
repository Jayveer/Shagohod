#pragma once
#include "../slot.h"

#pragma pack(push, 4)
struct StageTable {
	std::string name;
	int32_t offset;
};

struct StageTableS {
	char name[16];
	int32_t offset;
};

#pragma pop

class Stage : public Slot {
public:
	Stage(std::string filename);
	Stage(std::string filename, uint32_t sector);
	~Stage();

	void open();
	void extractAll(std::string output = "");
	void extract(uint16_t pageID, std::string output = "");
private:
	std::vector<StageTable> table;
	void populateTable(uint8_t* data);
	int64_t getNextPageOffset(uint16_t pageID);
};