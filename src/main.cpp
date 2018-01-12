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

#include "debug.h"
#include "pong.h"
#include "config.h"
#include "bluetooth.h"

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

char *str = "";

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
    //Debug::message = "YO";
    //Debug::pLcd = &lcd;
    //Debug::pWriter = &writer;
    //Debug::pConsole = &console;

    led0.SetEnable(1);
    led1.SetEnable(1);
    led2.SetEnable(1);
    led3.SetEnable(1);

    Comm::Package pkg;

    //lcd.FillColor(0xFFFFFF);

    Bluetooth bt;

    //lcd.FillColor(0);


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

    bt.SendPackage({57,Bluetooth::PkgType::kStart,{},Bluetooth::BitConsts::kSTART});
    bt.SendPackage({0,Bluetooth::PkgType::kMasterPlatform,{7},Bluetooth::BitConsts::kEND});
    bt.SendPackage({0,Bluetooth::PkgType::kSlavePlatform,{1},Bluetooth::BitConsts::kEND});
    bt.SendPackage({0,Bluetooth::PkgType::kReflection,{1,2},Bluetooth::BitConsts::kEND});
    bt.SendPackage({0,Bluetooth::PkgType::kLocation,{1,2},Bluetooth::BitConsts::kEND});
    bt.SendPackage({0,Bluetooth::PkgType::kResult,{0},Bluetooth::BitConsts::kEND});

	int i = 0;
	int num=0;
	libsc::Timer::TimerInt time=libsc::System::Time();
    while(1){
    	while(System::Time()!=time){
    		time=libsc::System::Time();
        	if(System::Time()%100==6){
        	    //bt.SendPackage({0,Bluetooth::PkgType::kMasterPlatform,{7},Bluetooth::BitConsts::kEND});
        	    bt.SendPackage({i,Bluetooth::PkgType::kStart,{},Bluetooth::BitConsts::kSTART});
    ++num;
    if(num==3){
    	int i=0;
    }
        	    i = (i+1) % 255;
        		led0.Switch();
        		led1.Switch();
        		led2.Switch();
        		led3.Switch();
    			char c[10];
    			lcd.SetRegion(Lcd::Rect(0,0,100,15));
    			if(bt.IsWaitingACK()){
    				writer.WriteString("waiting");
    			}else{
    				writer.WriteString("not waiting");
    			}
    			lcd.SetRegion(Lcd::Rect(0,15,100,15));
    			sprintf(c,"Qsize:%d",bt.queue.size());
    			writer.WriteBuffer(c,10);

    			lcd.SetRegion(Lcd::Rect(0,30,100,15));
    			sprintf(c,"%d %d %d %d %d",bt.s[0],bt.s[1],bt.s[2],bt.s[3],bt.s[4]);
    			writer.WriteBuffer(c,10);
    			lcd.SetRegion(Lcd::Rect(0,45,100,15));
    			sprintf(c,"%d %d %d %d %d",bt.r[0],bt.r[1],bt.r[2],bt.r[3],bt.r[4]);
    			writer.WriteBuffer(c,10);
    //			lcd.SetRegion(Lcd::Rect(0,60,100,15));
    //			sprintf(c,"%d %d %d %d %d   ",bt.ss,bt.sr,bt.sb,bt.sc,bt.b1);
    //			writer.WriteBuffer(c,10);
    			lcd.SetRegion(Lcd::Rect(0,60,100,15));
    			sprintf(c,"NS:%d     ",System::Time());
    			writer.WriteBuffer(c,10);
    			lcd.SetRegion(Lcd::Rect(0,75,100,15));
    			sprintf(c,"NS:%d     ",bt.NSCount);
    			writer.WriteBuffer(c,10);
    			lcd.SetRegion(Lcd::Rect(0,90,100,15));
    			sprintf(c,"AS:%d     ",bt.ASCount);
    			writer.WriteBuffer(c,10);
    			lcd.SetRegion(Lcd::Rect(0,105,100,15));
    			sprintf(c,"AR:%d     ",bt.ARCount);
    			writer.WriteBuffer(c,10);
    			lcd.SetRegion(Lcd::Rect(0,120,100,15));
    			sprintf(c,"D:%d     ",bt.DCount);
    			writer.WriteBuffer(c,10);
    			lcd.SetRegion(Lcd::Rect(0,135,100,15));
    			sprintf(c,"N:%d     ",bt.NCount);
    			writer.WriteBuffer(c,10);
        	}
    	}
    }
    return 0;
}
