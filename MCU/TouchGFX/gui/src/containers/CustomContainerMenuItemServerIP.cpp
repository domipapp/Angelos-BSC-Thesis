#include <gui/containers/CustomContainerMenuItemServerIP.hpp>

CustomContainerMenuItemServerIP::CustomContainerMenuItemServerIP()
{

}

void CustomContainerMenuItemServerIP::initialize()
{
    CustomContainerMenuItemServerIPBase::initialize();
}

void CustomContainerMenuItemServerIP::FlexButtonClicked()
{	uint16_t a = 123;
	Unicode::snprintf(textAreaBuffer, TEXTAREA_SIZE, "%u", a);
	textArea.invalidate();
}

void CustomContainerMenuItemServerIP::LoadParameter(const Unicode::UnicodeChar * string){

	Unicode::strncpy(textAreaBuffer, string, TEXTAREA_SIZE);
    TextAreaBackground.setAlpha(100);
    TextAreaBackground.invalidate();

}
