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
#include "score.h"

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

enum Game_role{
	Not_started = 0,
	Master,
	Slave
};

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

    Game_role role = Not_started;
    bool start = false;

    bt.SetHandler([&led0,&led1,&led2,&led3,&bt,&pkg,&start,&role](Bluetooth::Package package){
    	pkg = package;
    	switch((int)package.type){
    	case Bluetooth::PkgType::kStart:
    		led2.Switch();
    		role = Slave;
			break;
    	case Bluetooth::PkgType::kStartACK:
    		led1.Switch();
    		start = true;
    		break;
    	}
    });

	Pong pong;
	Score score;
	Ball ball(Config::GetBallConfig(&lcd), &score);
	Platform opponentPlatform(Config::GetPlatformConfig(&lcd,false));
	Platform yourPlatform(Config::GetPlatformConfig(&lcd,true));

	Joystick::State joystickDir = Joystick::State::kIdle;
	bool startClick = false;
	Joystick joystick(Config::GetJoystickConfig([&startClick, &joystickDir, &role](const uint8_t id, const Joystick::State state){
		if (state == Joystick::State::kLeft || state == Joystick::State::kRight){
			startClick = !startClick;
			joystickDir = state;
		}
		else if (state == Joystick::State::kSelect){
			role = Master;
		}
	}));

	libsc::Timer::TimerInt time = libsc::System::Time();

	lcd.SetRegion(Lcd::Rect(0,0,128,160));
	lcd.FillColor(0xFFFF);

	bool end_game = false;

	while (role == Not_started){}
	writer.WriteString("start");

	if (role == Master){
		writer.WriteString("master");
		ball.render();
		yourPlatform.render();
		opponentPlatform.render();
		pong.drawBorder(&lcd);
		System::DelayS(2);
		bt.SendPackage({0,Bluetooth::PkgType::kStart,{},Bluetooth::BitConsts::kSTART},true);
		led2.Switch();
		//wait for ack
		while (!start){}
	}

	if (role == Slave){
		writer.WriteString("slave");
		ball.render();
		yourPlatform.render();
		opponentPlatform.render();
		pong.drawBorder(&lcd);
	}

	while(!end_game){
		while(System::Time() != time){
			time = libsc::System::Time();
			if(time % 50 == 0){
				if (startClick){
					if (joystickDir == Joystick::State::kLeft){
						yourPlatform.moveLeft();
					}
					else if (joystickDir == Joystick::State::kRight){
						yourPlatform.moveRight();
					}
				}
				pong.oneFrame(&lcd, &ball, &yourPlatform, &opponentPlatform, &time, &score);
//        		if (libsc::Timer::TimeDiff(time, score.getTime()) >= 2000){
//        			pong.oneFrame(&lcd, &ball, &yourPlatform, &opponentPlatform);
//        		}
				if (score.getWin() == 3){
					end_game = true;
					//you win lcd typewriter
				}
				else if (score.getLose() == 3){
					end_game = true;
					//you lose lcd typewriter
				}
			}
		}
	}

    //prevent return 0
    while (true){}
    return 0;
}
