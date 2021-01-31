// Various authors.
// License: GPL version 2 or later.

#include "background.h"

#include "gmenu2x.h"


Background::Background(GMenu2X& gmenu2x)
	: gmenu2x(gmenu2x)
{
	setBehave(LAY_FILL);
}

void Background::paint(Surface& s) {
	const auto& bgmain = gmenu2x.bgmain;

	bgmain->blit(s, 0, 0);
}

bool Background::handleButtonPress(InputManager::Button button) {
	switch (button) {
		case InputManager::CANCEL:
			gmenu2x.showHelpPopup();
			return true;
		case InputManager::SETTINGS:
			gmenu2x.showSettings();
			return true;
		default:
			return false;
	}
}
