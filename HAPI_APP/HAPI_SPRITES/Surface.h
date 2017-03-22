#pragma once
/*
	Surface type
	Holds the raw surface data and has functions used by sprite along with some basic 2D drawing functions.
	Used to represent the screen via the special SCREEN_SURFACE macro as well as for textures
	Note: never changes width and height or numBytes.
*/

namespace HAPISPACE {
	
	// Blending modes for draw operations
	enum class EBlendMode
	{
		eReplace,			// result = source
		eAlphaBlend			// result = alpha * source + (1 - alpha) * dest, this is normal alpha blending
		//eComingSoon
	};	

	class Surface final
	{
	private:
		friend class CHapiSpritesCore;
		Surface(BYTE *alignedMemoryPointer, unsigned int width, unsigned int height);		
		
		HAPI_TColour *m_texels{ nullptr }; // Guaranteed to be aligned to 128 bit				
		const unsigned int m_width, m_height;
		const size_t m_numBytes;
		bool m_free{ true };	
		std::string m_filename;

		friend class Sprite;
		HAPI_TColour* GetRawData() const { return m_texels; }

		// Draws for when all ops guaranteed to be on the surface
		void DrawCircleGuaranteed(Point centre, int radius, HAPI_TColour borderColour);
		void DrawFilledCircleGuaranteed(Point centre, int radius, HAPI_TColour fillColour);
		void DrawLineGuaranteed(Line line,HAPI_TColour colour, EBlendMode blendMode);
		void SetPixel(int x, int y, const std::function<void(HAPI_TColour&)> &blendLambda);
		bool RoughlyEquals(HAPI_TColour colour1, HAPI_TColour colour2, float errorPercentage);
	public:
		// Construct from image file, most formats supported
		Surface(const std::string& filename);		

		// Construct from scratch and fills with colour
		Surface(unsigned int width, unsigned int height, HAPI_TColour colour = HAPI_TColour(0));		

		// Construct from an area of another surface
		Surface(const Surface &other, Rectangle copyArea);		

		// Copy constructor, allocates memory and copies raw data - deep copy
		Surface(const Surface &other);

		// Copy constructor from a shared pointer wrapped surface, allocates memory and copies raw data - deep copy
		Surface(const std::shared_ptr<Surface> &other) : Surface(*other.get()) {}		

		// Destructor
		~Surface();

		// Makes a deep copy of this surface and returns shared pointer to new surface
		std::shared_ptr<Surface> MakeCopy() const { return std::make_shared<Surface>(*this); }

		// Used to check surface has been created succesfully
		bool HasData() const { return (m_width > 0 && m_height > 0 && m_texels); }

		// Width of the surface in texels
		unsigned int Width() const { return m_width; }
		
		// Height of the surface in texels
		unsigned int Height() const { return m_height; }

		// Number of texels in the sruface
		unsigned int GetNumPixels() const { return m_width*m_height; }

		// Returns a rectangle set to the dimensions of the surface
		Rectangle GetRect() const { return Rectangle(0, m_width, 0, m_height); }

		// Returns the filename used when creating the surface
		std::string GetFilename() const { return m_filename; }

		// Allows the filename to be changed. Useful when saving XML via Sprite
		void ChangeFilename(std::string newFilename) { m_filename = std::move(newFilename); }

		// Makes on/off alpha by setting alpha to transparent wherever colour=colourKey otherwise opaque
		// If errorPercentage is 0 (the default) then only an exact match will for oldColour will be used
		// If this is higher then rough matches are also included. Note a value of 100 means all colours 
		void MakeAlphaChannelFromColourKey(HAPI_TColour colourKey, float errorPercentage=0);

		// Allows a colour in the surface to be changed to another colour. 
		// If errorPercentage is 0 (the default) then only an exact match will for oldColour will be used
		// If this is higher then rough matches are also included. Note a value of 100 means all colours are changed.
		// Very quick for error=0, bit slower otherwise
		// Note: ignores alpha
		void Recolour(HAPI_TColour oldColour, HAPI_TColour newColour, float errorPercentage = 0.0f);

		// Blit onto this surface from other surface at top left = posX, posY and use 'area' of other. 
		// Blending is determined by blendMode
		void Blit(const Surface &other, Point pos, Rectangle area, 
			EBlendMode blendMode=EBlendMode::eAlphaBlend, HAPI_TColour *modulate=nullptr);

		// Blit onto this surface from other surface at top left = posX, posY and use 'area' of other.
		// Uses provided lambda to handle blending a texel at a time
		void Blit(Surface& other, Point pos, Rectangle area,
			const std::function<void(const Point &p, HAPI_TColour&, const HAPI_TColour&)> &blendLambda);

		// Special blit function which uses a provided lambda to handle blending one scanline at a time for speed
		void Blit(Surface& other, Point pos, Rectangle area, 
			const std::function<void(const Point &p, HAPI_TColour*, const HAPI_TColour*, int)> &blendLambda);

