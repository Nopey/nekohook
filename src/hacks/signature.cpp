
/*
 *
 *	This file is used for getting pointers from signatures.
 *		Note: This file was stolen from darkstorm and refitted for the purposes of hookcat.
 *			  All credits goto gir for the amazing work on darkstorm <3
 *				-Onee
 *
 */

#include "../util/logging.hpp" // logging is cool

#include "signature.hpp"

namespace hacks {

// Credits to LEARN_MORE

#define INRANGE(x,a,b)		(x >= a && x <= b)
#define getBits( x )		(INRANGE(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define getByte( x )		(getBits(x[0]) << 4 | getBits(x[1]))

static uintptr_t findPattern(const uintptr_t rangeStart, const uintptr_t rangeEnd, const char* pattern) {
	auto pat = reinterpret_cast<const unsigned char*>(pattern);
	uintptr_t firstMatch = 0;
	for (uintptr_t pCur = rangeStart; pCur < rangeEnd; ++pCur) {
		if (*(uint8_t*)pat == (unsigned char)'\?' || *(uint8_t*)pCur == (uint8_t)getByte(pat)) {
			if (!firstMatch) {
				firstMatch = pCur;
			}
			pat += (*(uint16_t*)pat == *(uint16_t*)"\?\?" || *(uint8_t*)pat != (unsigned char)'\?') ? 3 : 2;
			if (!*pat) {
				return firstMatch;
			}
		} else if (firstMatch) {
			pCur = firstMatch;
			pat = reinterpret_cast<const unsigned char*>(pattern);
			firstMatch = 0;
		}
	}
	return (uintptr_t)nullptr;
}

// This is just a temp function to get info our of our shared object
uintptr_t GetObjectSignature(const SharedObject& shared_obj, const char* chPattern) {
	#ifdef __linux__
		std::tuple<const SharedObject&, uintptr_t, size_t> tmp = {shared_obj, 0, 0};
		// Find a nicer way to not do this every time we need a sig
		if (!dl_iterate_phdr([](struct dl_phdr_info* info, size_t, void* data) {
			auto tmp_ptr = (std::tuple<const SharedObject&, uintptr_t, size_t>&)data;
			if (std::string(info->dlpi_name).find(std::string("/") + std::get<0>(tmp_ptr).file_name) == std::string::npos) return 0;
			std::get<1>(tmp_ptr) = info->dlpi_addr + info->dlpi_phdr[0].p_vaddr;
			std::get<2>(tmp_ptr) = info->dlpi_phdr[0].p_memsz;
			return 1;
		}, (void*)&tmp))
			return findPattern(std::get<1>(tmp), std::get<1>(tmp) + std::get<2>(tmp), chPattern);
	#else
		#pragma message ("Unable to get signatures for this platform")
	#endif
	g_CatLogging.log("Couldnt find signature for \"%s\" at \"%s\"",shared_obj.file_name, chPattern);
	return 0;
}

}
