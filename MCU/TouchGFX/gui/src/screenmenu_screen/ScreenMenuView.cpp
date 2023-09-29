#include <gui/screenmenu_screen/ScreenMenuView.hpp>
#include <simulator_main.h>

ScreenMenuView::ScreenMenuView()
{
	keyboard.setButtons(&buttonWithLabelKeyboardExit, &buttonWithLabelKeyboardSave);
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
	Unicode::strncpy(textAreaDataFrequencyBuffer, string, TEXTAREADATAFREQUENCY_SIZE);
	TextAreaBackgroundDataFrequency.setAlpha(100);
	TextAreaBackgroundDataFrequency.invalidate();

	Unicode::strncpy(string, IP_ADDRESS, 50);
	Unicode::strncpy(textAreaServerIpBuffer, string, TEXTAREASERVERIP_SIZE);
	TextAreaBackgroundServerIp.setAlpha(100);
	TextAreaBackgroundServerIp.invalidate();

	Unicode::strncpy(string, PORT, 50);
	Unicode::strncpy(textAreaServerPortBuffer, string, TEXTAREASERVERPORT_SIZE);
	TextAreaBackgroundServerPort.setAlpha(100);
	TextAreaBackgroundServerPort.invalidate();

	Unicode::strncpy(string, WIFI_PASS, 50);
	Unicode::strncpy(textAreaWifiPassBuffer, string, TEXTAREAWIFIPASS_SIZE);
	TextAreaBackgroundWifiPass.setAlpha(100);
	TextAreaBackgroundWifiPass.invalidate();

	Unicode::strncpy(string, WIFI_SSID, 50);
	Unicode::strncpy(textAreaWifiSsidBuffer, string, TEXTAREAWIFISSID_SIZE);
	TextAreaBackgroundWifiSsid.setAlpha(100);
	TextAreaBackgroundWifiSsid.invalidate();
}

void ScreenMenuView::buttonWithLabelKeyboardExitClicked(){
	keyboard.setVisible(false);
	keyboard.getExitButton()->setVisible(false);
	keyboard.getSaveButton()->setVisible(false);
	keyboard.invalidate();
	keyboard.getExitButton()->invalidate();
	keyboard.getSaveButton()->invalidate();
	keyboard.clearBuffer();
}

void ScreenMenuView::buttonWithLabelKeyboardSaveClicked(){

}

void ScreenMenuView::flexButtonServerPortClicked(){
	popUpKeyboard();
}

void ScreenMenuView::flexButtonServerIpClicked(){
	popUpKeyboard();
}

void ScreenMenuView::flexButtonWifiSsidClicked(){
	popUpKeyboard();
}

void ScreenMenuView::flexButtonWifiPassClicked(){
	popUpKeyboard();
}

void ScreenMenuView::flexButtonDataFrequencyClicked(){
	popUpKeyboard();
}

void ScreenMenuView::popUpKeyboard(){
	keyboard.setVisible(true);
	keyboard.getExitButton()->setVisible(true);
	keyboard.getExitButton()->invalidate();
	keyboard.getSaveButton()->setVisible(true);
	keyboard.getSaveButton()->invalidate();
	keyboard.invalidate();
}
