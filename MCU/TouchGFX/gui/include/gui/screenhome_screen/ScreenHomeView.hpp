#ifndef SCREENHOMEVIEW_HPP
#define SCREENHOMEVIEW_HPP

#include <gui_generated/screenhome_screen/ScreenHomeViewBase.hpp>
#include <gui/screenhome_screen/ScreenHomePresenter.hpp>
#include "main.h"

class ScreenHomeView : public ScreenHomeViewBase
{
public:
    ScreenHomeView();
    virtual ~ScreenHomeView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void displayTemperatureAndHumidity();
    virtual void buttonDisconnectClicked();
    virtual void buttonSettingsClicked();
protected:
};

#endif // SCREENHOMEVIEW_HPP
