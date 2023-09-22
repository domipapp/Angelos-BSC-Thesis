#ifndef CUSTOMCONTAINERMENUITEMWIFISSID_HPP
#define CUSTOMCONTAINERMENUITEMWIFISSID_HPP

#include <gui_generated/containers/CustomContainerMenuItemWifiSsidBase.hpp>

class CustomContainerMenuItemWifiSsid : public CustomContainerMenuItemWifiSsidBase
{
public:
    CustomContainerMenuItemWifiSsid();
    virtual ~CustomContainerMenuItemWifiSsid() {}

    virtual void initialize();
    virtual void FlexButtonClicked();
    virtual void LoadParameter(const Unicode::UnicodeChar * string);
protected:
};

#endif // CUSTOMCONTAINERMENUITEMWIFISSID_HPP
