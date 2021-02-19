#include "stage.h"

Stage::Stage(std::string filename) : Slot(filename) {}

Stage::Stage(std::string filename, uint32_t sector) : Slot(filename, sector) {}

Stage::~Stage() {}

int64_t Stage::getNextPageOffset(uint16_t pageID) {
	return (pageID == header.numPages - 1) ? getFileSize(filename) : table[pageID + 1].offset * sector;
}

void Stage::populateTable(uint8_t* data) {
	int stride = GAME == SUBSISTENCE ? 0x14 : 0x0C;

	for (int i = 0; i < header.numPages; i++) {		
		int pos = i * stride;
		int s = 0; 
		while (data[pos + s] != '\0') { s++; }
		table[i].name.reserve(s);
		table[i].name.assign((char*)&data[pos], s);
		pos = pos + (stride - 4);
		table[i].offset = *(uint32_t*)&data[pos];
	}
}

void Stage::open() {
	std::ifstream stageDat;
	stageDat.open(filename, std::ios::binary);

	int size = sizeof(SlotHeader);
	stageDat.read((char*)&this->header, size);
	uint32_t keyA = header.timestamp;
	uint32_t keyB = keyA ^ 0xF0F0;
	keyA = decryptor.decodeBuffer(keyA, keyB, 0, 0x0C, (uint8_t*)&header.version);

	int stride = GAME == SUBSISTENCE ? 0x14 : 0x0C;
	size = stride * header.numPages;
	table.resize(header.numPages);

	uint8_t* tableData = new uint8_t[size];	
	stageDat.read((char*)tableData, size);
	keyA = decryptor.decodeBuffer(keyA, keyB, 0, size, tableData);

	populateTable(tableData);
	delete[] tableData;
}

void Stage::extract(uint16_t pageID, std::string output) {
	if (pageID > header.numPages) return;

	updateDir("stage", output);
	updateDir(table[pageID].name, output);

	int64_t nextOffset = getNextPageOffset(pageID);
	int64_t offset = table[pageID].offset * sector;

	int64_t size = nextOffset - offset;
	extractFiles(size, offset, output, true);
}

void Stage::extractAll(std::string output) {
	for (int i = 0; i < header.numPages; i++) {
		extract(i, output);
	}
}