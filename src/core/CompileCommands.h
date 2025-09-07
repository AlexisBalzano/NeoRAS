#include <algorithm>
#include <string>

#include "NeoRAS.h"


namespace ras {
void NeoRAS::RegisterCommand() {
    try
    {
        CommandProvider_ = std::make_shared<NeoRASCommandProvider>(this, logger_, chatAPI_, fsdAPI_);

        PluginSDK::Chat::CommandDefinition definition;
        definition.name = "ras help";
        definition.description = "Print NeoRAS available commands";
        definition.lastParameterHasSpaces = false;
        definition.parameters.clear();

        helpCommandId_ = chatAPI_->registerCommand(definition.name, definition, CommandProvider_)
            ;
        definition.name = "ras version";
        definition.description = "Print NeoRAS version";
        definition.lastParameterHasSpaces = false;
		definition.parameters.clear();

        versionCommandId_ = chatAPI_->registerCommand(definition.name, definition, CommandProvider_);

        definition.name = "ras assign";
        definition.description = "Assign runways for given ICAO";
        definition.lastParameterHasSpaces = false;
        definition.parameters.clear();

		PluginSDK::Chat::CommandParameter icao;
		icao.name = "icao";
		icao.type = PluginSDK::Chat::ParameterType::String;
        icao.minLength = 4;
        icao.maxLength = 4;
        icao.required = true;
		definition.parameters.push_back(icao);

        assignCommandId_ = chatAPI_->registerCommand(definition.name, definition, CommandProvider_);

        definition.name = "ras unassign";
        definition.description = "Unassign runways for given ICAO";
        definition.lastParameterHasSpaces = false;

        unassignCommandId_ = chatAPI_->registerCommand(definition.name, definition, CommandProvider_);
        
        definition.name = "ras include";
        definition.description = "Include airport in auto runway assignement";
        definition.lastParameterHasSpaces = false;

        includeCommandId_ = chatAPI_->registerCommand(definition.name, definition, CommandProvider_);

        definition.name = "ras exclude";
        definition.description = "Exclude airport in auto runway assignement";
        definition.lastParameterHasSpaces = false;

        excludeCommandId_ = chatAPI_->registerCommand(definition.name, definition, CommandProvider_);

        definition.name = "ras assign";
        definition.description = "Assign runways for given ATC Position";
        definition.lastParameterHasSpaces = false;
        definition.parameters.clear();

        PluginSDK::Chat::CommandParameter atcIdent;
        icao.name = "ATCident";
        icao.type = PluginSDK::Chat::ParameterType::String;
        icao.required = true;
        definition.parameters.push_back(atcIdent);

        assignGroupCommandId_ = chatAPI_->registerCommand(definition.name, definition, CommandProvider_);

        definition.name = "ras unassign";
        definition.description = "Unassign runways for given ATC Position";
        definition.lastParameterHasSpaces = false;

        unassignGroupCommandId_ = chatAPI_->registerCommand(definition.name, definition, CommandProvider_);
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
        chatAPI_->unregisterCommand(helpCommandId_);
        chatAPI_->unregisterCommand(versionCommandId_);
        chatAPI_->unregisterCommand(assignCommandId_);
        chatAPI_->unregisterCommand(assignGroupCommandId_);
        chatAPI_->unregisterCommand(unassignCommandId_);
        chatAPI_->unregisterCommand(unassignGroupCommandId_);
        chatAPI_->unregisterCommand(includeCommandId_);
        chatAPI_->unregisterCommand(excludeCommandId_);
        chatAPI_->unregisterCommand(resetCommandId_);

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
    else if (commandId == neoRAS_->helpCommandId_) {
        for (const char* line : {
		  "NeoRAS available commands:",
          ".ras version",
		  ".ras assign <ICAO>",
		  ".ras unassign <ICAO>",
		  ".ras assign <ATC Position>",
		  ".ras unassign <ATC Position>",
          ".ras include <ICAO>",
		  ".ras exclude <ICAO>"
            })
        {
            neoRAS_->DisplayMessage(line);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        return { true, std::nullopt };
    }
	// Additional command handling can be added here
    else {
        return { false, "error"};
    }

	return { true, std::nullopt };
}
}  // namespace ras