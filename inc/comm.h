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
	Byte r[5],s[5];
	int ss,sr,sb,sc = 0;
	int b1,errorc = 0;
	Byte* pMsg;
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
		queue.push_back(pkg);
		if ((IsWaitingACK() == false) && (queue.size() == 1)){
			SendFirst();
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
		for (int i = 0; i < size; i++){
			sr = size;
			buffer.push_back(*(data+i));
			sb = buffer.size();
			if (buffer.size() > 5){
				buffer.erase(buffer.begin());
			}
			if((*(data+i) == BitConsts::kSTART) || (*(data+i) == BitConsts::kEND) || (*(data+i) == BitConsts::kACK)){
				if (buffer.size() < 3){
					buffer.clear();
				}
				else{
					sc++;
					b1 = buffer[1];
					if (((buffer.size() == 3) && ((buffer[1] == 0) || (buffer[1] % 2 == 1)))
							|| ((buffer.size() == 4) && ((buffer[1] == 2) || (buffer[1] == 4) || (buffer[1] == 10)))
							|| ((buffer.size() == 5) && ((buffer[1] == 6) || (buffer[1] == 8)))){
						sc++;
						BuildBufferPackage();
					}
					else{
						buffer.clear();
					}
				}
			}
		}
		return 1;
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
		NSCount++;
		if (queue.size() > 0){
			if (IsWaitingACK()){
				SendBuffer(pMsg, queue[0].data.size()+3);
			}
			else{
				if (queue[0].type % 2 == 0){
					is_waiting_ack = true;
				}
				pMsg = new Byte[queue[0].data.size()+3];
				memcpy(pMsg,&queue[0].frame_id,1);
				memcpy(pMsg+1,&queue[0].type,1);
				for (int i = 0; i < queue[0].data.size(); i++){
					memcpy(pMsg+i+2,&queue[0].data[i],1);
				}
				memcpy(pMsg+queue[0].data.size()+2,&queue[0].endingByte,1);
				SendBuffer(pMsg, queue[0].data.size()+3);
				if (queue[0].type % 2 == 0){
					is_waiting_ack = true;
				}
				else{
					queue.erase(queue.begin());
					delete pMsg;
					pMsg = nullptr;
				}
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

private:
	/**
	 * when the buffer contains a complete package bytes,
	 * build the package, call handler/ handle the packages directly
	 * and clear buffer
	 */

	void BuildBufferPackage(){
		for (int j = 0; j < buffer.size(); j++){if (j<5){r[j] = buffer[j];}}
		Package receivedPkg;
		receivedPkg.frame_id = buffer[0];
		receivedPkg.type = static_cast<PkgType>(buffer[1]);
		receivedPkg.data.clear();
		for (int i = 0; i < buffer.size()-1; i++){
			receivedPkg.data.push_back(buffer[i+2]);
		}
		receivedPkg.endingByte = buffer[buffer.size()-1];
		buffer.clear();
		Handler(receivedPkg);
		if (receivedPkg.endingByte == BitConsts::kACK){
			is_waiting_ack = false;
			delete pMsg;
			pMsg = nullptr;
			queue.erase(queue.begin());
			SendFirst();
			ARCount++;
			receivedPkg.endingByte = 0; //dont know if its needed
		}
		else if ((receivedPkg.endingByte == BitConsts::kSTART) || (receivedPkg.endingByte == BitConsts::kEND)){
			//send ack msg
			ASCount++;
			Package ackPkg;
			ackPkg.frame_id = receivedPkg.frame_id;
			ackPkg.type = static_cast<PkgType>(receivedPkg.type + 1);
			ackPkg.data = {};
			ackPkg.endingByte = BitConsts::kACK;
			receivedPkg.endingByte = 0; //dont know if its needed
			Byte* pAck = new Byte[3];
			memcpy(pAck,&ackPkg.frame_id,1);
			memcpy(pAck+1,&ackPkg.type,1);
			memcpy(pAck+2,&ackPkg.endingByte,1);
			SendBuffer(pAck,3);
			delete pAck;
			pAck = nullptr;
		}
	}
};



#endif /* INC_COMM_H_ */
