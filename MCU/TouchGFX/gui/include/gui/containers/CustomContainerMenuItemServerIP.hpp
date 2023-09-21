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
protected:
};

#endif // CUSTOMCONTAINERMENUITEMSERVERIP_HPP
