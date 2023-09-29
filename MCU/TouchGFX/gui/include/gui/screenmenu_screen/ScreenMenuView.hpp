#ifndef SCREENMENUVIEW_HPP
#define SCREENMENUVIEW_HPP

#include <gui_generated/screenmenu_screen/ScreenMenuViewBase.hpp>
#include <gui/screenmenu_screen/ScreenMenuPresenter.hpp>
#include <gui/common/CustomKeyboard.hpp>

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
protected:
    CustomKeyboard keyboard;
    /* Holds a pointer to the button that called the keyboard.
     * Button clicked methods are responsible for handling this.
     * CALLINGBUTTONBUFFER_SIZE and CallingTextAreaBackground should also be set.
    */
    touchgfx::Unicode::UnicodeChar* CallingButtonBuffer = nullptr;
    uint16_t CALLINGBUTTONBUFFER_SIZE = 0;
    touchgfx::Box* CallingTextAreaBackground = nullptr;
};

#endif // SCREENMENUVIEW_HPP
