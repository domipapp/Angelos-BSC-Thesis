#include <gui/containers/CustomContainerMenuItemWifiPassword.hpp>

CustomContainerMenuItemWifiPassword::CustomContainerMenuItemWifiPassword()
{

}

void CustomContainerMenuItemWifiPassword::initialize()
{
    CustomContainerMenuItemWifiPasswordBase::initialize();
}

void CustomContainerMenuItemWifiPassword::FlexButtonClicked()
{
	keyboard->setVisible(true);
	keyboard->getExitButton()->setVisible(true);
	keyboard->getExitButton()->invalidate();
	keyboard->getSaveButton()->setVisible(true);
	keyboard->getSaveButton()->invalidate();
	// Set, so keyboard knows where to save buffer
	keyboard->setCallingContainer(this);
	keyboard->invalidate();
}


void CustomContainerMenuItemWifiPassword::LoadParameter(const Unicode::UnicodeChar * string){

	Unicode::strncpy(textAreaBuffer, string, TEXTAREA_SIZE);
    TextAreaBackground.setAlpha(100);
    TextAreaBackground.invalidate();

}

void CustomContainerMenuItemWifiPassword::SetKeyboard(CustomKeyboard* keyboard){
	this->keyboard = keyboard;
}
