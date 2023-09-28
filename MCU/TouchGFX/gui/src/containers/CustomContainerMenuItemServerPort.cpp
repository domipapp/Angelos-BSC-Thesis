#include <gui/containers/CustomContainerMenuItemServerPort.hpp>

CustomContainerMenuItemServerPort::CustomContainerMenuItemServerPort():keyboard(nullptr)
{
}

void CustomContainerMenuItemServerPort::initialize()
{
    CustomContainerMenuItemServerPortBase::initialize();
}


void CustomContainerMenuItemServerPort::FlexButtonClicked()
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

void CustomContainerMenuItemServerPort::LoadParameter(const Unicode::UnicodeChar * string){

	Unicode::strncpy(textAreaBuffer, string, TEXTAREA_SIZE);
    TextAreaBackground.setAlpha(100);
    TextAreaBackground.invalidate();

}

void CustomContainerMenuItemServerPort::SetKeyboard(CustomKeyboard* keyboard){
	this->keyboard = keyboard;
}


