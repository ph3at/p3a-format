/// P3A file version 1200 documentation header

#pragma once

#include <cstdint>
#include <xxhash.h> // uses the xxhash library for all hashes

namespace ph3lib::archive {

constexpr int P3A_MAX_PATH = 256;
constexpr char MAGIC_NUMBER[8] = "PH3ARCV";
constexpr char MAGIC_DICT_NUMBER[8] = "P3ADICT";
constexpr uint32_t ARCHIVE_VERSION = 1200;
constexpr uint32_t EXTENDED_HEADER_MIN_VER = 1200;

constexpr uint32_t HAS_ZSTD_DICTIONARY_FLAG = 1;

enum class CompressionScheme : uint64_t {
	None = 0,
	Lz4,
	Zstd,
	ZstdDict,
	NumSchemes,
};

// PH3ARCV file format:
//
// Header
// ------
// [ Extended Header ] // included starting from EXTENDED_HEADER_MIN_VER (version 1200)
// ------
// Header.num_files * IndexEntry
// ------
// [ Dictionary ] // optional, present iff Header.flags & HAS_ZSTD_DICTIONARY_FLAG == 1
// ------
// File data

// NOTES
// - path separator is alyways "/"
// - all file paths are lower case, and lookups can be automatically converted to lower case
// - all strings in P3A files are assumed to be utf8

// data structures

#pragma pack(push, 1)

struct IndexEntryV1100 {
	char path[P3A_MAX_PATH];
	CompressionScheme compression;
	uint64_t compressed_size;
	uint64_t uncompressed_size;
	uint64_t offset;              // offset of (compressed) data from start of archive
	XXH64_hash_t compressed_hash; // hash of the compressed data; checked optionally
};

struct IndexEntryV1200 {
	IndexEntryV1100 base;
	XXH64_hash_t uncompressed_hash; // hash of the uncompressed data; used for updating
};

using IndexEntry = IndexEntryV1200;

struct Header {
	char magic[8];
	uint32_t flags;
	uint32_t version;
	uint64_t num_files;
	XXH64_hash_t header_hash; // hash of magic, flags, version and num_files; will always be checked

	Header(uint32_t flags = 0, uint64_t num_files = 0, uint32_t version = ARCHIVE_VERSION);
	[[nodiscard]] XXH64_hash_t computeHash() const;
	[[nodiscard]] bool verifyHash() const;
};

struct ExtendedHeader {
	XXH64_hash_t hash; // hash of the rest of the extended header; will always be checked
	uint32_t extended_header_size;
	uint32_t index_entry_size;

	ExtendedHeader(uint32_t extended_header_size = sizeof(ExtendedHeader), uint32_t index_entry_size = sizeof(IndexEntry));
	[[nodiscard]] XXH64_hash_t computeHash() const;
	[[nodiscard]] bool verifyHash() const;
};

inline const ExtendedHeader NO_EXTENDED_HEADER = ExtendedHeader(0,0);

struct Dictionary {
	char magic[8];
	uint64_t size;
	// followed by "size" dictionary bytes

	Dictionary(uint64_t dict_size = 0);
};

#pragma pack(pop)
