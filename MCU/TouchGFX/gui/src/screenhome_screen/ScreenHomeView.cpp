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

void ScreenHomeView::displayTemperatureAndHumidity(){
	// Get data
	static float temperature, humidity;
	osStatus_t status = osMessageQueueGet(queueTouchgfxTempAndHumidHandle, &temperature, 0, 0);
	if (status != osOK)
		return;
    osMessageQueueGet(queueTouchgfxTempAndHumidHandle, &humidity, 0, osWaitForever);

    // Calculate resize position and print data on screen
    static uint16_t oldsize, newsize;
    oldsize = textAreaTemeratureNum.getTextWidth();
	Unicode::snprintfFloat(textAreaTemeratureNumBuffer, TEXTAREATEMERATURENUM_SIZE, "%.2f", temperature);
	textAreaTemeratureNum.resizeToCurrentTextWithAlignment();
	newsize = textAreaTemeratureNum.getTextWidth();
	textAreaTemeratureNum.setX(textAreaTemeratureNum.getX() - ((newsize - oldsize)/2));
	textAreaTemeratureNum.invalidate();

	// Calculate resize position and print data on screen
	oldsize = textAreaHumidityNum.getTextWidth();
	Unicode::snprintfFloat(textAreaHumidityNumBuffer, TEXTAREAHUMIDITYNUM_SIZE, "%.2f", humidity);
	textAreaHumidityNum.resizeToCurrentTextWithAlignment();
	newsize = textAreaHumidityNum.getTextWidth();
	textAreaHumidityNum.setX(textAreaHumidityNum.getX() - ((newsize - oldsize)/2));
	textAreaHumidityNum.invalidate();

}
