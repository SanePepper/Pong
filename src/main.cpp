/*
 * main.cpp
 *
 * Author: 
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstring>
#include <string>
#include <libbase/k60/mcg.h>
#include <libsc/system.h>

#include <libsc/led.h>
#include <libsc/k60/jy_mcu_bt_106.h>
#include "libsc/led.h"
#include "libsc/joystick.h"
#include "libsc/st7735r.h"
#include "libsc/battery_meter.h"
#include "libsc/k60/jy_mcu_bt_106.h"
#include "libbase/k60/pit.h"
#include "libsc/lcd_typewriter.h"
#include "libsc/joystick.h"

#include "pong.h"
#include "config.h"
#include "bluetooth.h"
#include "ball.h"
#include "platform.h"

namespace libbase
{
    namespace k60
    {
        Mcg::Config Mcg::GetMcgConfig()
        {
            Mcg::Config config;
            config.external_oscillator_khz = 50000;
            config.core_clock_khz = 150000;
            return config;
        }
    }
}

using libsc::System;
using namespace libsc;
using namespace libbase::k60;
using std::string;

Led *pLed = nullptr;

bool Listener(const unsigned char*, const unsigned int){
	if(pLed)pLed->Switch();
	return 1;
}

int main() {
    System::Init();

    Led led0(Config::GetLedConfig(0));
    Led led1(Config::GetLedConfig(1));
    Led led2(Config::GetLedConfig(2));
    Led led3(Config::GetLedConfig(3));
    pLed = &led0;

    St7735r lcd(Config::GetLcdConfig());
    LcdTypewriter writer(Config::GetWriterConfig(&lcd));
    LcdConsole console(Config::GetConsoleConfig(&lcd));
    lcd.SetRegion(Lcd::Rect(0,0,128,160));
    writer.WriteString("YOU WIN!");

    led0.SetEnable(1);
    led1.SetEnable(0);
    led2.SetEnable(0);
    led3.SetEnable(0);

    Comm::Package pkg;

    Bluetooth bt;

    bt.SetHandler([&led0,&led1,&led2,&led3,&bt,&pkg](Bluetooth::Package package){
    	pkg = package;
    	switch((int)package.type){
    	case Bluetooth::PkgType::kStart:
    		led0.Switch();
				break;
    	case Bluetooth::PkgType::kStartACK:
    		led1.Switch();
    		break;
    	}
    });

	Ball ball(Config::GetBallConfig(&lcd));
	Platform opponentPlatform(Config::GetPlatformConfig(&lcd,false));
	Platform yourPlatform(Config::GetPlatformConfig(&lcd,true));

	Joystick joystick(Config::GetJoystickConfig([&yourPlatform, &opponentPlatform](const uint8_t id, const Joystick::State state){
		if (state == Joystick::State::kLeft){
			yourPlatform.moveLeft();
		}
		else if (state == Joystick::State::kRight){
			yourPlatform.moveRight();
		}
		else if (state == Joystick::State::kUp){
			opponentPlatform.moveLeft();
		}
		else if (state == Joystick::State::kDown){
			opponentPlatform.moveRight();
		}
	}));

	libsc::Timer::TimerInt time = libsc::System::Time();

	lcd.SetRegion(Lcd::Rect(0,0,128,160));
	lcd.FillColor(0xFFFF);

	bool end_game = false;

    while(!end_game){
    	while(System::Time() != time){
    		time = libsc::System::Time();
        	if(time % 100 == 0){
        		ball.move(yourPlatform.getPosition().x, opponentPlatform.getPosition().x);

    			//draw the ball and platforms
    			ball.render();
    			opponentPlatform.render();
    			yourPlatform.render();

    			//Fill the border
    			lcd.SetRegion(Lcd::Rect(4,5,120,1));
    			lcd.FillColor(0x0000);
    			lcd.SetRegion(Lcd::Rect(4,5,1,150));
    			lcd.FillColor(0x0000);
    			lcd.SetRegion(Lcd::Rect(124,5,1,150));
    			lcd.FillColor(0x0000);
    			lcd.SetRegion(Lcd::Rect(4,155,120,1));
    			lcd.FillColor(0x0000);
        	}
    	}
    }
    return 0;
}
