#pragma once
#include <vector>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <mutex>
#include <unordered_set>

using namespace PluginSDK;

class DataManager {
public:
	DataManager(ras::NeoRAS* neoRAS);
	~DataManager() = default;

	void clearData();
	void clearJson();

	static std::filesystem::path getDllDirectory();
	void DisplayMessageFromDataManager(const std::string& message, const std::string& sender = "");

	void getActiveAirports(const std::string& positionIdent);
	void getMetar(const std::string& oaci); // Change return type accordingly
	void assignRunways(const std::string& oaci);
	void activateAirports(const std::string& oaci);

private:
	Aircraft::AircraftAPI* aircraftAPI_ = nullptr;
	Flightplan::FlightplanAPI* flightplanAPI_ = nullptr;
	Airport::AirportAPI* airportAPI_ = nullptr;
	PluginSDK::ControllerData::ControllerDataAPI* controllerDataAPI_ = nullptr;
	Chat::ChatAPI* chatAPI_ = nullptr;
	ras::NeoRAS* neoRAS_ = nullptr;
	PluginSDK::Logger::LoggerAPI* loggerAPI_ = nullptr;

	std::filesystem::path configPath_;
	nlohmann::ordered_json configJson_;

	std::mutex dataMutex_;
};