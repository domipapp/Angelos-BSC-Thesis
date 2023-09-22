#include <gui/containers/CustomContainerMenuItemDataFrequency.hpp>

CustomContainerMenuItemDataFrequency::CustomContainerMenuItemDataFrequency()
{

}

void CustomContainerMenuItemDataFrequency::initialize()
{
    CustomContainerMenuItemDataFrequencyBase::initialize();
}

void CustomContainerMenuItemDataFrequency::FlexButtonClicked()
{
	keyboard->setVisible(true);
	keyboard->getExitButton()->setVisible(true);
	keyboard->getExitButton()->invalidate();
	keyboard->getSaveButton()->setVisible(true);
	keyboard->getSaveButton()->invalidate();
	keyboard->invalidate();
}

void CustomContainerMenuItemDataFrequency::LoadParameter(const Unicode::UnicodeChar * string){

	Unicode::strncpy(textAreaBuffer, string, TEXTAREA_SIZE);
    TextAreaBackground.setAlpha(100);
    TextAreaBackground.invalidate();

}

void CustomContainerMenuItemDataFrequency::SetKeyboard(CustomKeyboard* keyboard){
	this->keyboard = keyboard;
}
