#include "Renderer.h"
#include "Utils\Geometry.h"
#include "Utils\TextFile.h"


namespace gw {

	bool Renderer::Initialize(int windowWidth, int windowHeight) {

		this->screenSize = glm::vec2(windowWidth, windowHeight);

		// -------------------------------------------
		// Initialize SDL 
		// -------------------------------------------
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			TRACE_ERROR("Error while initializing SDL." << std::endl);
			return false;
		}

		// Double buffering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); 

		// Assert HW Accelerated graphics
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		// 24-bit Z buffer
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		// Get screen resolution
		SDL_DisplayMode displayMode;
		if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0) {
			TRACE_ERROR("SDL_GetDesktopDisplayMode failed: " << SDL_GetError() << std::endl);
			return false;
		}

		int screenWidth = displayMode.w;
		int screenHeight = displayMode.h;

		// Calculate optimal window position on screen
		int left = (screenWidth - windowWidth - 550) / 3; // 550 - make room for debugging console window
		int top = (screenHeight - windowHeight) / 3;

		// Create the window 
		sdlWindow = SDL_CreateWindow("The Strategy Game", left, top, windowWidth, windowHeight, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL); 

		if (!sdlWindow) {
			TRACE_ERROR("Error while creating SDL window." << std::endl);
			TRACE_ERROR(SDL_GetError() << std::endl);
			return false;
		}

		// Move console window to nice position right of main window
#ifdef USE_CONSOLE
		int borderWidth = GetSystemMetrics(SM_CXSIZEFRAME);
		int borderHeight = GetSystemMetrics(SM_CYSIZEFRAME);
		int captionHeight = GetSystemMetrics(SM_CYCAPTION);
		consoleInst.move(left + windowWidth + borderWidth, top - borderHeight - captionHeight, 550, windowHeight);
