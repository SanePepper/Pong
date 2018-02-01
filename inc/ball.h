//
// Created by Daniel on 24/12/2017.
//

#ifndef INC_BALL_H
#define INC_BALL_H


#include "sprite.h"
#include "config.h"
#include <libsc/system.h>
//#include "game_config.h"

/**
 * Ball sprite for the Pong game.
 */
class Ball: public Sprite {
public:

//    explicit Ball(libsc::Lcd* pLcd) : Sprite(Config::GetSpriteConfig(0xFC40, 0xFFFF, 5, 5, 64, 80, pLcd)){
    explicit Ball(Config config) : Sprite(config){}

    /**
     * Sets velocity of the ball.
     * @param v_x Velocity in x direction.
     * @param v_y Velocity in y direction.
     */
    void setVelocity(int v_x, int v_y){
    	m_v_x = v_x;
    	m_v_y = v_y;
    }

    /**
     * Calculate movements for the next frame.
     */
    void move(int yourPlatform_x, int opponentPlatform_x){
    	clear();
    	m_position.x += m_v_x;
    	m_position.y += m_v_y;
    	rebound(yourPlatform_x, opponentPlatform_x);
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
    int m_v_x = -4;

    /**
     * Velocity in Y direction of the ball in pixels.
     */
    int m_v_y = 7;

    int win = 0;

    int lose = 0;

    /**
     * Executes rebound logic.
     */
    void rebound(int yourPlatform_x, int opponentPlatform_x){
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
    	}
    	if (m_position.y < 5 || m_position.y > 154){
			if (m_position.y < 5){
				//you win this round
				this->m_pLcd->SetRegion(Lcd::Rect(116-win*10,156,6,4));
				this->m_pLcd->FillColor(0x0EE0);
				win++;
//				if (win == 3){
//					end_game = true;
//				}
			}
			if (m_position.y > 154){
				//you lose this round
				this->m_pLcd->SetRegion(Lcd::Rect(116-lose*10,0,6,4));
				this->m_pLcd->FillColor(0x0EE0);
				lose++;
//				if (lose == 3){
//					end_game = true;
//				}
			}
			setPosition(64,80);
			setVelocity(-4,7);
			libsc::System::DelayS(2);
    	}
    }
	//process ball hit wall, win condition, platform
};


#endif //INC_BALL_H
