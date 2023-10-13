#include <gui/screenmenu_screen/ScreenMenuView.hpp>

ScreenMenuView::ScreenMenuView()
{
	// Set up keyboard and hide it
	keyboard.setButtons(&buttonWithLabelKeyboardExit, &buttonWithLabelKeyboardSave);
	keyboard.setPosition(80, 16, 320, 240);
	add(keyboard);
	keyboard.setVisible(false);
	keyboard.invalidate();
}

void ScreenMenuView::setupScreen()
{
    ScreenMenuViewBase::setupScreen();

    // If transitioning from home screen reload all previous settings and show Home button instead of Connect
    osStatus_t status = osSemaphoreAcquire(semaphoreTransitionFromHomeByButtonSettingsHandle, 0);

    if (status == osOK) {
        // The semaphore was acquired successfully
		loadPreviousSettings();

    	buttonHome.setVisible(true);
    	buttonConnect.setVisible(false);
    	buttonConnect.invalidate();
		return;
    }
}

void ScreenMenuView::tearDownScreen()
{
    ScreenMenuViewBase::tearDownScreen();
}

void ScreenMenuView::LoadDefaultParameters()
{

	setText(DATA_FREQUENCY, textAreaDataFrequencyBuffer, TEXTAREADATAFREQUENCY_SIZE, &textAreaDataFrequency, &TextAreaBackgroundDataFrequency, &scrollableContainerDataFrequency);

	setText(IP_ADDRESS, textAreaServerIpBuffer, TEXTAREASERVERIP_SIZE, &textAreaServerIp, &TextAreaBackgroundServerIp, &scrollableContainerServerIp);

	setText(PORT, textAreaServerPortBuffer, TEXTAREASERVERPORT_SIZE, &textAreaServerPort, &TextAreaBackgroundServerPort, &scrollableContainerServerPort);

	setText(WIFI_PASS, textAreaWifiPassBuffer, TEXTAREAWIFIPASS_SIZE, &textAreaWifiPass, &TextAreaBackgroundWifiPass, &scrollableContainerWifiPass);

	setText(WIFI_SSID, textAreaWifiSsidBuffer, TEXTAREAWIFISSID_SIZE, &textAreaWifiSsid, &TextAreaBackgroundWifiSsid, &scrollableContainerWifiSsid);

}

void ScreenMenuView::buttonWithLabelKeyboardExitClicked(){
	setVisibilityKeyboard(false);
	keyboard.clearBuffer();
}

void ScreenMenuView::buttonWithLabelKeyboardSaveClicked(){
	if (CallingButtonBuffer == nullptr || CALLINGBUTTONBUFFER_SIZE == 0 || CallingTextAreaBackground == nullptr || CallingTextArea == nullptr || CallingScrollableContainer == nullptr)
		return;

	// Set data
	setText(keyboard.getBuffer(), CallingButtonBuffer, CALLINGBUTTONBUFFER_SIZE, CallingTextArea, CallingTextAreaBackground, CallingScrollableContainer);

	// Clear buffer so on next reopen of the keyboard there is no data.
	buttonWithLabelKeyboardExitClicked();

	// Scroll to the leftmost position (by 50 so it doesn't take long)
	while(CallingScrollableContainer->doScroll(50, 0) != false){}
	CallingScrollableContainer->invalidate();

}

void ScreenMenuView::flexButtonServerPortClicked(){
	setVisibilityKeyboard(true);
	// Set parameters of calling types
	CallingButtonBuffer = textAreaServerPortBuffer;
	CALLINGBUTTONBUFFER_SIZE = TEXTAREASERVERPORT_SIZE;
	CallingTextAreaBackground = &TextAreaBackgroundServerPort;
	CallingTextArea = &textAreaServerPort;
	CallingScrollableContainer = &scrollableContainerServerPort;
	// Copy content into keyboard so it can be seen
	keyboard.setBufferCopy(CallingButtonBuffer);
}

