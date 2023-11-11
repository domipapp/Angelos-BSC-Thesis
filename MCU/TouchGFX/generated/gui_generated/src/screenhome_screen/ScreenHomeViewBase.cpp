/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/screenhome_screen/ScreenHomeViewBase.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

ScreenHomeViewBase::ScreenHomeViewBase() :
    buttonCallback(this, &ScreenHomeViewBase::buttonCallbackHandler)
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
    buttonDisconnect.setAction(buttonCallback);
    containerMenuBar.add(buttonDisconnect);

    add(containerMenuBar);

    textAreaTemperatureBackground.setXY(3, 62);
    textAreaTemperatureBackground.setBitmap(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_CONTAINERS_SMALL_WIDE_OUTLINED_LIGHT_ID));
    textAreaTemperatureBackground.setAlpha(150);
    add(textAreaTemperatureBackground);

    textAreaTemeratureNum.setXY(73, 137);
    textAreaTemeratureNum.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textAreaTemeratureNum.setLinespacing(0);
    Unicode::snprintf(textAreaTemeratureNumBuffer, TEXTAREATEMERATURENUM_SIZE, "%s", touchgfx::TypedText(T___SINGLEUSE_OROW).getText());
    textAreaTemeratureNum.setWildcard(textAreaTemeratureNumBuffer);
    textAreaTemeratureNum.resizeToCurrentText();
    textAreaTemeratureNum.setTypedText(touchgfx::TypedText(T___SINGLEUSE_NISC));
    add(textAreaTemeratureNum);

    textAreaTemperature.setXY(46, 78);
    textAreaTemperature.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textAreaTemperature.setLinespacing(0);
    textAreaTemperature.setTypedText(touchgfx::TypedText(T___SINGLEUSE_WQ0B));
    add(textAreaTemperature);

    textAreaHumidityBackground.setXY(266, 62);
    textAreaHumidityBackground.setBitmap(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_CONTAINERS_SMALL_WIDE_OUTLINED_LIGHT_ID));
    textAreaHumidityBackground.setAlpha(150);
    add(textAreaHumidityBackground);

    textAreaHumidityNum.setXY(336, 137);
    textAreaHumidityNum.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textAreaHumidityNum.setLinespacing(0);
    Unicode::snprintf(textAreaHumidityNumBuffer, TEXTAREAHUMIDITYNUM_SIZE, "%s", touchgfx::TypedText(T___SINGLEUSE_ZUM9).getText());
    textAreaHumidityNum.setWildcard(textAreaHumidityNumBuffer);
    textAreaHumidityNum.resizeToCurrentText();
    textAreaHumidityNum.setTypedText(touchgfx::TypedText(T___SINGLEUSE_NMKU));
    add(textAreaHumidityNum);

    textAreaHumidity.setXY(326, 78);
    textAreaHumidity.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textAreaHumidity.setLinespacing(0);
    textAreaHumidity.setTypedText(touchgfx::TypedText(T___SINGLEUSE_YRSU));
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
    if (&src == &buttonDisconnect)
    {
        //buttonDisconnectClicked
        //When buttonDisconnect clicked call virtual function
        //Call buttonDisconnectClicked
        buttonDisconnectClicked();
    
        //ChangeScreenDisconnectButton
        //When buttonDisconnectClicked completed change screen to ScreenMenu
        //Go to ScreenMenu with screen transition towards East
        application().gotoScreenMenuScreenSlideTransitionEast();
    }
    if (&src == &buttonSettings)
    {
        //buttonSettingsClicked
        //When buttonSettings clicked call virtual function
        //Call buttonSettingsClicked
        buttonSettingsClicked();
    
        //ChangeScreenSettingsButton
        //When buttonSettingsClicked completed change screen to ScreenMenu
        //Go to ScreenMenu with screen transition towards East
        application().gotoScreenMenuScreenSlideTransitionEast();
    }
}

void ScreenHomeViewBase::handleTickEvent()
{
    //displayTemperatureAndHumidity
    //When every N tick call virtual function
    //Call displayTemperatureAndHumidity
    displayTemperatureAndHumidity();
}
