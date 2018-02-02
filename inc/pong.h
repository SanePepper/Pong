/*
 * pong.h
 *
 *  Created on: 4 Jan 2018
 *      Author: PWS
 */

#include "libsc/st7735r.h"
#include "libsc/system.h"
#include "ball.h"
#include "platform.h"
#include "bluetooth.h"
#include "score.h"

#ifndef PONG_H_
#define PONG_H_

class Pong{
public:

	void oneFrame(libsc::St7735r* pLcd, Ball* pBall, Platform* pYourPlatform, Platform* pOpponentPlatform, libsc::Timer::TimerInt* time, Score* pScore){
		if (libsc::Timer::TimeDiff(*time, pScore->getTime()) >= 2000){
			pBall->move(pYourPlatform->getPosition().x, pOpponentPlatform->getPosition().x);
		}
		else{
			pBall->render();
		}
		//draw the ball and platforms
//		pBall->render();
		pOpponentPlatform->render();
		pYourPlatform->render();

		//Fill the border
		pLcd->SetRegion(Lcd::Rect(4,5,120,1));
		pLcd->FillColor(0x0000);
		pLcd->SetRegion(Lcd::Rect(4,5,1,150));
		pLcd->FillColor(0x0000);
		pLcd->SetRegion(Lcd::Rect(124,5,1,150));
		pLcd->FillColor(0x0000);
		pLcd->SetRegion(Lcd::Rect(4,155,120,1));
		pLcd->FillColor(0x0000);
	}

	void drawBorder(libsc::St7735r* pLcd){
		pLcd->SetRegion(Lcd::Rect(4,5,120,1));
		pLcd->FillColor(0x0000);
		pLcd->SetRegion(Lcd::Rect(4,5,1,150));
		pLcd->FillColor(0x0000);
		pLcd->SetRegion(Lcd::Rect(124,5,1,150));
		pLcd->FillColor(0x0000);
		pLcd->SetRegion(Lcd::Rect(4,155,120,1));
		pLcd->FillColor(0x0000);
	}

};


#endif /* PONG_H_ */