void ScreenMenuView::flexButtonServerIpClicked(){
	setVisibilityKeyboard(true);
	// Set parameters of calling types
	CallingButtonBuffer = textAreaServerIpBuffer;
	CALLINGBUTTONBUFFER_SIZE = TEXTAREASERVERIP_SIZE;
	CallingTextAreaBackground = &TextAreaBackgroundServerIp;
	CallingTextArea = &textAreaServerIp;
	CallingScrollableContainer = &scrollableContainerServerIp;
	// Copy content into keyboard so it can be seen
	keyboard.setBufferCopy(CallingButtonBuffer);
}

void ScreenMenuView::flexButtonWifiSsidClicked(){
	setVisibilityKeyboard(true);
	// Set parameters of calling types
	CallingButtonBuffer = textAreaWifiSsidBuffer;
	CALLINGBUTTONBUFFER_SIZE = TEXTAREAWIFISSID_SIZE;
	CallingTextAreaBackground = &TextAreaBackgroundWifiSsid;
	CallingTextArea = &textAreaWifiSsid;
	CallingScrollableContainer = &scrollableContainerWifiSsid;
	// Copy content into keyboard so it can be seen
	keyboard.setBufferCopy(CallingButtonBuffer);
}

void ScreenMenuView::flexButtonWifiPassClicked(){
	setVisibilityKeyboard(true);
	// Set parameters of calling types
	CallingButtonBuffer = textAreaWifiPassBuffer;
	CALLINGBUTTONBUFFER_SIZE = TEXTAREAWIFIPASS_SIZE;
	CallingTextAreaBackground = &TextAreaBackgroundWifiPass;
	CallingTextArea = &textAreaWifiPass;
	CallingScrollableContainer = &scrollableContainerWifiPass;
	// Copy content into keyboard so it can be seen
	keyboard.setBufferCopy(CallingButtonBuffer);
}

void ScreenMenuView::flexButtonDataFrequencyClicked(){
	setVisibilityKeyboard(true);
	// Set parameters of calling types
	CallingButtonBuffer = textAreaDataFrequencyBuffer;
	CALLINGBUTTONBUFFER_SIZE = TEXTAREADATAFREQUENCY_SIZE;
	CallingTextAreaBackground = &TextAreaBackgroundDataFrequency;
	CallingTextArea = &textAreaDataFrequency;
	CallingScrollableContainer = &scrollableContainerDataFrequency;
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

    // Save data into main.c
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

	// Signal for connection
	osEventFlagsSet(eventConfigurationsLoadedHandle, EVENT_FLAG_ESP_WIFI_CONNECT);
	osEventFlagsSet(eventConfigurationsLoadedHandle, EVENT_FLAG_ESP_SERVER_CONNECT);

}


void ScreenMenuView::waitForConnection()
{
	osEventFlagsWait(eventESPServerConnectedHandle, EVENT_FLAG_ESP_SERVER_CONNECTED,  osFlagsWaitAny, osWaitForever);
	// Set text to green to show successful connection. Touchgfx will wait according to WaitAfterConnection interaction
	textAreaConnecting.setColor(touchgfx::Color::getColorFromRGB(0, 255, 0));
	textAreaConnecting.invalidate();
}

void ScreenMenuView::setText(const char* data, Unicode::UnicodeChar * textAreaBuffer,  const uint16_t TEXTAREA_SIZE, touchgfx::TextAreaWithOneWildcard* textArea, touchgfx::Box* TextAreaBackground, touchgfx::ScrollableContainer* ScrollableContainer){
	static Unicode::UnicodeChar string[50];
	static uint16_t textSize;

	// Copy data
	Unicode::strncpy(string, data, 50);
	Unicode::strncpy(textAreaBuffer, string, TEXTAREA_SIZE);

	// Resize text and background
	textArea->resizeToCurrentText();
	textSize = textArea->getTextWidth();
	if(textSize < MIN_CALLINGTEXTAREABACKGROUND_WIDTH)
		TextAreaBackground->setWidth(MIN_CALLINGTEXTAREABACKGROUND_WIDTH);
	else
		TextAreaBackground->setWidth(textSize);

	// Set "color"
	TextAreaBackground->setAlpha(100);

	TextAreaBackground->invalidate();

	// Scroll to the leftmost position (by 50 so it doesn't take long)
	while(ScrollableContainer->doScroll(50, 0) != false){}
	ScrollableContainer->invalidate();

}

