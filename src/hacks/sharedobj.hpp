
/*
 * sharedobj.h
 *
 *  Created on: Oct 3, 2016
 *      Author: nullifiedcat
 */

#pragma once

#if defined(__linux__)
#include <link.h> // link maps
typedef link_map* CatLinkMap_t;
#elif defined(_WIN32)
#include <windows.h> // loadlibrary
typedef HMODULE CatLinkMap_t;
#endif
#include <string>	// std::string

// Create a shared object with the name of the shared object, then it attempts to find its path and allows access to the linkmap
class SharedObject {
public:
	SharedObject(const char* _file_name);
	const char* file_name;	// The name of the library we want
 	std::string path; // The full path of the shared library we are looking for
	CatLinkMap_t lmap = nullptr; // Link map returned from dllopen
private:
	void RefreshPath(); // Used by the class to set or refresh the path
	void RefreshLmap();
};
