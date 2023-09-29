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
	setVisibilityKeyboard(false);
	keyboard.clearBuffer();
}

void ScreenMenuView::buttonWithLabelKeyboardSaveClicked(){
	if (CallingButtonBuffer == nullptr || CALLINGBUTTONBUFFER_SIZE == 0 || CallingTextAreaBackground == nullptr)
		return;
	Unicode::strncpy(CallingButtonBuffer, keyboard.getBuffer(), CALLINGBUTTONBUFFER_SIZE);
	buttonWithLabelKeyboardExitClicked();
	CallingTextAreaBackground->setAlpha(100);
	CallingTextAreaBackground->invalidate();
}

void ScreenMenuView::flexButtonServerPortClicked(){
	setVisibilityKeyboard(true);
	CallingButtonBuffer = textAreaServerPortBuffer;
	CALLINGBUTTONBUFFER_SIZE = TEXTAREASERVERPORT_SIZE;
	CallingTextAreaBackground = &TextAreaBackgroundServerPort;
}

void ScreenMenuView::flexButtonServerIpClicked(){
	setVisibilityKeyboard(true);
	CallingButtonBuffer = textAreaServerIpBuffer;
	CALLINGBUTTONBUFFER_SIZE = TEXTAREASERVERIP_SIZE;
	CallingTextAreaBackground = &TextAreaBackgroundServerIp;
}

void ScreenMenuView::flexButtonWifiSsidClicked(){
	setVisibilityKeyboard(true);
	CallingButtonBuffer = textAreaWifiSsidBuffer;
	CALLINGBUTTONBUFFER_SIZE = TEXTAREAWIFISSID_SIZE;
	CallingTextAreaBackground = &TextAreaBackgroundWifiSsid;
}

void ScreenMenuView::flexButtonWifiPassClicked(){
	setVisibilityKeyboard(true);
	CallingButtonBuffer = textAreaWifiPassBuffer;
	CALLINGBUTTONBUFFER_SIZE = TEXTAREAWIFIPASS_SIZE;
	CallingTextAreaBackground = &TextAreaBackgroundWifiPass;
}

void ScreenMenuView::flexButtonDataFrequencyClicked(){
	setVisibilityKeyboard(true);
	CallingButtonBuffer = textAreaDataFrequencyBuffer;
	CALLINGBUTTONBUFFER_SIZE = TEXTAREADATAFREQUENCY_SIZE;
	CallingTextAreaBackground = &TextAreaBackgroundDataFrequency;
}


void ScreenMenuView::setVisibilityKeyboard(bool state){
	keyboard.setVisible(state);
	keyboard.getExitButton()->setVisible(state);
	keyboard.getExitButton()->invalidate();
	keyboard.getSaveButton()->setVisible(state);
	keyboard.getSaveButton()->invalidate();
	keyboard.invalidate();
}
