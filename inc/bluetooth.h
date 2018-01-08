/*
 * bluetooth.h
 *
 * bluetooth and pit implementation of comm
 *
 *  Created on: Dec 23, 2017
 *      Author: dipsy
 */

#ifndef INC_BLUETOOTH_H_
#define INC_BLUETOOTH_H_

#include <vector>
#include <functional>

#include <libbase/k60/mcg.h>
#include <libsc/system.h>
#include "libsc/k60/jy_mcu_bt_106.h"
#include "libbase/k60/pit.h"
#include "comm.h"
#include "config.h"

using libsc::k60::JyMcuBt106;
using libbase::k60::Pit;
using std::vector;
using std::function;

class Bluetooth: public Comm {
public:
	//initialize member bluetooth and member pit in this constructor
    Bluetooth():
    	m_bt(Config::GetBluetoothConfig([this](const Byte *data, const size_t size){
    		Comm::Listener(data, size);
    		return true;
    	})),
		m_pit(Config::GetBluetoothPitConfig([this](Pit*){
    		//this->SendFirst();
    		if (is_waiting_ack){
    			SendFirst();
    		}
    	})) {}

    ~Bluetooth(){}

    //implement send buffer
    virtual void SendBuffer(const Byte *buff, const int &size){
    	m_bt.SendBuffer(buff, size);
    	send_time = libsc::System::Time();
    	//is_timer_enabled = is_waiting_ack;
    }

    //getter
    bool IsTimerEnable(){return is_timer_enabled;}
private:
    JyMcuBt106 m_bt;
    Pit m_pit;

    bool is_timer_enabled=false;

    //implement enable timer
    void EnableTimer(bool flag){
    	is_timer_enabled = flag;
    	m_pit.SetEnable(flag);
    }
};



#endif /* INC_BLUETOOTH_H_ */
