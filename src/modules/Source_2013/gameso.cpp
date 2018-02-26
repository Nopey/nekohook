/*
 * sharedobj.cpp
 * (The second half, where it actually loads the Source Shared Object & stores it globally)
 *
 *  Created on: Oct 3, 2016
 *      Author: nullifiedcat
 */

#include <string>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

#include "gameso.hpp"

namespace gameso
{
    //TODO: Remove/Replace Helper Function:
    // You are responsible for delete[]'ing the resulting string.
    char *strfmt(const char *fmt, ...)
    {
        char *buf = new char[1024];
        va_list list;
        va_start(list, fmt);
        vsprintf(buf, fmt, list);
        va_end(list);
        return buf;
    }


bool LocateSharedObject(std::string &name, std::string &out_full_path)
{
    FILE *proc_maps = fopen(strfmt("/proc/%i/maps", getpid()), "r");
    if (proc_maps == nullptr)
    {
        return false;
    }
    char buffer[512];
    while (fgets(buffer, 511, proc_maps))
    {
        char *path     = strchr(buffer, '/');
        char *filename = strrchr(buffer, '/') + 1;
        if (not path or not filename)
            continue;
        if (!strncmp(name.c_str(), filename, name.length()))
        {
            out_full_path.assign(path);
            out_full_path.resize(out_full_path.size() - 1);
            return true;
        }
    }
    return false;
}

/*void SharedObject::Load()
{
    while (not LocateSharedObject(file, path))
    {
        sleep(1);
    }
    while (!(lmap = (link_map *) dlopen(path.c_str(), RTLD_NOLOAD)))
    {
        sleep(1);
        char *error = dlerror();
        if (error)
        {
            logging::Info("DLERROR: %s", error);
        }
    }
    logging::Info("Shared object %s loaded at 0x%08x", basename(lmap->l_name),
                  lmap->l_addr);
    if (factory)
    {
        fptr = reinterpret_cast<fn_CreateInterface_t>(
            dlsym(lmap, "CreateInterface"));
        if (!this->fptr)
        {
            logging::Info("Failed to create interface factory for %s",
                          basename(lmap->l_name));
        }
    }
}

void *SharedObject::CreateInterface(const std::string &interface)
{
    return (void *) (fptr(interface.c_str(), nullptr));
}
void LoadAllSharedObjects()
{
    try
    {
        steamclient().Load();
        client().Load();
        engine().Load();
        vstdlib().Load();
        tier0().Load();
        inputsystem().Load();
        materialsystem().Load();
        vguimatsurface().Load();
        vgui2().Load();
        studiorender().Load();
        libsdl().Load();
    }
    catch (std::exception &ex)
    {
        logging::Info("Exception: %s", ex.what());
    }
}
*/
SharedObject &steamclient()
{
    static SharedObject obj("steamclient.so");//, true);
    return obj;
}
SharedObject &client()
{
    static SharedObject obj("client.so");//, true);
    return obj;
}
SharedObject &engine()
{
    static SharedObject obj("engine.so");//, true);
    return obj;
}
SharedObject &vstdlib()
{
    static SharedObject obj("libvstdlib.so");//, true);
    return obj;
}
SharedObject &tier0()
{
    static SharedObject obj("libtier0.so");//, false);
    return obj;
}
SharedObject &inputsystem()
{
    static SharedObject obj("inputsystem.so");//, true);
    return obj;
}
SharedObject &materialsystem()
{
    static SharedObject obj("materialsystem.so");//, true);
    return obj;
}
SharedObject &vguimatsurface()
{
    static SharedObject obj("vguimatsurface.so");//, true);
    return obj;
}
SharedObject &vgui2()
{
    static SharedObject obj("vgui2.so");//, true);
    return obj;
}
SharedObject &studiorender()
{
    static SharedObject obj("studiorender.so");//, true);
    return obj;
}
SharedObject &libsdl()
{
    static SharedObject obj("libSDL2-2.0.so.0");//, false);
    return obj;
}
}
