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
    void popUpKeyboard();
protected:
    CustomKeyboard keyboard;
};

#endif // SCREENMENUVIEW_HPP
