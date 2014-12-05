#pragma once
#include "Game\Renderer.h"
#include "Game\Input.h"
#include "World\Button.h"
#include "World\Cursor.h"

namespace gw {

	class OKButtonAction : public ButtonAction {
	public:

		OKButtonAction() : msg(NULL) {}
		OKButtonAction(char* msg) : msg(msg) {}

		void operator()() {
			if (msg != NULL) TRACE_WARNING(msg << std::endl);
		}

	private:

		char* msg;
	};

	class GUI {
	public:

		bool Initialize(Renderer &renderer);

		void Update(Input &input);
		void Render(Renderer &renderer);

		static void FrameStart();

	private:
		size_t inactiveTexID;
		size_t activeTexID;
		size_t overTexID;
		
		Button okBtn;
		OKButtonAction okAction;

		Cursor arrowCursor;
	};
}