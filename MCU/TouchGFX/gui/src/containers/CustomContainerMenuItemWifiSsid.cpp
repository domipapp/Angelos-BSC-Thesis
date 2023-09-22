#include <gui/containers/CustomContainerMenuItemWifiSsid.hpp>

CustomContainerMenuItemWifiSsid::CustomContainerMenuItemWifiSsid()
{

}

void CustomContainerMenuItemWifiSsid::initialize()
{
    CustomContainerMenuItemWifiSsidBase::initialize();
}

void CustomContainerMenuItemWifiSsid::FlexButtonClicked()
{	uint16_t a = 123;
	Unicode::snprintf(textAreaBuffer, TEXTAREA_SIZE, "%u", a);
	textArea.invalidate();
}

void CustomContainerMenuItemWifiSsid::LoadParameter(const Unicode::UnicodeChar * string){

	Unicode::strncpy(textAreaBuffer, string, TEXTAREA_SIZE);
    TextAreaBackground.setAlpha(100);
    TextAreaBackground.invalidate();

}
