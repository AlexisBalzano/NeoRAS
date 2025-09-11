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

        definition.name = "ras setup";
        definition.description = "Assign runways for given ATC Position";
        definition.lastParameterHasSpaces = false;
        definition.parameters.clear();

        PluginSDK::Chat::CommandParameter atcIdent;
        icao.name = "ATCident";
        icao.type = PluginSDK::Chat::ParameterType::String;
        icao.required = false;
        definition.parameters.push_back(atcIdent);

        setupCommandId_ = chatAPI_->registerCommand(definition.name, definition, CommandProvider_);

        definition.parameters.clear();
        definition.name = "ras unset";
        definition.description = "Unassign all runways";
        definition.lastParameterHasSpaces = false;

        unsetCommandId_ = chatAPI_->registerCommand(definition.name, definition, CommandProvider_);
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
        chatAPI_->unregisterCommand(setupCommandId_);
        chatAPI_->unregisterCommand(unassignCommandId_);
        chatAPI_->unregisterCommand(unsetCommandId_);
        chatAPI_->unregisterCommand(includeCommandId_);
        chatAPI_->unregisterCommand(excludeCommandId_);

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
		  ".ras setup <ATC Position>",
		  ".ras unset",
          ".ras include <ICAO>",
		  ".ras exclude <ICAO>"
            })
        {
            neoRAS_->DisplayMessage(line);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        return { true, std::nullopt };
    }
    else if (commandId == neoRAS_->assignCommandId_) {
        std::string oaci = args[0];
        std::transform(oaci.begin(), oaci.end(), oaci.begin(), ::toupper);
		std::string metar = neoRAS_->GetDataManager()->getMetar(oaci);
		std::string message = neoRAS_->GetDataManager()->parseMetar(metar).toString();
		neoRAS_->DisplayMessage(message, "METAR");
		return { true, std::nullopt };
    }
    else if (commandId == neoRAS_->includeCommandId_) {
        std::string oaci = args[0];
        std::transform(oaci.begin(), oaci.end(), oaci.begin(), ::toupper);
        bool success = neoRAS_->GetDataManager()->activateAirport(oaci);
		std::string message = success ? "Airport " + oaci + " included in auto runway assignment." : "Failed to include airport " + oaci + ".";
		neoRAS_->DisplayMessage(message);
		return { true, std::nullopt };
    }
    else if (commandId == neoRAS_->excludeCommandId_) {
        std::string oaci = args[0];
        std::transform(oaci.begin(), oaci.end(), oaci.begin(), ::toupper);
		bool success = neoRAS_->GetDataManager()->deactivateAirport(oaci);
        std::string message = success ? "Airport " + oaci + " excluded from auto runway assignment." : "Failed to exclude airport " + oaci + ".";
		neoRAS_->DisplayMessage(message);
		return { true, std::nullopt };
    }
    else if (commandId == neoRAS_->unassignCommandId_) {
        std::string oaci = args[0];
        std::transform(oaci.begin(), oaci.end(), oaci.begin(), ::toupper);
        bool success = neoRAS_->GetDataManager()->deleteRunways(oaci);
        std::string message = success ? "Unassigned " + oaci + " runways." : "Couldn't unassign runways of " + oaci + ".";
        neoRAS_->DisplayMessage(message);
        return { true, std::nullopt };
    }
	// Additional command handling can be added here
    else {
        return { false, "error"};
    }

	return { true, std::nullopt };
}
}  // namespace ras