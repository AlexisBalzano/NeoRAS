#include <algorithm>
#include <fstream>

#include "../NeoRAS.h"
#include "DataManager.h"

#ifdef DEV
#define LOG_DEBUG(loglevel, message) loggerAPI_->log(loglevel, message)
#else
#define LOG_DEBUG(loglevel, message) void(0)
#endif

DataManager::DataManager(ras::NeoRAS* neoRAS)
	: neoRAS_(neoRAS) {
	aircraftAPI_ = neoRAS_->GetAircraftAPI();
	flightplanAPI_ = neoRAS_->GetFlightplanAPI();
	airportAPI_ = neoRAS_->GetAirportAPI();
	chatAPI_ = neoRAS_->GetChatAPI();
	loggerAPI_ = neoRAS_->GetLogger();
	controllerDataAPI_ = neoRAS_->GetControllerDataAPI();
	controllerAPI_ = neoRAS_->GetControllerAPI();
	fsdAPI_ = neoRAS_->GetFsdAPI();

	configPath_ = getDllDirectory();
}


std::filesystem::path DataManager::getDllDirectory()
{
	std::filesystem::path documentsPath = neoRAS_->GetClientInformation().documentsPath;
	DisplayMessageFromDataManager("Documents path: " + documentsPath.string());
	return documentsPath / "/plugins";
}

void DataManager::clearData()
{
	configJson_.clear();
	configPath_.clear();
	if (aircraftAPI_)
		aircraftAPI_ = nullptr;
	if (flightplanAPI_)
		flightplanAPI_ = nullptr;
	if (airportAPI_)
		airportAPI_ = nullptr;
}

void DataManager::clearJson()
{
	std::lock_guard<std::mutex> lock(dataMutex_);
	configJson_.clear();
}

void DataManager::DisplayMessageFromDataManager(const std::string& message, const std::string& sender)
{
	Chat::ClientTextMessageEvent textMessage;
	textMessage.sentFrom = "NeoRAS";
	(sender.empty()) ? textMessage.message = ": " + message : textMessage.message = sender + ": " + message;
	textMessage.useDedicatedChannel = true;

	chatAPI_->sendClientMessage(textMessage);
}

void DataManager::getActiveAirports(const std::string& positionIdent)
{
	// Need to get the "active" array from ATC-Data.json found in the loaded package
}

std::string DataManager::getMetar(const std::string& oaci)
{
	httplib::Client cli("https://metar.vatsim.net");
	httplib::Headers headers = { {"User-Agent", "NeoRAS-plugin"} };
	std::string apiEndpoint = "/" + oaci;

	auto res = cli.Get(apiEndpoint.c_str(), headers);
	if (res && res->status != 200) {
		std::string errorMsg = "Error fetching METAR for " + oaci + ": HTTP " + std::to_string(res->status);
		LOG_DEBUG(PluginSDK::Logger::LogLevel::Error, errorMsg);
		return errorMsg;
	}
	else if (res && res->status == 200) {
		LOG_DEBUG(PluginSDK::Logger::LogLevel::Info, "Successfully fetched METAR for " + oaci);
		return res->body;
	}
	else {
		std::string errorMsg = "Error fetching METAR for " + oaci + ": No response from server";
		LOG_DEBUG(PluginSDK::Logger::LogLevel::Error, errorMsg);
		return errorMsg;
	}
}

ras::WindData DataManager::parseMetar(const std::string& metar)
{
	// Extract wind information from the METAR string 00000KT + if Variable + gusts
	ras::WindData windData = { 0, {0, 0}, 0};
	auto it = metar.find("KT");
	if (it != std::string::npos) {
		std::string windSegment = metar.substr(0, it);
		size_t pos = windSegment.find_last_of(' ');
		if (pos != std::string::npos) {
			windSegment = windSegment.substr(pos + 1);
		}
		if (windSegment.length() >= 5) {
			windData.windDirection = std::stoi(windSegment.substr(0, 3));
			windData.windSpeed = std::stoi(windSegment.substr(3, 2));
			size_t gustPos = windSegment.find("G");
			if (gustPos != std::string::npos && gustPos + 2 < windSegment.length()) {
				windData.windGust = std::stoi(windSegment.substr(gustPos + 1, 2));
			}
		}
	}
	return windData;
}

bool DataManager::activateAirport(std::string oaci)
{
	std::transform(oaci.begin(), oaci.end(), oaci.begin(), ::toupper);
	return airportAPI_->setAirportStatus(oaci, Airport::AirportStatus::Active);
}
