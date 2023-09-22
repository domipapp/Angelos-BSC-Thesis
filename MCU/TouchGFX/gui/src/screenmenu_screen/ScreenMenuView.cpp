#include <gui/screenmenu_screen/ScreenMenuView.hpp>
#include <simulator_main.h>

ScreenMenuView::ScreenMenuView()
{
	keyboard.setButtons(&buttonWithLabelKeyboardExit, &buttonWithLabelKeyboardSave);
	customContainerMenuItemServerPort.SetKeyboard(&keyboard);
	keyboard.setPosition(80, 16, 320, 240);
	add(keyboard);
	keyboard.setVisible(false);
	keyboard.invalidate();
}

void ScreenMenuView::setupScreen()
{
    ScreenMenuViewBase::setupScreen();
}

void ScreenMenuView::tearDownScreen()
{
    ScreenMenuViewBase::tearDownScreen();
}

void ScreenMenuView::LoadDefaultParameters()
{
	Unicode::UnicodeChar string[50];

	Unicode::strncpy(string, "AAA", 50);
	customContainerMenuItemDataFrequency.LoadParameter(string);

	Unicode::strncpy(string, IP_ADDRESS, 50);
	customContainerMenuItemServerIP.LoadParameter(string);

	Unicode::strncpy(string, PORT, 50);
	customContainerMenuItemServerPort.LoadParameter(string);

	Unicode::strncpy(string, WIFI_PASS, 50);
	customContainerMenuItemWifiPassword.LoadParameter(string);

	Unicode::strncpy(string, WIFI_SSID, 50);
	customContainerMenuItemWifiSsid.LoadParameter(string);
}
