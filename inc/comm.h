/*
 * comm.h
 *
 * Communication Protocol used for Pong, use of hardware components
 * to be specified in bluetooth.h
 *
 *  Created on: Dec 27, 2017
 *      Author: dipsy
 */

#ifndef INC_COMM_H_
#define INC_COMM_H_

#include <vector>
#include <functional>

#include <libbase/k60/mcg.h>
#include <libsc/system.h>
#include <libbase/misc_types.h>

#include "debug.h"
#include "bluetooth.h"

using std::vector;
using std::function;

class Pong;

class Comm {
public:
	/**
	 * Package Types (better looking of 0-11 integers)
	 */
	enum PkgType : Byte{
		kStart=0,
		kStartACK=1,
		kMasterPlatform=2,
		kMasterPlatformACK=3,
		kSlavePlatform=4,
		kSlavePlatformACK=5,
		kReflection=6,
		kReflectionACK=7,
		kLocation=8,
		kLocationACK=9,
		kResult=10,
		kResultACK=11
	};
	int NSCount = 0;
	int ASCount = 0;
	int ARCount = 0;
	/**
	 * Special Byte for labeling end of package
	 */
	struct BitConsts {
	    static constexpr Byte kSTART = 0xF0;
	    static constexpr Byte kEND = 0xFF;
	    static constexpr Byte kACK = 0xF1;
	};

	/**
	 * Structure of a single package
	 */
	struct Package{
		Byte frame_id;
		PkgType type;
		vector<Byte> data;
		//Byte endingByte = BitConsts::kEND;
		Byte endingByte;
	};

	/**
	 * type of package handler function
	 */
	typedef function<void(Package)> PackageHandler;

	/**
	 * Constructor and destructor
	 */
	Comm():is_waiting_ack(false){}
	virtual ~Comm(){}

	//TX helpers
	/**
	 * SendPackage will send package
	 * if need_ack, keep sending package until receive ack
	 * else send out the package directly
	 * tips: use a queue
	 */
	void SendPackage(const Package& pkg, bool need_ack = true){
		if ((IsWaitingACK()) || (queue.size() > 0)){
			queue.push_back(pkg);
		}
		else{
			SendBuffer(&pkg.frame_id,sizeof(pkg));
			NSCount++;
			if (need_ack){
				is_waiting_ack = true;
				queue.push_back(pkg);
				S = pkg;
			}
		}
	}

	/**
	 * Implementation of sending out bytes shall be implemented in derived class
	 */
	virtual void SendBuffer(const Byte *buff, const int &size)=0;

	/**
	 * Listener (will be irs for bluetooth) when bunch of Bytes arrive
	 * You may want to parse the Byte array into packages
	 */
	bool Listener(const Byte* data, const size_t size){
		//buffer.clear();
		for (int i = 0; i < size; i++){
			buffer.push_back(*(data+i));
			if ((i == size - 1) && ((*(data+i) == BitConsts::kSTART) || (*(data+i) == BitConsts::kEND) || (*(data+i) == BitConsts::kACK))){
				Tb = *(data+i);
				//Tb = buffer[sizeof(buffer)-1];
				BuildBufferPackage();
				C++;
			}
		}
		return 0;
	}

	/**
	 * Set the package handler, which shall be called when a package is parsed
	 */
	void SetHandler(PackageHandler handler){this->Handler = handler;}

	//some variable getters
	bool IsWaitingACK(){return is_waiting_ack;}

	//tx variables
	vector<Package> queue;	//pending send package
	int send_time = -1;			//last send time

protected:
	bool is_waiting_ack;

	/**
	 * you need to resend package per 10ms until receive ACK
	 * this function set enable this proccess or not
	 */
	virtual void EnableTimer(bool flag)=0;

	/**
	 * deliver the first package in queue
	 */
	virtual void SendFirst(){
		if (queue.size() > 0){
			SendBuffer(&queue[0].frame_id,sizeof(queue[0]));
			NSCount++;
			if (queue[0].type % 2 == 0){
				is_waiting_ack = true;
				//eByte = queue[0].endingByte;
			}
			else{
				queue.erase(queue.begin());
			}
		}
	}

private:
	/**
	 * package handler function
	 */
	PackageHandler Handler; //implement later, change the value & render & validation
public:
	//rx veriable
	vector<Byte> buffer;	//temporary storage for incoming Bytes
	Byte C = 0;
	bool b1 = 0,b2 = 0,b3 = 0;
	Package T,S;
	Byte Tb;
private:
	/**
	 * when the buffer contains a complete package bytes,
	 * build the package, call handler/ handle the packages directly
	 * and clear buffer
	 */

	void BuildBufferPackage(){
		Package receivedPkg;
		receivedPkg.frame_id = buffer[0];
		receivedPkg.type = static_cast<PkgType>(buffer[1]);
		for (int i = 2; i < sizeof(receivedPkg)-1; i++){
			receivedPkg.data.push_back(buffer[i]);
		}
		receivedPkg.endingByte = buffer[sizeof(receivedPkg)-1];
		T = receivedPkg;
		buffer.clear();
		Handler(receivedPkg);
		C++;
		b1 = (receivedPkg.endingByte == BitConsts::kSTART);
		b2 = (receivedPkg.endingByte == BitConsts::kEND);
		b3 = (receivedPkg.endingByte == BitConsts::kACK);
		if (receivedPkg.endingByte == BitConsts::kACK){
			is_waiting_ack = false;
			queue.erase(queue.begin());
			SendFirst();
			ARCount++;
		}
		else if ((receivedPkg.endingByte == BitConsts::kSTART) || (receivedPkg.endingByte == BitConsts::kEND)){
			//send ack msg
			Package ackPkg;
			ackPkg.frame_id = receivedPkg.frame_id;
			ackPkg.type = static_cast<PkgType>(receivedPkg.type + 1);
			ackPkg.data = {};
			ackPkg.endingByte = BitConsts::kACK;
			SendBuffer(&ackPkg.frame_id,sizeof(ackPkg));
			ASCount++;
			C++;
		}

	}
};



#endif /* INC_COMM_H_ */
