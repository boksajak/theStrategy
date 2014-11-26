#pragma once

namespace gw {
	
	struct MouseState {
		int x;
		int y;

		float fx;
		float fy;

		float wheelDelta;

		bool leftClick;
		bool middleClick;
		bool rightClick;

		bool leftDown;
		bool middleDown;
		bool rightDown;
	};

	struct InputState {
		MouseState mouse;
	};

	class Input {
	public:

		bool Initialize(float screenWidth, float screenHeight);
		void Destroy();

		void Tick();

		InputState state;
	private:

		float screenWidth;
		float screenHeight;
	};
}