void ScreenMenuView::setText(const Unicode::UnicodeChar * data, Unicode::UnicodeChar * textAreaBuffer,  const uint16_t TEXTAREA_SIZE, touchgfx::TextAreaWithOneWildcard* textArea, touchgfx::Box* TextAreaBackground, touchgfx::ScrollableContainer* ScrollableContainer){
	static uint16_t textSize;

	// Copy data
	Unicode::strncpy(textAreaBuffer, data, TEXTAREA_SIZE);

	// Resize text and background
	textArea->resizeToCurrentText();
	textSize = textArea->getTextWidth();
	if(textSize < MIN_CALLINGTEXTAREABACKGROUND_WIDTH)
		TextAreaBackground->setWidth(MIN_CALLINGTEXTAREABACKGROUND_WIDTH);
	else
		TextAreaBackground->setWidth(textSize);
	// Set "color"
	TextAreaBackground->setAlpha(100);

	TextAreaBackground->invalidate();

	// Scroll to the leftmost position (by 50 so it doesn't take long)
	while(ScrollableContainer->doScroll(50, 0) != false){}
	ScrollableContainer->invalidate();
}


void ScreenMenuView::setText(const uint16_t data, Unicode::UnicodeChar * textAreaBuffer,  const uint16_t TEXTAREA_SIZE, touchgfx::TextAreaWithOneWildcard* textArea, touchgfx::Box* TextAreaBackground, touchgfx::ScrollableContainer* ScrollableContainer){
	static uint16_t textSize;

	// Copy data
	Unicode::itoa(data, textAreaBuffer, TEXTAREA_SIZE, 10);

	// Resize text and background
	textArea->resizeToCurrentText();
	textSize = textArea->getTextWidth();
	if(textSize < MIN_CALLINGTEXTAREABACKGROUND_WIDTH)
		TextAreaBackground->setWidth(MIN_CALLINGTEXTAREABACKGROUND_WIDTH);
	else
		TextAreaBackground->setWidth(textSize);

	// Set "color"
	TextAreaBackground->setAlpha(100);

	TextAreaBackground->invalidate();

	// Scroll to the leftmost position (by 50 so it doesn't take long)
	while(ScrollableContainer->doScroll(50, 0) != false){}
	ScrollableContainer->invalidate();
}

void ScreenMenuView::loadPreviousSettings(){
	setText(data_frequency, textAreaDataFrequencyBuffer, TEXTAREADATAFREQUENCY_SIZE, &textAreaDataFrequency, &TextAreaBackgroundDataFrequency, &scrollableContainerDataFrequency);

	setText(server_ip, textAreaServerIpBuffer, TEXTAREASERVERIP_SIZE, &textAreaServerIp, &TextAreaBackgroundServerIp, &scrollableContainerServerIp);

	setText(server_port, textAreaServerPortBuffer, TEXTAREASERVERPORT_SIZE, &textAreaServerPort, &TextAreaBackgroundServerPort, &scrollableContainerServerPort);

	setText(wifi_pass, textAreaWifiPassBuffer, TEXTAREAWIFIPASS_SIZE, &textAreaWifiPass, &TextAreaBackgroundWifiPass, &scrollableContainerWifiPass);

	setText(wifi_ssid, textAreaWifiSsidBuffer, TEXTAREAWIFISSID_SIZE, &textAreaWifiSsid, &TextAreaBackgroundWifiSsid, &scrollableContainerWifiSsid);
}