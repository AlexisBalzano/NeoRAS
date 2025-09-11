#pragma once
#include <vector>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <mutex>
#include <unordered_set>

using namespace PluginSDK;

namespace ras {
struct WindData {
	int windDirection;
	std::pair<int, int> windVariation;
	int windSpeed;
	int windGust = 0;

	std::string toString() { // DEBUG Function
		return "Wind: " + std::to_string(windDirection) + " at " + std::to_string(windSpeed) + " KT"
			+ (windGust > 0 ? ", gusts " + std::to_string(windGust) + " KT" : "")
			+ (windVariation.first != 0 || windVariation.second != 0 ? ", variable " + std::to_string(windVariation.first) + "° to " + std::to_string(windVariation.second) + "°" : "");
	}
};
}

class DataManager {

public:
	DataManager(ras::NeoRAS* neoRAS);
	~DataManager() = default;

	void clearData();
	void clearJson();

	std::filesystem::path getDllDirectory();
	void DisplayMessageFromDataManager(const std::string& message, const std::string& sender = "");

	std::vector<std::string> getActiveAirports(const std::string& positionIdent);
	std::string getMetar(const std::string& oaci);
	ras::WindData parseMetar(const std::string& metar);
	bool assignRunways(const std::string& oaci);
	bool deleteRunways(const std::string& oaci);
	bool activateAirport(std::string oaci);
	bool deactivateAirport(std::string oaci);

private:
	Aircraft::AircraftAPI* aircraftAPI_ = nullptr;
	Flightplan::FlightplanAPI* flightplanAPI_ = nullptr;
	Airport::AirportAPI* airportAPI_ = nullptr;
	ControllerData::ControllerDataAPI* controllerDataAPI_ = nullptr;
	Controller::ControllerAPI* controllerAPI_ = nullptr;
	Fsd::FsdAPI* fsdAPI_ = nullptr;
	Chat::ChatAPI* chatAPI_ = nullptr;
	Logger::LoggerAPI* loggerAPI_ = nullptr;
	ras::NeoRAS* neoRAS_ = nullptr;

	std::filesystem::path configPath_;
	nlohmann::ordered_json configJson_;

	std::mutex dataMutex_;
};