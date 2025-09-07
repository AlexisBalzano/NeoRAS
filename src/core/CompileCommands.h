#include <algorithm>
#include <string>

#include "NeoRAS.h"


namespace ras {
void NeoRAS::RegisterCommand() {
    try
    {
        CommandProvider_ = std::make_shared<NeoRASCommandProvider>(this, logger_, chatAPI_, fsdAPI_);

        PluginSDK::Chat::CommandDefinition definition;
        definition.name = "ras version";
        definition.description = "Print NeoRAS version";
        definition.lastParameterHasSpaces = false;
		definition.parameters.clear();

        versionCommandId_ = chatAPI_->registerCommand(definition.name, definition, CommandProvider_);
    }
    catch (const std::exception& ex)
    {
        logger_->error("Error registering command: " + std::string(ex.what()));
    }
}

inline void NeoRAS::unegisterCommand()
{
    if (CommandProvider_)
    {
        chatAPI_->unregisterCommand(versionCommandId_);
        CommandProvider_.reset();
	}
}

Chat::CommandResult NeoRASCommandProvider::Execute( const std::string &commandId, const std::vector<std::string> &args)
{
    if (commandId == neoRAS_->versionCommandId_)
    {
		neoRAS_->DisplayMessage("NeoRAS version " + std::string(PLUGIN_VERSION));
        return { true, std::nullopt };
	}
    else {
        return { false, "error"};
    }

	return { true, std::nullopt };
}
}  // namespace ras