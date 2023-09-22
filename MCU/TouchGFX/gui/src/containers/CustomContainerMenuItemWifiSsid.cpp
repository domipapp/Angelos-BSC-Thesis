#include <gui/containers/CustomContainerMenuItemWifiSsid.hpp>

CustomContainerMenuItemWifiSsid::CustomContainerMenuItemWifiSsid()
{

}

void CustomContainerMenuItemWifiSsid::initialize()
{
    CustomContainerMenuItemWifiSsidBase::initialize();
}

void CustomContainerMenuItemWifiSsid::FlexButtonClicked()
{	
    keyboard->setVisible(true);
	keyboard->getExitButton()->setVisible(true);
	keyboard->getExitButton()->invalidate();
	keyboard->getSaveButton()->setVisible(true);
	keyboard->getSaveButton()->invalidate();
	keyboard->invalidate();
}

void CustomContainerMenuItemWifiSsid::LoadParameter(const Unicode::UnicodeChar * string){

	Unicode::strncpy(textAreaBuffer, string, TEXTAREA_SIZE);
    TextAreaBackground.setAlpha(100);
    TextAreaBackground.invalidate();

}

void CustomContainerMenuItemWifiSsid::SetKeyboard(CustomKeyboard* keyboard){
	this->keyboard = keyboard;
}
