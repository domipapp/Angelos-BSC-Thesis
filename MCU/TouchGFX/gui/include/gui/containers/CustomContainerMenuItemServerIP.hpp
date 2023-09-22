#ifndef CUSTOMCONTAINERMENUITEMSERVERIP_HPP
#define CUSTOMCONTAINERMENUITEMSERVERIP_HPP

#include <gui_generated/containers/CustomContainerMenuItemServerIPBase.hpp>
#include <gui/common/CustomKeyboard.hpp>

class CustomContainerMenuItemServerIP : public CustomContainerMenuItemServerIPBase
{
public:
    CustomContainerMenuItemServerIP();
    virtual ~CustomContainerMenuItemServerIP() {}

    virtual void initialize();
    virtual void FlexButtonClicked();
    virtual void LoadParameter(const Unicode::UnicodeChar * string);
    virtual void SetKeyboard(CustomKeyboard* keyboard);
protected:
    CustomKeyboard* keyboard;
};

#endif // CUSTOMCONTAINERMENUITEMSERVERIP_HPP