		// Blit onto this surface from other surface and rotate around its centre (middle of area rectangle) 
		// Blending is determined by blendMode
		void BlitRotated(const Surface &other, Point pos, Rectangle area, float radians,
			EBlendMode blendMode = EBlendMode::eAlphaBlend, HAPI_TColour *modulate = nullptr);

		// Blit onto this surface from other surface and rotate around its centre (middle of area rectangle) 
		// Blending is determined by provided lambda
		void BlitRotated(const Surface &other, Point pos, Rectangle area, float radians, 
			const std::function<void(const Point &p, HAPI_TColour&, const HAPI_TColour&)> &blendLambda);

		// Set a pixel to a colour, ignores if not on surface, blending based on colour alpha channel
		void SetPixel(Point pos, HAPI_TColour colour);

		// Returns the colour of a pixel
		HAPI_TColour GetPixel(Point pos) const;

		// Set a pixel to a colour, ignores if not on surface, uses lambda for blending
		void SetPixel(Point pos, const std::function<void(HAPI_TColour&)> &blendLambda);		

		// Draws a single pixel wide line, blending based on colour alpha channel
		void DrawLine(Line line, HAPI_TColour colour, EBlendMode blendMode = EBlendMode::eAlphaBlend);

		// Draws a single pixel wide line, blending based on colour alpha channel
		void DrawLine(Point start, Point end, HAPI_TColour colour, EBlendMode blendMode = EBlendMode::eAlphaBlend) { DrawLine(Line{ start,end }, colour); }

		// Draws a single pixel wide line, uses lambda for blending. 		
		void DrawLine(Line line, const std::function<void(HAPI_TColour&)> &blendLambda);

		// Draw a number of lines at once, blending based on colour alpha channel
		// If closeShape true draws a final line from last point of last line to first point of first line
		void DrawLines(const std::vector<Line>& lines, HAPI_TColour colour, EBlendMode blendMode = EBlendMode::eAlphaBlend, bool closeShape = false);

		// Draws a number of lines at once, uses lambda for blending
		// If closeShape true draws a final line from last point of last line to first point of first line
		void DrawLines(const std::vector<Line>& lines, const std::function<void(HAPI_TColour&)> &blendLambda, bool closeShape = false);

		// Draws a rectangle outline
		void DrawRect(Rectangle rect, HAPI_TColour borderColour, EBlendMode blendMode = EBlendMode::eAlphaBlend);

		// Draws a filled in rectangle
		void DrawFilledRect(Rectangle rect, HAPI_TColour fillColour, EBlendMode blendMode = EBlendMode::eAlphaBlend);

		// Draws a rectangle outline with blending based on lambda
		void DrawRect(Rectangle rect,const std::function<void(HAPI_TColour&)> &blendBorderLambda);

		// Draws a filled rectangle with blending based on lambda
		void DrawFilledRect(Rectangle rect, const std::function<void(HAPI_TColour&)> &blendFillLambda);

		// Draws a circle outline with blending based on colour alpha channel
		void DrawCircle(Point centre, int radius, HAPI_TColour borderColour);

		// Draws a filled circle
		void DrawFilledCircle(Point centre, int radius, HAPI_TColour fillColour);

		// Draws a circle outline with blending based on lambda
		void DrawCircle(Point centre, int radius,const std::function<void(HAPI_TColour&)> &blendBorderLambda);

		// Draws a filled circle with blending based on lambda
		void DrawFilledCircle(Point centre, int radius,const std::function<void(HAPI_TColour&)> &blendFillLambda);

		// Greyscale clear - very quick
		void Clear(BYTE grey=0);
		
		// Colour clear
		void Clear(HAPI_TColour colour);

		// Flips the image horizontally. Width and Height stay the same.
		void FlipHorizontal();

		// Joins two surfaces together to create a third. Note: does dynamic memory allocation.
		// if alongX is true then other surface is added to the right of this
		// if alongX is false then other surface is added below this
		std::shared_ptr<Surface> Combine(const std::shared_ptr<Surface>& other, bool alongX = true) const;

		// Creates a new surface which is a scaled version of this
		// Currently only supports nearest pixel filtering - other methods are planned to be added later
		std::shared_ptr<Surface> CreateScaled(float scaleX, float scaleY) const;

		// Creates a new surface which is a scaled version of this by fitting it into the provided height and width
		// Currently only supports nearest pixel filtering - other methods are planned to be added later
		std::shared_ptr<Surface> CreateScaled(unsigned int width, unsigned int height) const;

		// Saves the surface using the currently set filename
		// Supported formats are bmp, png, tga and jpg - format is derived from filename extension
		bool SaveImageFile() const;

		// Saves a copy of the surface with a new filename (does not change filename)
		// Supported formats are bmp, png, tga and jpg - format is derived from filename extension
		// Optionally can sepcify just an area to save
		bool SaveImageFile(const std::string& filename,Rectangle *area=nullptr) const;
	};	
}