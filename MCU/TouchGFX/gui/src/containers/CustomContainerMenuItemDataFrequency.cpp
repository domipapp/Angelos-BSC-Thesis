#include <gui/containers/CustomContainerMenuItemDataFrequency.hpp>

CustomContainerMenuItemDataFrequency::CustomContainerMenuItemDataFrequency()
{

}

void CustomContainerMenuItemDataFrequency::initialize()
{
    CustomContainerMenuItemDataFrequencyBase::initialize();
}

void CustomContainerMenuItemDataFrequency::FlexButtonClicked()
{	uint16_t a = 123;
	Unicode::snprintf(textAreaBuffer, TEXTAREA_SIZE, "%u", a);
	textArea.invalidate();

}

void CustomContainerMenuItemDataFrequency::LoadParameter(const Unicode::UnicodeChar * string){

	Unicode::strncpy(textAreaBuffer, string, TEXTAREA_SIZE);
    TextAreaBackground.setAlpha(100);
    TextAreaBackground.invalidate();

}
