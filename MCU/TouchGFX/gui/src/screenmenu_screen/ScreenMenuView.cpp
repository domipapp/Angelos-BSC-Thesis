#include <gui/screenmenu_screen/ScreenMenuView.hpp>

ScreenMenuView::ScreenMenuView()
{

}

void ScreenMenuView::setupScreen()
{
    ScreenMenuViewBase::setupScreen();
}

void ScreenMenuView::tearDownScreen()
{
    ScreenMenuViewBase::tearDownScreen();
}

void ScreenMenuView::ServerPortClicked()
{	uint16_t a = 123;
	Unicode::snprintf(textAreaServerPortBuffer, TEXTAREASERVERPORT_SIZE, "%u", a);
	textAreaServerPort.invalidate();
}
