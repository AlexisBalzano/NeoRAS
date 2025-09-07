#include "NeoRAS.h"

extern "C" PLUGIN_API PluginSDK::BasePlugin *CreatePluginInstance()
{
    try
    {
        return new ras::NeoRAS();
    }
    catch (const std::exception &e)
    {
        return nullptr;
    }
}