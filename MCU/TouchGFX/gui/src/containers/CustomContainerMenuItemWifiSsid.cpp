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
