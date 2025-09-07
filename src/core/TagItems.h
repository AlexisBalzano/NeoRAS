#pragma once
#include <chrono>
#include <format>
#include <string>

#include "NeoRAS.h"

#ifdef DEV
#define LOG_DEBUG(loglevel, message) logger_->log(loglevel, message)
#else
#define LOG_DEBUG(loglevel, message) void(0)
#endif

namespace ras {
void NeoRAS::RegisterTagItems()
{
    PluginSDK::Tag::TagItemDefinition tagDef;

    // Tag item def
    tagDef.name = "TAG_ITEM";
    tagDef.defaultValue = "PLACEHOLDER";
    std::string tagID = tagInterface_->RegisterTagItem(tagDef);
    tagItemId_ = tagID;
}
} // namespace ras