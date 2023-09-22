#ifndef CUSTOMCONTAINERMENUITEMSERVERIP_HPP
#define CUSTOMCONTAINERMENUITEMSERVERIP_HPP

#include <gui_generated/containers/CustomContainerMenuItemServerIPBase.hpp>

class CustomContainerMenuItemServerIP : public CustomContainerMenuItemServerIPBase
{
public:
    CustomContainerMenuItemServerIP();
    virtual ~CustomContainerMenuItemServerIP() {}

    virtual void initialize();
    virtual void FlexButtonClicked();
    virtual void LoadParameter(const Unicode::UnicodeChar * string);
protected:
};

#endif // CUSTOMCONTAINERMENUITEMSERVERIP_HPP
