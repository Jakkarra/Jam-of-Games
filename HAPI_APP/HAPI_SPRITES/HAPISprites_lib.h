#pragma once
/*
HAPISprites_lib.h
All the includes required to use the HAPI Sprites library.

For full details on using the library please see the online documentation (CTRL-Click to open)
https://scm-intranet.tees.ac.uk/users/u0018197/Happier/reference.html

Last modified by Keith Ditchburn: 5th February 2017

Version 0.50 - 05/02/2017
- Surface functions added:
	- GetPixel
	- CreateScaled
- Sprite functions added:
	- CreateSizes
Added insertion operators for Points, lines and Rectangles

Version 0.40 - 16/01/2017
- Testing & Optimisations
- Initial documentation

Version 0.40 - 09/01/2017 
- Bug fixing
- Pixel perfect collisions implemented
- Further operators added
- Rotation working

Previous Versions
- Initial builds
*/

// Standard library types required by HAPI Sprites
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>

// Core HAPI library types
#include "HAPI/HAPI_Types.h"
#include "HAPI/HAPI_InputCodes.h"

// HAPI Sprites types
#include "Point.h"
#include "Line.h"
#include "Rectangle.h"
#include "Surface.h"
#include "Sprite.h"

// All of HAPI Sprites is wrapped inside the HAPISPACE namespace
namespace HAPISPACE {
	
	// Core HAPI
	class IHapi;

	// Describes a key event
	enum class EKeyEvent
	{
		ePress,
		eRelease
	};

	// Describes a mouse event
	enum class EMouseEvent
	{
		eLeftButtonDown,
		eLeftButtonUp,
		eMiddleButtonDown,
		eMiddleButtonUp,
		eRightButtonDown,
		eRightButtonUp,
		eWheelForward,
		eWheelBack,
		eXButton
	};

	// Describes a controlelr event
	enum class EControllerEvent
	{
		eAttached,
		eUnattached,
		eDigitalPress,
		eDigitalRelease,
		eAnalogue
	};

	// Interface for input callbacks
	class IHapiInputListener
	{
	public:		
		// Called when HAPI detects a key event
		virtual void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) = 0;
		
