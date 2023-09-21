#ifndef SCREENWELCOMEVIEW_HPP
#define SCREENWELCOMEVIEW_HPP

#include <gui_generated/screenwelcome_screen/ScreenWelcomeViewBase.hpp>
#include <gui/screenwelcome_screen/ScreenWelcomePresenter.hpp>

class ScreenWelcomeView : public ScreenWelcomeViewBase
{
public:
    ScreenWelcomeView();
    virtual ~ScreenWelcomeView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SCREENWELCOMEVIEW_HPP
