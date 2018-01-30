/*
 * config.h
 *
 * configure all the peripherals here
 *
 *  Created on: Dec 23, 2017
 *      Author: dipsy
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include <functional>

#include "libsc/led.h"
#include "libsc/joystick.h"
#include "libsc/st7735r.h"
#include "libsc/lcd_typewriter.h"
#include "libsc/lcd_console.h"
#include "libsc/battery_meter.h"
#include "libsc/k60/jy_mcu_bt_106.h"
#include "libbase/k60/pit.h"
#include "libsc/k60/uart_device.h"
#include "libsc/joystick.h"
#include "sprite.h"

using libsc::Led;
using libsc::Lcd;
using libsc::Joystick;
using libsc::St7735r;
using libsc::LcdTypewriter;
using libsc::LcdConsole;
using libsc::BatteryMeter;
using libsc::k60::JyMcuBt106;
using libbase::k60::Pit;
using libsc::Joystick;

class Config{
public:

	static Led::Config GetLedConfig(int id){
		Led::Config config;
		config.id = id;
		config.is_active_low = true;
		return config;
	}

    static Joystick::Config GetJoystickConfig(Joystick::Listener isr) {
        //TODO: finish it
    	Joystick::Config config;
    	config.id = 0;
    	config.is_active_low = true;
    	config.dispatcher = isr;
    	return config;
    }

    static St7735r::Config GetLcdConfig() {
        //TODO: finish it
		St7735r::Config config;
		config.fps = 20;
		config.is_bgr = false;
		return config;
    }

    static LcdTypewriter::Config GetWriterConfig(St7735r* lcd){
		LcdTypewriter::Config config;
		config.is_clear_line = true;
		config.is_text_wrap = true;
		config.text_color = 0;
		config.bg_color = 0xFFFF;
		config.lcd = lcd;
		return config;

    }

    static LcdConsole::Config GetConsoleConfig( St7735r* lcd){
		LcdConsole::Config config;
		config.bg_color = 0xFFFFFF;
		config.text_color = 0;
		config.region = Lcd::Rect(0, 0, 128, 160);
		config.lcd = lcd;
		return config;

    }
    static JyMcuBt106::Config GetBluetoothConfig(std::function<bool(const Byte *data, const size_t size)> isr) {
        //TODO: finish it
    	JyMcuBt106::Config config;
    	config.id = 0;
    	config.baud_rate = libbase::k60::Uart::Config::BaudRate::k115200;
    	config.rx_isr = isr;
    	return config;
    }

    static Pit::Config GetBluetoothPitConfig(std::function<void(Pit*)> isr){
    	//TODO: finish it
    	 Pit::Config pitConfig;
    	 pitConfig.channel = 0;
    	 pitConfig.count = 75000*10;
    	 pitConfig.isr = isr;
    	 pitConfig.is_enable = true; //dont know if it's needed
    	 //Pit pit(pitConfig);
    	 return pitConfig;
    }

    static Sprite::Config GetBallConfig(Lcd* pLcd){
    	Sprite::Config config;
    	config.fg_color = 0xFC20;
    	config.bg_color = 0xFFFF;
    	config.width = 5;
    	config.height = 5;
    	config.x = 64;
    	config.y = 80;
    	config.pLcd = pLcd;
    	return config;
    }

    static Sprite::Config GetPlatformConfig(Lcd* pLcd, bool yours){
    	Sprite::Config config;
    	config.fg_color = 0x0000;
    	config.bg_color = 0xFFFF;
    	config.width = 30;
    	config.height = 4;
    	if (yours){
    		config.x = 48;
    		config.y = 148;
    	}
    	else{
    		config.x = 48;
    		config.y = 8;
    	}
    	config.pLcd = pLcd;
    	return config;
    }

};


#endif /* INC_CONFIG_H_ */
