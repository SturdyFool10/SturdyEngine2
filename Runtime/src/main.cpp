#include "Window.h"
#include "iostream"
void Resized(const SFT::input::ResizeEvent& event) {
	std::cout << "Resized: (" << event.size.x << ", " << event.size.y << ")" << std::endl;
}

int main() {
	auto win = SFT::Window(800, 600, "Hello World!", true);
	win.addResizeHandler(Resized);
	while (!win.shouldWindowClose()) {
		win.pollEvents();
	}
}