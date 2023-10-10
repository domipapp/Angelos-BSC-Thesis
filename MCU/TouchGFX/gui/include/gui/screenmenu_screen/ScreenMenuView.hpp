#ifndef SCREENMENUVIEW_HPP
#define SCREENMENUVIEW_HPP

#include <gui_generated/screenmenu_screen/ScreenMenuViewBase.hpp>
#include <gui/screenmenu_screen/ScreenMenuPresenter.hpp>
#include <gui/common/CustomKeyboard.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/hal/OSWrappers.hpp>
#include <string.h>
#include "main.h"

class ScreenMenuView : public ScreenMenuViewBase
{
public:
    ScreenMenuView();
    virtual ~ScreenMenuView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void LoadDefaultParameters();
    virtual void buttonWithLabelKeyboardExitClicked();
    virtual void buttonWithLabelKeyboardSaveClicked();
    virtual void flexButtonServerPortClicked();
    virtual void flexButtonServerIpClicked();
    virtual void flexButtonWifiSsidClicked();
    virtual void flexButtonWifiPassClicked();
    virtual void flexButtonDataFrequencyClicked();
    void setVisibilityKeyboard(bool state);
    virtual void buttonConnectClicked();
    virtual void waitForConnection();

private:
    CustomKeyboard keyboard;
    /* Holds a pointer to the button that called the keyboard.
     * Button clicked methods are responsible for handling this.
     * CALLINGBUTTONBUFFER_SIZE, CallingTextAreaBackground and CallingTextArea
     *  should also be set.
    */
    touchgfx::Unicode::UnicodeChar* CallingButtonBuffer = nullptr;
    uint16_t CALLINGBUTTONBUFFER_SIZE = 0;
    touchgfx::Box* CallingTextAreaBackground = nullptr;
    touchgfx::TextAreaWithOneWildcard* CallingTextArea = nullptr;

    /*  Minimum width for the CallingTextAreaBackground, so in case text is shorter
     * it doesn't look ugly
     */
    const uint8_t MIN_CALLINGTEXTAREABACKGROUND_WIDTH = 161;

    // Overloaded setText function to match data frequency and keyboard uses
    void setText(const char* data, Unicode::UnicodeChar * textAreaBuffer,  const uint16_t TEXTAREA_SIZE, touchgfx::TextAreaWithOneWildcard* textArea, touchgfx::Box* TextAreaBackground);
    void setText(const Unicode::UnicodeChar* data, Unicode::UnicodeChar * textAreaBuffer,  const uint16_t TEXTAREA_SIZE, touchgfx::TextAreaWithOneWildcard* textArea, touchgfx::Box* TextAreaBackground);
    void setText(const uint16_t data, Unicode::UnicodeChar * textAreaBuffer,  const uint16_t TEXTAREA_SIZE, touchgfx::TextAreaWithOneWildcard* textArea, touchgfx::Box* TextAreaBackground);
};

#endif // SCREENMENUVIEW_HPP
