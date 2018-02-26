/*
 * sharedobj.h
 * (The second half, where it actually loads the Source Shared Object & stores it globally)
 * 
 *  Created on: Oct 3, 2016
 *      Author: nullifiedcat
 */

#pragma once
#include <string>
#include <vector>
#include "../../hacks/sharedobj.hpp"

struct link_map;
typedef void *(*fn_CreateInterface_t)(const char *, int *);

namespace gameso
{
bool LocateSharedObject(std::string &name, std::string &out_full_path);
inline char *Pointer(SharedObject &so, uintptr_t offset)
{
    if (so.lmap != nullptr)
    {
        return reinterpret_cast<char *>(uintptr_t(so.lmap->l_addr) + offset);
    }
    else
    {
        return nullptr;
    }
}

SharedObject &steamclient();
SharedObject &client();
SharedObject &engine();
SharedObject &vstdlib();
SharedObject &tier0();
SharedObject &inputsystem();
SharedObject &materialsystem();
SharedObject &vguimatsurface();
SharedObject &vgui2();
SharedObject &studiorender();
SharedObject &libsdl();

//No longer needed, as the Load() is called in the constructor
//void LoadAllSharedObjects();

}

