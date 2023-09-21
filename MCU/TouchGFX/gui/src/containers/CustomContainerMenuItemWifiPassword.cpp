#include <gui/containers/CustomContainerMenuItemWifiPassword.hpp>

CustomContainerMenuItemWifiPassword::CustomContainerMenuItemWifiPassword()
{

}

void CustomContainerMenuItemWifiPassword::initialize()
{
    CustomContainerMenuItemWifiPasswordBase::initialize();
}

void CustomContainerMenuItemWifiPassword::FlexButtonClicked()
{	uint16_t a = 123;
	Unicode::snprintf(textAreaBuffer, TEXTAREA_SIZE, "%u", a);
	textArea.invalidate();
}
