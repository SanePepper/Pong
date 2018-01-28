//
// Created by Daniel on 24/12/2017.
//

#ifndef INC_BALL_H
#define INC_BALL_H


#include "sprite.h"
//#include "game_config.h"

/**
 * Ball sprite for the Pong game.
 */
class Ball: public Sprite {
public:

	Ball() : Sprite(5,5,0xFC40,0x0000){
		Sprite::setPosition(0,0);
	}

    explicit Ball(libsc::Lcd* pLcd);

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
    void move(){
    	m_position.x += m_v_x;
    	m_position.y += m_v_y;
    }

    /**
     * Renders the ball.
     */
    void render() override{}

private:

    /**
     * Velocity in X direction of the ball in pixels.
     */
    int m_v_x = 0;

    /**
     * Velocity in Y direction of the ball in pixels.
     */
    int m_v_y = 7;

    /**
     * Executes rebound logic.
     */
    void rebound();
	//process ball hit wall, win condition, platform
};


#endif //INC_BALL_H
