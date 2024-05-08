#include "SturdyEngine/SturdyEngine.h"
#include "iostream"

void setup(SFT::SturdyEngine* instance) {
	std::cout << "setup" << std::endl;
}
void update(SFT::SturdyEngine* instance, double delapsedTime) {
	std::cout << "update called" << std::endl;
}
void clean(SFT::SturdyEngine* instance) {
	std::cout << "clean called" << std::endl;
}


int main() {
	SFT::SturdyEngine app = SFT::SturdyEngine();
	app.setSetupFn(setup);
	app.setMainFn(update);
	app.setCleanFn(clean);
	app.run();
}