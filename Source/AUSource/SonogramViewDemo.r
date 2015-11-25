#include <AudioUnit/AudioUnit.r>

#include "SonogramViewDemoVersion.h"

// Note that resource IDs must be spaced 2 apart for the 'STR ' name and description
#define kAudioUnitResID_SonogramViewDemo				1000

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SonogramViewDemo~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define RES_ID			kAudioUnitResID_SonogramViewDemo
#define COMP_TYPE		kAudioUnitType_Effect
#define COMP_SUBTYPE	SonogramViewDemo_COMP_SUBTYPE
#define COMP_MANUF		SonogramViewDemo_COMP_MANF	

#define VERSION			kSonogramViewDemoVersion
#define NAME			"Apple Demo: SonogramViewDemo"
#define DESCRIPTION		"SonogramViewDemo AU"
#define ENTRY_POINT		"SonogramViewDemoEntry"

#include "AUResources.r"