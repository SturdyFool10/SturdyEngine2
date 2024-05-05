#include "Window.h"

void Resized(const SFT::input::ResizeEvent& event) {

}

int main() {
	auto win = SFT::Window(800, 600, "Hello World!", true);
	while (!win.shouldWindowClose()) {
		win.pollEvents();
	}
}