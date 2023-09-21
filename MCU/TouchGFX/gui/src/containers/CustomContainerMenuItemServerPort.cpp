#include <gui/containers/CustomContainerMenuItemServerPort.hpp>

CustomContainerMenuItemServerPort::CustomContainerMenuItemServerPort()
{

}

void CustomContainerMenuItemServerPort::initialize()
{
    CustomContainerMenuItemServerPortBase::initialize();
}


void CustomContainerMenuItemServerPort::FlexButtonClicked()
{	uint16_t a = 123;
	Unicode::snprintf(textAreaBuffer, TEXTAREA_SIZE, "%u", a);
	textArea.invalidate();
}
