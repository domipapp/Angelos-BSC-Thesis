#ifndef CUSTOMCONTAINERMENUITEMDATAFREQUENCY_HPP
#define CUSTOMCONTAINERMENUITEMDATAFREQUENCY_HPP

#include <gui_generated/containers/CustomContainerMenuItemDataFrequencyBase.hpp>

class CustomContainerMenuItemDataFrequency : public CustomContainerMenuItemDataFrequencyBase
{
public:
    CustomContainerMenuItemDataFrequency();
    virtual ~CustomContainerMenuItemDataFrequency() {}

    virtual void initialize();
    virtual void FlexButtonClicked();
protected:
};

#endif // CUSTOMCONTAINERMENUITEMDATAFREQUENCY_HPP
