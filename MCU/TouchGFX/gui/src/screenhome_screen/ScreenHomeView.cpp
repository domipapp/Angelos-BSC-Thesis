#include <gui/screenhome_screen/ScreenHomeView.hpp>

ScreenHomeView::ScreenHomeView()
{

}

void ScreenHomeView::setupScreen()
{
    ScreenHomeViewBase::setupScreen();
}

void ScreenHomeView::tearDownScreen()
{
    ScreenHomeViewBase::tearDownScreen();
}

void ScreenHomeView::signalTransitionSource(){
	osSemaphoreRelease(semaphoreTransitionFromHomeHandle);
}
