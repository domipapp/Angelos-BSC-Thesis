#ifndef CUSTOMCONTAINERMENUITEMWIFISSID_HPP
#define CUSTOMCONTAINERMENUITEMWIFISSID_HPP

#include <gui_generated/containers/CustomContainerMenuItemWifiSsidBase.hpp>
#include <gui/common/CustomKeyboard.hpp>
class CustomContainerMenuItemWifiSsid : public CustomContainerMenuItemWifiSsidBase
{
public:
    CustomContainerMenuItemWifiSsid();
    virtual ~CustomContainerMenuItemWifiSsid() {}

    virtual void initialize();
    virtual void FlexButtonClicked();
    virtual void LoadParameter(const Unicode::UnicodeChar * string);
    virtual void SetKeyboard(CustomKeyboard* keyboard);
protected:
    CustomKeyboard* keyboard;
};

#endif // CUSTOMCONTAINERMENUITEMWIFISSID_HPP
