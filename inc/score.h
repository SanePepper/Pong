/*
 * score.h
 *
 *  Created on: 1 Feb 2018
 *      Author: PWS
 */

#include <libsc/system.h>

#ifndef INC_SCORE_H_
#define INC_SCORE_H_

class Score{
public:
	int getWin(){
		return m_win;
	}

	int getLose(){
		return m_lose;
	}

	void Win(){
		m_win++;
		goalTime = libsc::System::Time();
	}

	void Lose(){
		m_lose++;
		goalTime = libsc::System::Time();
	}

	libsc::Timer::TimerInt getTime(){
		return goalTime;
	}

private:
	int m_win = 0;
	int m_lose = 0;
	libsc::Timer::TimerInt goalTime = libsc::System::Time();
};



#endif /* INC_SCORE_H_ */
