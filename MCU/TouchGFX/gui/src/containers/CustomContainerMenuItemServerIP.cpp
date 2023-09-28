#include <gui/containers/CustomContainerMenuItemServerIP.hpp>

CustomContainerMenuItemServerIP::CustomContainerMenuItemServerIP()
{

}

void CustomContainerMenuItemServerIP::initialize()
{
    CustomContainerMenuItemServerIPBase::initialize();
}

void CustomContainerMenuItemServerIP::FlexButtonClicked()
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

void CustomContainerMenuItemServerIP::LoadParameter(const Unicode::UnicodeChar * string){

	Unicode::strncpy(textAreaBuffer, string, TEXTAREA_SIZE);
    TextAreaBackground.setAlpha(100);
    TextAreaBackground.invalidate();

}

void CustomContainerMenuItemServerIP::SetKeyboard(CustomKeyboard* keyboard){
	this->keyboard = keyboard;
}

