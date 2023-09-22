#ifndef CUSTOMCONTAINERMENUITEMSERVERPORT_HPP
#define CUSTOMCONTAINERMENUITEMSERVERPORT_HPP

#include <gui_generated/containers/CustomContainerMenuItemServerPortBase.hpp>

class CustomContainerMenuItemServerPort : public CustomContainerMenuItemServerPortBase
{
public:
    CustomContainerMenuItemServerPort();
    virtual ~CustomContainerMenuItemServerPort() {}

    virtual void initialize();
    virtual void FlexButtonClicked();
    virtual void LoadParameter(const Unicode::UnicodeChar * string);
protected:
};

#endif // CUSTOMCONTAINERMENUITEMSERVERPORT_HPP
