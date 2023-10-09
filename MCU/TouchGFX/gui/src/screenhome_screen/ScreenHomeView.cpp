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
    oldsize = textAreaTemerature.getTextWidth();
	Unicode::snprintfFloat(textAreaTemeratureBuffer, TEXTAREATEMERATURE_SIZE, "%.2f", temperature);
	textAreaTemerature.resizeToCurrentTextWithAlignment();
	newsize = textAreaTemerature.getTextWidth();
	textAreaTemerature.setX(textAreaTemerature.getX() - ((newsize - oldsize)/2));
	textAreaTemerature.invalidate();

	// Calculate resize position and print data on screen
	oldsize = textAreaHumidity.getTextWidth();
	Unicode::snprintfFloat(textAreaHumidityBuffer, TEXTAREAHUMIDITY_SIZE, "%.2f", humidity);
	textAreaHumidity.resizeToCurrentTextWithAlignment();
	newsize = textAreaHumidity.getTextWidth();
	textAreaHumidity.setX(textAreaHumidity.getX() - ((newsize - oldsize)/2));
	textAreaHumidity.invalidate();

}
