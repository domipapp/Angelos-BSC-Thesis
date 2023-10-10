/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/screenhome_screen/ScreenHomeViewBase.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

ScreenHomeViewBase::ScreenHomeViewBase() :
    buttonCallback(this, &ScreenHomeViewBase::buttonCallbackHandler),
    frameCountDisplayTemperatureAndHumidityInterval(0)
{
    __background.setPosition(0, 0, 480, 272);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    add(__background);

    Background.setXY(0, 0);
    Background.setBitmap(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_BACKGROUNDS_480X272_WAVES_ID));
    add(Background);

    containerMenuBar.setPosition(0, 0, 480, 50);
    TopBar.setXY(0, 0);
    TopBar.setBitmap(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_BARS_480X272_BOTTOM_DIM_DARK_ID));
    TopBar.setAlpha(135);
    containerMenuBar.add(TopBar);

    textAreaMenu.setXY(182, 1);
    textAreaMenu.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    textAreaMenu.setLinespacing(0);
    textAreaMenu.setTypedText(touchgfx::TypedText(T___SINGLEUSE_5MHQ));
    containerMenuBar.add(textAreaMenu);

    buttonSettings.setXY(0, 0);
    buttonSettings.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_BUTTON_REGULAR_HEIGHT_50_TINY_ROUNDED_ACTIVE_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_BUTTON_REGULAR_HEIGHT_50_TINY_ROUNDED_PRESSED_ID));
    buttonSettings.setLabelText(touchgfx::TypedText(T___SINGLEUSE_R00K));
    buttonSettings.setLabelColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    buttonSettings.setLabelColorPressed(touchgfx::Color::getColorFromRGB(255, 255, 255));
    buttonSettings.setAction(buttonCallback);
    containerMenuBar.add(buttonSettings);

    buttonDisconnect.setXY(370, 0);
    buttonDisconnect.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_BUTTON_REGULAR_HEIGHT_50_TINY_ROUNDED_ACTIVE_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_BUTTON_REGULAR_HEIGHT_50_TINY_ROUNDED_PRESSED_ID));
    buttonDisconnect.setLabelText(touchgfx::TypedText(T___SINGLEUSE_FKC0));
    buttonDisconnect.setLabelColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    buttonDisconnect.setLabelColorPressed(touchgfx::Color::getColorFromRGB(255, 255, 255));
    containerMenuBar.add(buttonDisconnect);

    add(containerMenuBar);

    textAreaTemerature.setXY(73, 137);
    textAreaTemerature.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textAreaTemerature.setLinespacing(0);
    Unicode::snprintf(textAreaTemeratureBuffer, TEXTAREATEMERATURE_SIZE, "%s", touchgfx::TypedText(T___SINGLEUSE_OROW).getText());
    textAreaTemerature.setWildcard(textAreaTemeratureBuffer);
    textAreaTemerature.resizeToCurrentText();
    textAreaTemerature.setTypedText(touchgfx::TypedText(T___SINGLEUSE_NISC));
    add(textAreaTemerature);

    textAreaHumidity.setXY(336, 137);
    textAreaHumidity.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textAreaHumidity.setLinespacing(0);
    Unicode::snprintf(textAreaHumidityBuffer, TEXTAREAHUMIDITY_SIZE, "%s", touchgfx::TypedText(T___SINGLEUSE_ZUM9).getText());
    textAreaHumidity.setWildcard(textAreaHumidityBuffer);
    textAreaHumidity.resizeToCurrentText();
    textAreaHumidity.setTypedText(touchgfx::TypedText(T___SINGLEUSE_NMKU));
    add(textAreaHumidity);
}

ScreenHomeViewBase::~ScreenHomeViewBase()
{

}

void ScreenHomeViewBase::setupScreen()
{

}

void ScreenHomeViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &buttonSettings)
    {
        //ChangeSrceen
        //When buttonSettings clicked change screen to ScreenMenu
        //Go to ScreenMenu with screen transition towards East
        application().gotoScreenMenuScreenSlideTransitionEast();
    
        //signalTransitionSource
        //When ChangeSrceen completed call virtual function
        //Call signalTransitionSource
        signalTransitionSource();
    }
}

void ScreenHomeViewBase::handleTickEvent()
{
    frameCountDisplayTemperatureAndHumidityInterval++;
    if(frameCountDisplayTemperatureAndHumidityInterval == TICK_DISPLAYTEMPERATUREANDHUMIDITY_INTERVAL)
    {
        //displayTemperatureAndHumidity
        //When every N tick call virtual function
        //Call displayTemperatureAndHumidity
        displayTemperatureAndHumidity();
        frameCountDisplayTemperatureAndHumidityInterval = 0;
    }
}