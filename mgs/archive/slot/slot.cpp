#include "slot.h"

Slot::Slot(std::string filename) {
	this->filename = filename;
}

Slot::Slot(std::string filename, uint32_t sector) {
	this->filename = filename;
	this->sector = sector;
}

Slot::~Slot() {}

std::string Slot::getFilename() {
	return this->filename;
}

std::string Slot::getCreatedDate() {
	std::string createdDate;
	createdDate.resize(20);
	time_t rawtime = (time_t)this->header.timestamp;
	tm ptm;
	gmtime_s(&ptm, &rawtime);

	sprintf_s(&createdDate[0],
		20,
		"%02d/%02d/%04d %02d:%02d:%02d",
		ptm.tm_mday,
		ptm.tm_mon,
		(1900 + ptm.tm_year),
		ptm.tm_hour,
		ptm.tm_min,
		ptm.tm_sec);

	return createdDate;
}

void Slot::open() {
	std::ifstream slotDat;
	slotDat.open(filename, std::ios::binary);
	slotDat.read((char*)&this->header, sizeof(SlotHeader));
	initPageInfo(slotDat);
	slotDat.close();
}

void Slot::initPageInfo(std::ifstream& slotDat) {
	pageInfo.resize(header.numPages);
	slotDat.seekg(sector);

	for (int i = 0; i < header.numPages; i++) {
		pageInfo[i].offset = slotDat.tellg();
		pageInfo[i].size = getMaxPageSize(slotDat);
	}

}

uint8_t* Slot::decompressPage(uint8_t* compressedPage, int64_t compressedSize, int64_t decompressedSize) {
	int size;
	ZlibWrapper zlib;
	uint8_t* decompressedPage = new uint8_t[decompressedSize];

	zlib.zlibInflate((char*)(compressedPage), (char*)decompressedPage, compressedSize, decompressedSize, &size, 1);

	return decompressedPage;
}

void Slot::decryptPage(uint8_t* src, int64_t size) {
	uint16_t key = *(uint16_t*)src;

	decryptor.decodePage(key, 0, size, src);
	src[0] = 0x78; src[1] = 0x9C;
}

void Slot::decryptPageCNF(const std::string& stageName, uint8_t* src) {
	uint32_t keyA = strcode(stageName.c_str());
	uint32_t keyB = header.timestamp;

	int32_t numTags = *(uint32_t*)src;
	decryptor.decodeCNF(keyA, keyB, 0, 4, (uint8_t*)&numTags);

	int64_t cnfHeaderSize = sizeof(DataCNF) + sizeof(DataCNFTag) * numTags;
	decryptor.decodeCNF(keyA, keyB, 0, cnfHeaderSize, src);
}

void Slot::extractFiles(int64_t size, int64_t offset, std::string& output, bool isEncrypted) {
	std::ifstream fileDat;
	fileDat.open(filename, std::ios::binary);
	fileDat.seekg(offset);

	uint8_t* pageData = new uint8_t[size];
	fileDat.read((char*)pageData, size);
	fileDat.close();

	if (isEncrypted) decryptPageCNF(getCurrentDir(output), pageData);

	DataConfig cnf = DataConfig(pageData, size);
	cnf.setHandler(this);
	cnf.setWorkDir(output);
	cnf.read(BINARY);

	delete[] pageData;
}

void Slot::extract(uint16_t pageID, std::string output) {
	if (pageID > header.numPages) return;

	updateDir(getExtensionlessName(filename), output);
	updateDir(std::to_string(pageID), output);

	int64_t size = pageInfo[pageID].size;
	int64_t offset = pageInfo[pageID].offset;

	extractFiles(size, offset, output);
}

void Slot::extractAll(std::string output) {
	for (int i = 0; i < header.numPages; i++) {
		extract(i, output);
	}
}

int64_t Slot::getMaxPageSize(std::ifstream& slotDat) {
	int64_t start = slotDat.tellg();
	int32_t numTags;
	slotDat.read((char*)&numTags, 4);

	int64_t size = sizeof(DataCNF) + sizeof(DataCNFTag) * numTags;
	size += getNextSectorOffset(size);

	for (int i = 0; i < numTags; i++) {
		uint32_t id;
		slotDat.read((char*)&id, 4);

		int32_t offset;
		slotDat.read((char*)&offset, 4);

		uint32_t fileext = id >> 24;
		uint32_t filename = id & 0xFFFFFF;

		if (fileext == 0x7F && filename) size += (offset + getNextSectorOffset(offset));
	}

	slotDat.seekg(start + size, slotDat.beg);
	return size;
}

int64_t Slot::getMaxPageSize(int64_t offset) {
	std::ifstream slotDat;
	slotDat.open(filename, std::ios::binary);
	slotDat.seekg(offset);

	int64_t size = getMaxPageSize(slotDat);

	slotDat.close();
	return size;
}

/* CNF */
int64_t Slot::getNextSectorOffset(int64_t currentOffset) {
	return getAlignment(currentOffset, sector);
}

int32_t Slot::getSectorSize() {
	return this->sector;
}

void Slot::processQar(std::string filename, std::string region, std::string* workDir) {}

void Slot::processSlot(std::string filename, int16_t page, int64_t offset, std::string region, std::string* workDir) {}

void Slot::processVram(std::string filename, std::string region, std::string* workDir) {}

void Slot::processAfp(std::string filename, std::string region, std::string* workDir) {	}

void Slot::processFile(std::string filename, std::string region, std::string* workDir, uint8_t* fileData, int size) {
	writeDataToFile(fileData, size, filename, *workDir);
}

void Slot::processEnc(std::string region, uint8_t** section, int64_t size, int64_t sectionSize) {
	decryptPage(*section, size);
	*section = decompressPage(*section, size, sectionSize);
}