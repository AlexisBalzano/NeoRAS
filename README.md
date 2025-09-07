# NeoRAS
Automatic Runway Assignement for *NeoRadar* ATC Client <br>

# Installation
- Drop `NeoRAS[.dll/.so/.dylib]` in `Documents/NeoRadar/plugins`.
- Load the plugin from within NeoRadar UI.

# Usage
PLUGIN_USAGE_INSTRUCTIONS

# Commands
- `.ras help` : Display help information.
- `.ras version` : Display the plugin version.
- `.ras assign <ICAO>` : Assigns runways for the specified airport ICAO code.
- `.ras unaassign <ICAO>` : Unassigns runways for the specified airport ICAO code.
- `.ras assign <ATCPOS>` : Assign runways for all active airports of ATC position.
- `.ras unaassign` : Unassign all runways.
- `.ras include <ICAO>` : include airport in the automatic runway assignment.
- `.ras exclude <ICAO>` : exclude airport from the automatic runway assignment.
- `.ras reset` : reset all runway assignments.