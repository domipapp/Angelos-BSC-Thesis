#ifndef CUSTOMCONTAINERMENUITEMWIFIPASSWORD_HPP
#define CUSTOMCONTAINERMENUITEMWIFIPASSWORD_HPP

#include <gui_generated/containers/CustomContainerMenuItemWifiPasswordBase.hpp>
#include <gui/common/CustomKeyboard.hpp>
class CustomContainerMenuItemWifiPassword : public CustomContainerMenuItemWifiPasswordBase
{
public:
    CustomContainerMenuItemWifiPassword();
    virtual ~CustomContainerMenuItemWifiPassword() {}

    virtual void initialize();
    virtual void FlexButtonClicked();
    virtual void LoadParameter(const Unicode::UnicodeChar * string);
    virtual void SetKeyboard(CustomKeyboard* keyboard);
protected:
    CustomKeyboard* keyboard;
};

#endif // CUSTOMCONTAINERMENUITEMWIFIPASSWORD_HPP
