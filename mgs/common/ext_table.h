#pragma once
#include <inttypes.h>

struct EXT_TABLE {
	const char* name;
	int id;
};

const EXT_TABLE ext_table[48] = {
	{"gcx",  0x02 },
	{"bhv",  0x26 },
	{"tri",  0x03 },
	{"zon",  0x12 },
	{"rcm",  0x22 },
	{"mtar", 0x08 },
	{"gnv",  0x23 },
	{"mtcm", 0x0B },
	{"cv2",  0x07 },
	{"mds",  0x05 },
	{"hzt",  0x15 },
	{"hz2",  0x16 },
	{"lt2",  0x06 },
	{"cvg",  0x20 },
	{"rpd",  0x25 },
	{"geom", 0x0C },
	{"cvd",  0x1D },
	{"mtra", 0x1F },
	{"mdh",  0x04 },
	{"nhz",  0x19 },
	{"mdl",  0x0D },
	{"mdb",  0x0E },
	{"mdb",  0x0E },
	{"eft",  0x18 },
	{"sar",  0x1E },
	{"mtsq", 0x09 },
	{"row",  0x14 },
	{"mtz",  0x1C },
	{"psq",  0xFF },
	{"slot", 0x60 },
	{"bin",  0x01 },
	{"nav",  0x0F },
	{"fnt",  0x17 },
	{"sta",  0x24 },
	{"eft",  0x11 },
	{"efg",  0x21 },
	{"far",  0x0A },
	{"rpd",  0x13 },
	{"mtn",  0x1B },
	{"cvd",  0x10 },
	{"trz",  0x1A },
	{"la2",  0x5F },
	{"rat",  0x6B },
	{"img",  0x69 },
	{"var",  0x68 },
	{"raw",  0x6E },
	{"ico",  0x5E },
	{"rcm",  0x6C }
};

inline
const char* getExtForID(uint8_t id) {
	for (int i = 0; i < 48; i++) {
		if (ext_table[i].id == id)
			return ext_table[i].name;
	}
	return "";
}

inline
uint8_t getIDforExt(const char* ext) {
	for (int i = 0; i < 48; i++) {
		if (!strcmp(ext_table[i].name, ext))
			return ext_table[i].id;
	}
	return -1;
}