		// Called when HAPI detects a mouse event other mouse move
		virtual void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) = 0;

		// Non virtual as may not want to implement - gets a lot of calls 
		virtual void OnMouseMove(const HAPI_TMouseData& mouseData) {};

		// Called when HAPI detects a controller event
		virtual void OnControllerEvent(unsigned int controllerId, EControllerEvent controllerEvent, unsigned int buttonIndex,
			const HAPI_TControllerData& controllerData) = 0;
	};

	// Public HAPI Sprites Interface
	class IHapiSprites
	{
	public:
		// Initialise to required size, optionally can provide a window title and can combine some flags: eDefaults, eFullscreen, ePerfOverlay
		virtual bool Initialise(unsigned int &width, unsigned int &height, const std::string &windowTitle = std::string("HAPI Sprites"),
			unsigned int flags = eDefaults) = 0;

		// Add listener to get input events
		virtual bool AddInputListener(IHapiInputListener &listener) = 0;

		// Remove input event listener
		virtual bool RemoveInputListener(IHapiInputListener &listener) = 0;

		// Returns the screen surface
		virtual Surface& GetScreenSurface() const = 0;

		// Create a surface from a texture, returns a shared pointer
		virtual std::shared_ptr<Surface> MakeSurface(const std::string &filename) const = 0;

		// Create a surface from scratch, returns a shared pointer
		virtual std::shared_ptr<Surface> MakeSurface(unsigned int width, unsigned int height, HAPI_TColour colour = HAPI_TColour(0)) const = 0;

		// Creates a full copy of a surface, returns new shared pointer
		virtual std::shared_ptr<Surface> CopySurface(const std::shared_ptr<Surface> &other) const = 0;

		// Causes HAPI to close the window and return false in the next update call
		virtual bool Close() = 0;

		// HAPI Sprites must be regularly updated via this call. Returns false when app is closing.
		virtual bool Update() = 0;

		// ************************************************************
		// The following functions all map 1:1 to the core HAPI library
		// ************************************************************
		
		// Tells HAPI to display (or not) the frames per second, defaults to upper left (0,0)
		virtual bool SetShowFPS(bool set, unsigned int x = 0, unsigned int y = 0) = 0;

		// Retrieve the current FPS
		virtual int GetFPS() const = 0;

		// Tells HAPI to display or not the cursor (mouse pointer)
		virtual bool SetShowCursor(bool set) = 0;

		// Returns a structure with the current mouse state
		virtual const HAPI_TMouseData& GetMouseData() const = 0;

		// Returns a structure with the current keyboard state
		virtual const HAPI_TKeyboardData& GetKeyboardData() const = 0;

		// Returns the maximum number of controller devices that could be installed (not how many actually are)
		virtual unsigned int GetMaxControllers() const = 0;

		// Returns a structure with the specified controller index (up to GetMaxControllers) state
		// Note: you must check that the isAttached member is true
		virtual const HAPI_TControllerData& GetControllerData(unsigned int controller) const = 0;

		// Sets the rumble motor speeds for the specified controller
		virtual bool SetControllerRumble(unsigned int controller, WORD leftMotor, WORD rightMotor) const = 0;

		// Load a sound file, most formats are supported:
		// ogg, wav, flac, aiff, au, raw, paf, svx, nist, voc, ircam, w64, mat4, mat5 pvf, htk, sds, avr, sd2, caf, wve, mpc2k, rf64.
		virtual bool LoadSound(const std::string &filename) = 0;

		// Note: there is a hardware limit on how many sounds can be playing at once (1024 on PC but 32 on mobiles)
		virtual bool PlaySound(const std::string &filename) = 0;

		// As above but also allows volume, pan etc. to be changed from the defaults
		virtual bool PlaySound(const std::string &filename, const HAPI_TSoundOptions &options) = 0;

		// As above but also returns instanceId (for use when querying sound status)
		virtual bool PlaySound(const std::string &filename, const HAPI_TSoundOptions &options, unsigned int &instanceId) = 0;

		// You can change things like pitch, volume and loop as the sound is playing
		virtual bool ChangePlayingSoundOptions(unsigned int &soundInstanceId, const HAPI_TSoundOptions &options) = 0;

		// Stop a sound early. Use the id retrieved from PlaySound
		virtual bool StopSound(unsigned int soundInstanceId, bool togglePause = false) = 0;

		// Retrieve info on a sound instance e.g. to see if it is paused, finished etc.
		virtual const HAPI_MediaStatus GetSoundStatus(unsigned int soundInstanceId, float &playbackPosition) const = 0;

		// Allows access to the raw sample data of a sound
		// Note: to see how to interpret the raw data use the values from HAPI_TSoundFormat
		virtual bool GetSoundRawData(const std::string& filename, std::vector<SWORD>& rawData, HAPI_TSoundFormat &soundInfo) const = 0;

		// Allows changing a sound's sample data. Note: if sound is currently playing it will be stopped.
		virtual bool SetSoundRawData(const std::string& filename, const std::vector<SWORD>& rawData, const HAPI_TSoundFormat &soundInfo) = 0;

		// Allows a sound to be created from scratch.
		virtual bool CreateSound(const std::string& name, const std::vector<SWORD>& rawData, const HAPI_TSoundFormat &soundInfo) = 0;

		// Saves a sound under a new filename
		// Supported formats (derived from filename extension) :
		// ogg, wav, flac, aiff, au, raw, paf, svx, nist, voc, ircam, w64, mat4, mat5 pvf, htk, sds, avr, sd2, caf, wve, mpc2k, rf64.
		virtual bool SaveSound(const std::string& name, const std::string& newFilename) const = 0;

		// Plays streamed video / music using default options
		virtual bool PlayStreamedMedia(const std::string &filename) = 0;

		// Plays streamed media and allows playback options to be set and instance obtained
		virtual bool PlayStreamedMedia(const std::string &filename, const HAPI_TStreamedMediaOptions& options, unsigned int &instanceId) = 0;

		// Restarts streamed media that has already been played (more efficient than reloading it)
		virtual bool PlayStreamedMedia(unsigned int instanceId) = 0;

		// Stops streaming media started via PlayStreamedMedia
		virtual bool StopStreamedMedia(unsigned int instanceId) const = 0;

		// Pause or unpause streaned media started with PlayStreamedMedia
		virtual bool PauseStreamedMedia(unsigned int instanceId, bool pause = true) const = 0;

		// Returns info about the media e.g. like current position
		virtual const HAPI_TStreamedMediaInfo& GetStreamedMediaInfo(unsigned int instanceId) const = 0;

		// If a HAPI call fails this can be used to retrieve the error code and system the error occurred in
		// (see HAPI_ErrorCodes enum and online documentation for more details on errors as well as the output pane in viz and the generated log)
		virtual HAPI_ErrorCodes GetLastError(HAPI_System &system) const = 0;

		// Returns the time since HAPI was initialised in milliseconds
		virtual DWORD GetTime() const = 0;

		// Allows the font to be changed. This version uses fonts registered with the OS
		virtual bool ChangeFont(const std::string &fontName) = 0;

		// Allows the font to be changed. This version allows it to be loaded from a file.
		virtual bool ChangeFontFromFile(const std::string &fontFilename) = 0;

		// Allows your own debug text to be displayed in the output pane of Visual Studio and the log
		virtual void DebugText(const std::string &str) = 0;

		// Allows text to be drawn on the screen at a position and with a specified fill colour with the current font
		// Text size and flags for bold, italic etc. are optional
		virtual bool RenderText(unsigned int x, unsigned int y, const HAPI_TColour &fillColour, const std::string& text,
			unsigned int textSize = 12, unsigned int styleFlags = eRegular) = 0;

		// Allows the size of the text to be calculated. Returns the size in width and height variables
		virtual bool GetTextSize(unsigned int x, unsigned int y,  const std::string& text, unsigned int textSize,
			unsigned int styleFlags, int &width, int  &height, float outlineThickness = 0) = 0;

		// As above but allows you to specify another colour to act as an outline to the text and how thick it should be
		// DISABLED: due to memory leaks in SFML outlining is temporarily disabled
		virtual bool RenderText(unsigned int x, unsigned int y, const HAPI_TColour &fillColour, const HAPI_TColour &outlineColour,
			float outlineThickness, const std::string& text, unsigned int textSize = 12, unsigned int styleFlags = eRegular) = 0;
	
		// Display a message box to the user
		virtual bool UserMessage(const std::string &text, const std::string& title,
			HAPI_ButtonType buttonType = HAPI_ButtonType::eButtonTypeOk, HAPI_UserResponse *userResponse = nullptr) = 0;

		// Displays a modal dialog box to get input from the user
		virtual bool UserTextEntry(const std::string &question, std::string &result,
			std::string initialText = std::string(), int dialogWidth = 0) = 0;

		// Display a file open or file save dialog
		virtual bool FileDialog(const std::vector<HAPI_TFileDialogFilters>& filters, bool saveAsDialog,
			std::string &filename, HAPI_UserResponse &response) = 0;

		// Allows the icon shown in the window and task bar to be changed from the default HAPI logo
		virtual bool SetIcon(const Surface& surface, unsigned int width, unsigned int height) const = 0;
	};
}

// Returns the HAPI Sprites instance
HAPISPACE::IHapiSprites& GetHAPI_Sprites();

// the HAPI Sprites instance. A shortcut to the above
#define HAPI_Sprites GetHAPI_Sprites()

// Another shortcut, this time to the always present screen surface
#define SCREEN_SURFACE HAPI_Sprites.GetScreenSurface()

// Useful macros
#define DEGREES_TO_RADIANS(x) (x * 0.0174533f)
#define RAIDANS_TO_DEGREES(x) (x * 57.2958f)




