#ifndef CUSTOMCONTAINERMENUITEMWIFIPASSWORD_HPP
#define CUSTOMCONTAINERMENUITEMWIFIPASSWORD_HPP

#include <gui_generated/containers/CustomContainerMenuItemWifiPasswordBase.hpp>

class CustomContainerMenuItemWifiPassword : public CustomContainerMenuItemWifiPasswordBase
{
public:
    CustomContainerMenuItemWifiPassword();
    virtual ~CustomContainerMenuItemWifiPassword() {}

    virtual void initialize();
    virtual void FlexButtonClicked();
protected:
};

#endif // CUSTOMCONTAINERMENUITEMWIFIPASSWORD_HPP
