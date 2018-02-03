//
// Created by Daniel on 24/12/2017.
//

#ifndef INC_BALL_H
#define INC_BALL_H


#include "sprite.h"
#include "config.h"
#include <libsc/system.h>
#include "score.h"

/**
 * Ball sprite for the Pong game.
 */
class Ball: public Sprite {
public:

    explicit Ball(Config config, Score* score) : Sprite(config), pScore(score){}

    /**
     * Sets velocity of the ball.
     * @param v_x Velocity in x direction.
     * @param v_y Velocity in y direction.
     */
    void setVelocity(int8_t v_x, int8_t v_y){
    	m_v_x = v_x;
    	m_v_y = v_y;
    }

    std::pair<int8_t,int8_t> getVelocity(){
    	std::pair<int8_t,int8_t>temp;
    	temp.first = m_v_x;
    	temp.second = m_v_y;
    	return temp;
    }

    /**
     * Calculate movements for the next frame.
     */
    bool move(Byte yourPlatform_x, Byte opponentPlatform_x){
    	clear();
    	m_position.x += m_v_x;
    	m_position.y += m_v_y;
    	bool return_val = rebound(yourPlatform_x, opponentPlatform_x);
    	draw();
    	return return_val;
    }

    void draw(){
	    this->m_pLcd->SetRegion(Lcd::Rect(m_position.x-m_width/2, m_position.y-m_height/2, m_width, m_height));
	    this->m_pLcd->FillColor(m_fg_color);
    }

    void clear(){
	    this->m_pLcd->SetRegion(Lcd::Rect(m_position.x-m_width/2, m_position.y-m_height/2, m_width, m_height));
	    this->m_pLcd->FillColor(m_bg_color);
    }

    /**
     * Renders the ball.
     */
    void render() override{
//    	clear();
    	//change position of the entity
    	draw();
    }

private:

    /**
     * Velocity in X direction of the ball in pixels.
     */
    int8_t m_v_x = -4;

    /**
     * Velocity in Y direction of the ball in pixels.
     */
    int8_t m_v_y = 7;

    int win = 0;

    int lose = 0;

    Score* pScore = nullptr;
    /**
     * Executes rebound logic.
     */
    bool rebound(int yourPlatform_x, int opponentPlatform_x){
    	//hit wall
    	int contactPoint_x, contactPoint_y, dx, dy, px, py;
    	px = m_position.x - m_v_x;
    	py = m_position.y - m_v_y;
    	if (m_position.x < 4 || m_position.x > 123){
        	if (m_position.x < 4){
        		dx = 4 - px;
        	}
        	else{
        		dx = 123 - px;
        	}
       		dy = dx * m_v_y / m_v_x; //result should be int, floored
       		contactPoint_x = px + dx;
       		contactPoint_y = py + dy;
       		m_position.x = contactPoint_x + ((m_v_x-dx)/m_v_x)*(-m_v_x);
       		m_position.y = contactPoint_y + ((m_v_y-dy)/m_v_y)*(m_v_y);
    		setVelocity(-m_v_x,m_v_y);
    		return true;
    	}
    	//hit platform
    	if((m_position.y < 11 && opponentPlatform_x <= px && px <= opponentPlatform_x + 29) || (m_position.y > 148 && yourPlatform_x <= px && px <= yourPlatform_x + 29)){ // && (m_position.x >= )
        	int new_v_x;
        	if (m_position.y < 11){
        		dy = 11 - py;
           		new_v_x = (px-opponentPlatform_x)/2 - 7;
        	}
        	else{
        		dy = 148 - py;
           		new_v_x = (px-yourPlatform_x)/2 - 7;
        	}
       		dx = dy * m_v_x / m_v_y; //result should be int, floored
       		contactPoint_x = px + dx;
       		contactPoint_y = py + dy;
       		m_position.x = contactPoint_x + ((m_v_x-dx)/m_v_x)*(new_v_x);
       		m_position.y = contactPoint_y + ((m_v_y-dy)/m_v_y)*(-m_v_y);
    		setVelocity(new_v_x,-m_v_y);
    		return true;
    	}
    	if (m_position.y < 5 || m_position.y > 154){
    		Score score;
			if (m_position.y < 5){
				//you win this round
				this->m_pLcd->SetRegion(Lcd::Rect(116-pScore->getWin()*10,156,6,4));
				this->m_pLcd->FillColor(0x0EE0);
				pScore->Win();
			}
			if (m_position.y > 154){
				//you lose this round
				this->m_pLcd->SetRegion(Lcd::Rect(116-pScore->getLose()*10,0,6,4));
				this->m_pLcd->FillColor(0x0EE0);
				pScore->Lose();
			}
			setPosition(64,80);
			setVelocity(-4,7);
    	}
    	return false;
    }
};


#endif //INC_BALL_H
