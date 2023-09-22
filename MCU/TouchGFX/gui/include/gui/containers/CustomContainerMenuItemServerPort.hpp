#ifndef CUSTOMCONTAINERMENUITEMSERVERPORT_HPP
#define CUSTOMCONTAINERMENUITEMSERVERPORT_HPP

#include <gui_generated/containers/CustomContainerMenuItemServerPortBase.hpp>
#include <gui/common/CustomKeyboard.hpp>
class CustomContainerMenuItemServerPort : public CustomContainerMenuItemServerPortBase
{
public:
    CustomContainerMenuItemServerPort();
    virtual ~CustomContainerMenuItemServerPort() {}

    virtual void initialize();
    virtual void FlexButtonClicked();
    virtual void LoadParameter(const Unicode::UnicodeChar * string);
    virtual void SetKeyboard(CustomKeyboard* keyboard);
protected:
    CustomKeyboard* keyboard;
};

#endif // CUSTOMCONTAINERMENUITEMSERVERPORT_HPP
