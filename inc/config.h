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

using libsc::Led;
using libsc::Lcd;
using libsc::Joystick;
using libsc::St7735r;
using libsc::LcdTypewriter;
using libsc::LcdConsole;
using libsc::BatteryMeter;
using libsc::k60::JyMcuBt106;
using libbase::k60::Pit;

class Config{
public:

	static Led::Config GetLedConfig(int id){
		Led::Config config;
		config.id = id;
		config.is_active_low = true;
		return config;
	}

    static Joystick::Config GetJoystickConfig() {
        //TODO: finish it
    }

    static St7735r::Config GetLcdConfig() {
        //TODO: finish it
		St7735r::Config config;
		config.fps = 20;
		config.is_bgr = false;
		//config.is_revert = false;
		return config;
    }

    static LcdTypewriter::Config GetWriterConfig(St7735r* lcd){
		LcdTypewriter::Config config;
		config.is_clear_line = true;
		config.is_text_wrap = true;
		config.text_color = 0xFFFFFF;
		config.bg_color = 0;
		config.lcd = lcd;
		return config;

    }

    static LcdConsole::Config GetConsoleConfig( St7735r* lcd){
		LcdConsole::Config config;
		config.bg_color = 0;
		config.text_color = 0xFFFFFF;
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
    	 pitConfig.count = 75000*100;
    	 pitConfig.isr = isr;
    	 pitConfig.is_enable = true; //dont know if it's needed
    	 //Pit pit(pitConfig);
    	 return pitConfig;
    }

};


#endif /* INC_CONFIG_H_ */
