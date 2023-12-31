#ifndef SCREENWELCOMEPRESENTER_HPP
#define SCREENWELCOMEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ScreenWelcomeView;

class ScreenWelcomePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ScreenWelcomePresenter(ScreenWelcomeView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~ScreenWelcomePresenter() {}

private:
    ScreenWelcomePresenter();

    ScreenWelcomeView& view;
};

#endif // SCREENWELCOMEPRESENTER_HPP