#endif


		// -------------------------------------------
		// Initialize OpenGL 
		// -------------------------------------------
		sdlGLContext = SDL_GL_CreateContext(sdlWindow); 

		if (!sdlGLContext) {
			TRACE_ERROR("Error while creating SDL GLContext." << std::endl);
			TRACE_ERROR(SDL_GetError() << std::endl);
			return false;
		}

		GLenum glewStatus = glewInit();

		if (GLEW_OK != glewStatus){
			TRACE_ERROR("Error: " << glewGetErrorString(glewStatus) << std::endl);
			return false;
		} 

		// Vertical Sync
		SDL_GL_SetSwapInterval(1); 

		TRACE_RENDERER("Context created: " << glGetString(GL_VERSION) << std::endl);

		// GL Settings
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);   
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable( GL_BLEND );

		glClearColor(1.0f, 1.0f, 0.6f, 1.0f);

		// Load shaders
		if (!InitializeBillboards()) {
			return false;
		}

		TRACE_RENDERER("Initialized OpenGL Renderer!" << std::endl);
		return true;
	}

	void Renderer::Destroy(){

		SDL_GL_DeleteContext(sdlGLContext);
		SDL_DestroyWindow(sdlWindow);
		SDL_Quit();
	}

	void Renderer::Render() {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// --------------------------------------------------------------------
		//    Render billboards
		// --------------------------------------------------------------------

		glBindVertexArray(vaoBillboard);
		glUseProgram(billboardShader.programId);

		// TODO: Maybe insertion sort (or shell sort) will be faster than priority queue for our purpose
		// Render transparent objects back to front and opaque objects front to back to minimize overdraw - in this matter are billboards already sorted
		while (!billboards.empty()) {
			const GLBillboard &billboard = billboards.top();

			// Render billboard
			glUniform3fv(billboardShader.screenCoordsLoc, 1, glm::value_ptr(glm::vec3(billboard.topLeftCoord.x, billboard.topLeftCoord.y, billboard.z)));
			glUniform2fv(billboardShader.sizeLoc, 1, glm::value_ptr(billboard.size));
			glUniform2fv(billboardShader.topLeftUVLoc, 1, glm::value_ptr(billboard.uvTopLeft));
			glUniform2fv(billboardShader.bottomRightUVLoc, 1, glm::value_ptr(billboard.uvBottomRight));

			glActiveTexture(GL_TEXTURE10);
			glBindTexture(GL_TEXTURE_2D, billboard.texture.textureId);
			glUniform1i(billboardShader.textureLoc, 10);

			glDrawArrays(GL_POINTS, 0, 1);

			billboards.pop();
		}


		glBindVertexArray(0);

		// Swap buffers
		SDL_GL_SwapWindow(sdlWindow);
	}

	void Renderer::RenderBillboard(size_t texIdx, glm::vec2 screenCoords, glm::vec2 size, glm::vec2 uvTopLeft, glm::vec2 uvBottomRight, float rotation, float z) {

		// Culling
		static const glm::vec4 screenBox(-1.0f, -1.0f, 1.0f, 1.0f);
		if (!BoxIntersect(glm::vec4(screenCoords.x, screenCoords.y, screenCoords.x + size.x, screenCoords.y + size.y), screenBox)) return; 

		// Put object into render queue
		billboards.push(GLBillboard(textures[texIdx], screenCoords, size, uvTopLeft, uvBottomRight, rotation, z)); 
	}

	size_t Renderer::UploadTexture(char* bytes, size_t width, size_t height, char bitsPerPixel) {

		GLuint textureId;

		GLenum format;
		GLenum type;
		bool hasAlpha = false;

		switch(bitsPerPixel) {
		case 8:
			format = GL_LUMINANCE;
			type = GL_UNSIGNED_BYTE;
			break;
		case 16:
			format = GL_BGRA;
			type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
			hasAlpha = true;
			break;
		case 24:
			format = GL_BGR;
			type = GL_UNSIGNED_BYTE;
			break;
		case 32:
			format = GL_BGRA;
			type = GL_UNSIGNED_BYTE;
			hasAlpha = true;
			break;
		case 96:
			format = GL_BGR;
			type = GL_FLOAT;
			break;
		default:
			TRACE_ERROR("Not supported pixel depth of " << (int)bitsPerPixel << "bpp encountered" << std::endl);
			return -1;
		}

		// Load Texture to GPU
		glGenTextures(1, &textureId);

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, type, bytes);

		glGenerateMipmap(GL_TEXTURE_2D);   

		// Trilinear filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Enable anisotropic filtering if supported 
		if(glewIsExtensionSupported("GL_EXT_texture_filter_anisotropic")) {
			GLfloat fLargest;

			// Get largest anisotropy level supported
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(4.0f, fLargest)); // we want level 4 if possible
		}

		textures.push_back(GLTexture(textureId, hasAlpha));

		return textures.size() - 1;
	}

	bool Renderer::InitializeBillboards() {

		// Load shader
		if (!LoadShader("shaders/billboard", billboardShader.programId)) {
			return false;
		}

		// Shader attributes
		billboardShader.screenCoordsLoc = glGetUniformLocation(billboardShader.programId, "screenCoords");
		billboardShader.sizeLoc = glGetUniformLocation(billboardShader.programId, "size");
		billboardShader.topLeftUVLoc = glGetUniformLocation(billboardShader.programId, "topLeftUV");
		billboardShader.bottomRightUVLoc = glGetUniformLocation(billboardShader.programId, "bottomRightUV");
		billboardShader.textureLoc = glGetUniformLocation(billboardShader.programId, "bbTexture");
		
		// ----------------------------------------------------------------
		// ******** Initialize VBO for drawing model *********
		// ----------------------------------------------------------------

		// Generate vertex array object (VAO)
		glGenVertexArrays(1, &vaoBillboard);

		// Start using this VAO
		glBindVertexArray(vaoBillboard);

		GLuint vertexBuffer;
		glGenBuffers(1, &vertexBuffer);

		// Start using this buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		//glEnableVertexAttribArray(phongShader->positionLoc); 
		//glVertexAttribPointer(phongShader->positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, position));

		//glEnableVertexAttribArray(phongShader->normalLoc); 
		//glVertexAttribPointer(phongShader->normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, normal));

		//glEnableVertexAttribArray(phongShader->texCoordLoc); 
		//glVertexAttribPointer(phongShader->texCoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, texCoords));

		//// Copy data to buffer
		//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &(vertices[0]), GL_STATIC_DRAW);

		// Cancel VAO binding
		glBindVertexArray(0);

		return true;
	}

	void Renderer::printShaderInfoLog(GLuint obj){    
#ifndef USE_CONSOLE
		return;
#endif

		int infologLength = 0;
		int charsWritten  = 0;    
		char *infoLog;     

		glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);     

		if (infologLength > 0)    {  
			infoLog = (char *)malloc(infologLength);    
			glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
			if (charsWritten>0) TRACE(infoLog << std::endl);        
			free(infoLog);    
		}
	} 

	void Renderer::printProgramInfoLog(GLuint obj){   
#ifndef USE_CONSOLE
		return;
#endif
		int infologLength = 0;    
		int charsWritten  = 0; 

		char *infoLog;   

		glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);  

		if (infologLength > 0)    {     
			infoLog = (char *)malloc(infologLength);  
			glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);    
			if (charsWritten>0) TRACE(infoLog << std::endl);    
			free(infoLog);   
		}
	} 

	bool Renderer::LoadShader(const char* name, GLuint &p) {

		char *vs = NULL,*fs = NULL, *gs = NULL,*gs_eval = NULL,*gs_control = NULL;  

		GLuint v,f,g,g_eval,g_control;    

		TRACE_LOADER("Compiling shader: " << name << std::endl);

		v = glCreateShader(GL_VERTEX_SHADER);  
		f = glCreateShader(GL_FRAGMENT_SHADER); 
		g = glCreateShader(GL_GEOMETRY_SHADER); 
		g_eval = glCreateShader(GL_TESS_EVALUATION_SHADER); 
		g_control = glCreateShader(GL_TESS_CONTROL_SHADER); 

		StringBuilder sb(1024);

		sb.Append(name, ".vp");
		TextFile vertexShaderFile(sb.GetString());
		vertexShaderFile.ReadFile(vs);

		sb.Clear(); 
		sb.Append(name, ".fp");
		TextFile fragmentShaderFile(sb.GetString());
		fragmentShaderFile.ReadFile(fs);

		sb.Clear(); 
		sb.Append(name, ".geom");
		TextFile geometryShaderFile(sb.GetString());
		geometryShaderFile.ReadFile(gs);

		sb.Clear(); 
		sb.Append(name, "_eval.geom");
		TextFile evalGeometryShaderFile(sb.GetString());
		evalGeometryShaderFile.ReadFile(gs_eval);

		sb.Clear(); 
		sb.Append(name, "_control.geom");
		TextFile controlGeometryShaderFile(sb.GetString());
		controlGeometryShaderFile.ReadFile(gs_control);

		if (!vs) {
			TRACE_ERROR("Could not load shader. Vertex shader file ("<<name<<") not found!" << std::endl);
			return false;
		}

		const char * vv = vs;
		const char * ff = fs; 
		const char * gg = gs; 
		const char * gg_eval = gs_eval; 
		const char * gg_control = gs_control; 

		glShaderSource(v, 1, &vv, NULL);
		if (fs) glShaderSource(f, 1, &ff,NULL);
		if (gs) glShaderSource(g, 1, &gg,NULL);
		if (gs_eval) glShaderSource(g_eval, 1, &gg_eval,NULL);
		if (gs_control) glShaderSource(g_control, 1, &gg_control,NULL);

		glCompileShader(v);  
		if (fs) glCompileShader(f);  
		if (gs) glCompileShader(g);  
		if (gs_eval) glCompileShader(g_eval);  
		if (gs_control) glCompileShader(g_control);  

		printShaderInfoLog(v);  
		if (fs) printShaderInfoLog(f);  
		if (gs) printShaderInfoLog(g);  
		if (gs_eval) printShaderInfoLog(g_eval);  
		if (gs_control) printShaderInfoLog(g_control);  

		p = glCreateProgram();  

		glAttachShader(p,v);   
		if (fs) glAttachShader(p,f);    
		if (gs) glAttachShader(p,g);    
		if (gs_control) glAttachShader(p,g_eval);    
		if (gs_eval) glAttachShader(p,g_control);   

		glBindFragDataLocation(p, 0, "outputColor");    // TODO: toto asi netreba napisat natvrdo, ale co uz

		glLinkProgram(p);  
		printProgramInfoLog(p);  

		// Cleanup
		glDetachShader(p, v);
		if (fs) glDetachShader(p, f);
		if (gs) glDetachShader(p, g);
		if (gs_eval) glDetachShader(p, g_eval);
		if (gs_control) glDetachShader(p, g_control);

		glDeleteShader(v);
		if (fs) glDeleteShader(f);
		if (gs) glDeleteShader(g);
		if (gs_eval) glDeleteShader(g_eval);
		if (gs_control) glDeleteShader(g_control);

		return true;
	}

}