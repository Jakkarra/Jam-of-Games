#pragma once
/*
	Sprite type
	A sprite handles all rendering and collisions
	It maintains a surface and a set of frames describing the surface
*/

namespace HAPISPACE {

	// Simple POD describing a frame of the surface
	struct Frame final
	{
		// the name is optional so may be empty
		std::string name; 

		// Rectangular area of surface that makes up this frame
		Rectangle rect;
		
		// Construct from a rectangle, name will be empty
		Frame(Rectangle rectIn) : rect{ std::move(rectIn) } {}

		// Construct from rectangle and name
		Frame(std::string nameIn, Rectangle rectIn) : name{ std::move(nameIn) }, rect{ std::move(rectIn) } {}

		// Name empty, rectangle invalid
		Frame() = default;
	};

	// A sprite class that handles all blitting and frames
	class Sprite final
	{
	private:		
		std::shared_ptr<Surface> m_surface;
		std::vector<Frame> m_frames;
		BYTE m_minAlphaForCollision{ 255 };
		void ParseXML(const std::string &xmlFilename);
		bool RotatedCollisionCheck(const Sprite& other, const Point thisPos, Point otherPos,
			unsigned int thisFrameNumber, unsigned int otherFrameNumber,float thisAngle, float otherAngle) const;
		const Frame& GetFrameFast(unsigned int frameNum) const { return m_frames.data()[frameNum]; } //14.6
	public:
		// Allows creation from an equally spaced set of frames vertically or horizontally
		Sprite(std::shared_ptr<Surface> surface, unsigned int numFrames = 1, bool layoutHorizontal = true);

		// Allows creation from a provided vector of Frame representing the frames within the surface
		Sprite(std::shared_ptr<Surface> surface, std::vector<Frame> frames);

		// Allows creation from an existing Surface and an XML file that describes the layout
		Sprite(std::shared_ptr<Surface> surface, const std::string& xmlFilename);

		// Allows creation of a new surface and Sprite from an XML file that describes the layout
		Sprite(const std::string& xmlFilename, const std::string& textureRootPath=std::string());

		~Sprite();

		// Gets access to the surface the sprite uses
		std::shared_ptr<Surface> GetSurface() const { return m_surface; }	

		// Returns the number of frames
		auto GetNumFrames() const { return m_frames.size(); }

		// Return the information for one frame
		bool GetFrame(unsigned int frameNum, Frame& frame) const;

		// Return the information for all frames
		const std::vector<Frame>& GetAllFrames() const { return m_frames; }

		// Completely replaces the frame information
		void ModifyFrames(const std::vector<Frame>& newFrames);

		// Render sprite's surface with pos top left on passed surface with frame number frameNum (0 based)
		void Render(Surface& surface, Point pos, unsigned int frameNum = 0, 
			EBlendMode blendMode=EBlendMode::eAlphaBlend, HAPI_TColour *modulate = nullptr) const;

		// Render sprite's surface with pos top left on passed surface with frame number frameNum (0 based)
		// Uses a provided lambda to handle blending
		void Render(Surface& surface, Point pos,
			const std::function<void(const Point &p, HAPI_TColour&, const HAPI_TColour&)> &blendLambda, unsigned int frameNum = 0) const;

		// Render sprite's surface with pos top left on passed surface with frame number frameNum (0 based)
		// Uses a provided lambda to handle blending by scanline
		void Render(Surface& surface, Point pos, 
			const std::function<void(const Point &p,HAPI_TColour*, const HAPI_TColour*, int)> &scanlineLambda, unsigned int frameNum = 0) const;

		// Render sprite's surface rotated around its centre on passed surface with frame number frameNum (0 based)
		void RenderRotated(Surface& surface, Point pos, float radians,unsigned int frameNum = 0,
			EBlendMode blendMode = EBlendMode::eAlphaBlend, HAPI_TColour *modulate = nullptr) const;

		// Render sprite's surface rotated around its centre on passed surface with frame number frameNum (0 based)
		// Uses a provided lambda to handle blending
		void RenderRotated(Surface& surface, Point pos, float radians, 
			const std::function<void(const Point &p, HAPI_TColour&, const HAPI_TColour&)> &blendLambda, unsigned int frameNum = 0) const;

		// min alpha defaults to 255 meaning collisions occur only when alpha is 255
		// This function allows you to lower that amount
		void SetMinimumAlphaForCollision(BYTE minAlpha) { m_minAlphaForCollision = minAlpha; }

		// Checks to see if the other sprite and this have collided. Does bounding check then pixel perfect check.
		// If angles are other than 0 then does a rotated collision check (slower)
		bool CheckCollision(Point thisPos, const Sprite& other, Point otherPos,
			unsigned int thisFrameNumber=0, float thisAngle = 0.0f, unsigned int otherFrameNumber=0,float otherAngle=0.0f) const;

		// Creates a new sprite with a new surface which is made up solely of areas in the frame collection
		// Useful for trimming away dead areas or for making more cache friendly 
		// if alongX is true then frames are added left to right
		// if alongX is false then frames are added top to bottom (the default as best cache friendly method)
		// Note: does memory allocation so potentially slow
		Sprite CreateCompacted(bool alongX=false) const;

		// Joins two sprites together to create a third. 		
		// if alongX is true then other sprite is added to the right of this
		// if alongX is false then other sprite is added below this
		// Note: does memory allocation so potentially slow
		Sprite CreateCombined(const Sprite& other, bool alongX = true) const;

		// Creates a new surface repeating the image num times, each time scaleX, scaleY different in size
		// Then creates a new sprite with frame data set to each of the repeats
		// if alongX is true then scaled surfaces are added horizontally
		// if alongX is false then scaled surfaces are added vertically
		// Be careful! The resulting surface could become enormous if your scale factors are too high
		Sprite CreateSizes(float scaleX, float scaleY, int num, bool alongX=true) const;

		// Saves an XML file describing the layout of the surface
		// Optionally saves the surface as well (in the same directory)
		bool SaveAsXML(const std::string& filename, bool saveSurface = false) const;
	};
}