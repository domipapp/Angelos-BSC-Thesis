#ifndef CUSTOMCONTAINERMENUITEMDATAFREQUENCY_HPP
#define CUSTOMCONTAINERMENUITEMDATAFREQUENCY_HPP

#include <gui_generated/containers/CustomContainerMenuItemDataFrequencyBase.hpp>
#include <gui/common/CustomKeyboard.hpp>
class CustomContainerMenuItemDataFrequency : public CustomContainerMenuItemDataFrequencyBase
{
public:
    CustomContainerMenuItemDataFrequency();
    virtual ~CustomContainerMenuItemDataFrequency() {}

    virtual void initialize();
    virtual void FlexButtonClicked();
    virtual void LoadParameter(const Unicode::UnicodeChar * string);
    virtual void SetKeyboard(CustomKeyboard* keyboard);
protected:
    CustomKeyboard* keyboard;
};

#endif // CUSTOMCONTAINERMENUITEMDATAFREQUENCY_HPP
