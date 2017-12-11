
/*
 * hooks.h
 *
 *  Created on: Oct 4, 2016
 *      Author: nullifiedcat
 *
 *	Parts of copypasted code
 *	Credits: Casual_Hacker
 *
 */

#pragma once

// Class used to hook functions in objects with virtual functions
class VMTHook {
public:
	~VMTHook();
	void Set(void* inst, size_t offset = 0); // For when you start hooking.
	void HookMethod(void* func, size_t idx); // Give it the function you are replacing with and the number position of the function to replace
	void Apply();	// To finish your hook
	void Release(); // To release this hook
	inline void* GetMethod(size_t idx) const { return vtable_original[idx]; }

	void*** vtable_ptr = nullptr; 		 // location to the pointer in the object that contains its vtable
	void** vtable_original = nullptr; // location of the objects original vtable
	void** vtable_hooked = nullptr;   // location of our vtable replacement
};

namespace offsets {

// This is for offsets to functions in vtable on multiple platforms.
constexpr int PlatformOffset(int offset_linux = -1, int offset_windows = -1, int offset_osx = -1) {
#if CMAKE_SYSTEM_NAME == Linux
	return offset_linux;
#elif CMAKE_SYSTEM_NAME == Windows
	return offset_windows;
#else
	return -1;
#endif

}
}