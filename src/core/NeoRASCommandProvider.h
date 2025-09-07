#pragma once
#include "NeoRAS.h"

using namespace PluginSDK;

namespace ras {

class NeoRAS;

class NeoRASCommandProvider : public PluginSDK::Chat::CommandProvider
{
public:
    NeoRASCommandProvider(ras::NeoRAS *neoRAS, PluginSDK::Logger::LoggerAPI *logger, Chat::ChatAPI *chatAPI, Fsd::FsdAPI *fsdAPI)
            : neoRAS_(neoRAS), logger_(logger), chatAPI_(chatAPI), fsdAPI_(fsdAPI) {}
		
	Chat::CommandResult Execute(const std::string &commandId, const std::vector<std::string> &args) override;

private:
    Logger::LoggerAPI *logger_ = nullptr;
    Chat::ChatAPI *chatAPI_ = nullptr;
    Fsd::FsdAPI *fsdAPI_ = nullptr;
    NeoRAS *neoRAS_ = nullptr;
};
}  // namespace ras