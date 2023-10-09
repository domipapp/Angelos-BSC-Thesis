#include <gui/screenmenu_screen/ScreenMenuView.hpp>

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

    // If transitioning from home screen reload all previous settings
    osStatus_t status = osSemaphoreAcquire(semaphoreTransitionFromHomeHandle, 0);

    if (status == osOK) {
        // The semaphore was acquired successfully
    	Unicode::itoa(data_frequency, textAreaDataFrequencyBuffer, TEXTAREADATAFREQUENCY_SIZE, 10);
    	TextAreaBackgroundDataFrequency.setAlpha(100);
    	TextAreaBackgroundDataFrequency.invalidate();
    	;
    	Unicode::strncpy(textAreaServerIpBuffer, server_ip, TEXTAREASERVERIP_SIZE);
    	TextAreaBackgroundServerIp.setAlpha(100);
    	TextAreaBackgroundServerIp.invalidate();

    	Unicode::strncpy(textAreaServerPortBuffer, server_port, TEXTAREASERVERPORT_SIZE);
    	TextAreaBackgroundServerPort.setAlpha(100);
    	TextAreaBackgroundServerPort.invalidate();

    	Unicode::strncpy(textAreaWifiPassBuffer, wifi_pass, TEXTAREAWIFIPASS_SIZE);
    	TextAreaBackgroundWifiPass.setAlpha(100);
    	TextAreaBackgroundWifiPass.invalidate();

    	Unicode::strncpy(textAreaWifiSsidBuffer, wifi_ssid, TEXTAREAWIFISSID_SIZE);
    	TextAreaBackgroundWifiSsid.setAlpha(100);
    	TextAreaBackgroundWifiSsid.invalidate();
    }
}

void ScreenMenuView::tearDownScreen()
{
    ScreenMenuViewBase::tearDownScreen();
}

void ScreenMenuView::LoadDefaultParameters()
{
	Unicode::UnicodeChar string[50];

	Unicode::strncpy(string, DATA_FREQUENCY, 50);
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
	// Copy content into keyboard so it can be seen
	keyboard.setBufferCopy(CallingButtonBuffer);
}

void ScreenMenuView::flexButtonServerIpClicked(){
	setVisibilityKeyboard(true);
	CallingButtonBuffer = textAreaServerIpBuffer;
	CALLINGBUTTONBUFFER_SIZE = TEXTAREASERVERIP_SIZE;
	CallingTextAreaBackground = &TextAreaBackgroundServerIp;
	// Copy content into keyboard so it can be seen
	keyboard.setBufferCopy(CallingButtonBuffer);
}

void ScreenMenuView::flexButtonWifiSsidClicked(){
	setVisibilityKeyboard(true);
	CallingButtonBuffer = textAreaWifiSsidBuffer;
	CALLINGBUTTONBUFFER_SIZE = TEXTAREAWIFISSID_SIZE;
	CallingTextAreaBackground = &TextAreaBackgroundWifiSsid;
	// Copy content into keyboard so it can be seen
	keyboard.setBufferCopy(CallingButtonBuffer);
}

void ScreenMenuView::flexButtonWifiPassClicked(){
	setVisibilityKeyboard(true);
	CallingButtonBuffer = textAreaWifiPassBuffer;
	CALLINGBUTTONBUFFER_SIZE = TEXTAREAWIFIPASS_SIZE;
	CallingTextAreaBackground = &TextAreaBackgroundWifiPass;
	// Copy content into keyboard so it can be seen
	keyboard.setBufferCopy(CallingButtonBuffer);
}

void ScreenMenuView::flexButtonDataFrequencyClicked(){
	setVisibilityKeyboard(true);
	CallingButtonBuffer = textAreaDataFrequencyBuffer;
	CALLINGBUTTONBUFFER_SIZE = TEXTAREADATAFREQUENCY_SIZE;
	CallingTextAreaBackground = &TextAreaBackgroundDataFrequency;
	// Copy content into keyboard so it can be seen
	keyboard.setBufferCopy(CallingButtonBuffer);
}


void ScreenMenuView::setVisibilityKeyboard(bool state){
	keyboard.setVisible(state);
	keyboard.getExitButton()->setVisible(state);
	keyboard.getExitButton()->invalidate();
	keyboard.getSaveButton()->setVisible(state);
	keyboard.getSaveButton()->invalidate();
	keyboard.invalidate();
}

void ScreenMenuView::buttonConnectClicked(){
    //Show textAreaConnecting
    textAreaConnecting.setVisible(true);
    textAreaConnecting.invalidate();
    // Force an immediate redraw of the invalidated area on the screen
	//Application::getInstance()->invalidateArea(textAreaConnecting.getAbsoluteRect());
    Rect rectToRedraw = textAreaConnecting.getAbsoluteRect();
    Application::getInstance()->requestRedraw();
    OSWrappers::giveFrameBufferSemaphore();
    Application::getInstance()->requestRedraw();

	char tmp[50];
	Unicode::toUTF8(textAreaServerIpBuffer, (uint8_t*)tmp, 50);
	strncpy(server_ip, tmp, 50);

	Unicode::toUTF8(textAreaServerPortBuffer, (uint8_t*)tmp, 50);
	strncpy(server_port, tmp, 50);

	Unicode::toUTF8(textAreaWifiSsidBuffer, (uint8_t*)tmp, 50);
	strncpy(wifi_ssid, tmp, 50);

	Unicode::toUTF8(textAreaWifiPassBuffer, (uint8_t*)tmp, 50);
	strncpy(wifi_pass, tmp, 50);

	data_frequency = Unicode::atoi(textAreaDataFrequencyBuffer);

	osEventFlagsSet(eventConfigurationsLoadedHandle, EVENT_FLAG_ESP_WIFI_CONNECT);
	osEventFlagsSet(eventConfigurationsLoadedHandle, EVENT_FLAG_ESP_SERVER_CONNECT);

}


void ScreenMenuView::waitForConnection()
{
	osEventFlagsWait(eventESPServerConnectedHandle, EVENT_FLAG_ESP_SERVER_CONNECTED,  osFlagsWaitAny, osWaitForever);
	// Set text to green to show successful connection. Touchgfx will wait according to WaitAfterConnection interaction
	textAreaConnecting.setColor(touchgfx::Color::getColorFromRGB(0, 255, 0));
	textAreaConnecting.invalidate();
    // Force an immediate redraw of the invalidated area on the screen
	Application::getInstance()->invalidateArea(textAreaConnecting.getAbsoluteRect());
}
