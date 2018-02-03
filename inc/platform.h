//
// Created by Daniel on 24/12/2017.
//

#ifndef INC_PLATFORM_H
#define INC_PLATFORM_H


#include "sprite.h"
//#include "game_config.h"

/**
 * Platform Sprite for the Pong game.
 */
class Platform: public Sprite {
public:

    explicit Platform(Config config):Sprite(config){}

    void setX(Byte x){
    	clear();
    	m_position.x = x;
    }

    /**
     * Moves the platform one step to the left.
     */
    void moveLeft(){
    	if (m_position.x-4 >= 4){
        	clear();
    		m_position.x -= 4;
    	}
    }

    /**
     * Moves the platform one step to the right.
     */
    void moveRight(){
    	if (m_position.x+30+4 <= 124){
        	clear();
    		m_position.x += 4;
    	}
    }

    void draw(){
	    this->m_pLcd->SetRegion(Lcd::Rect(m_position.x, m_position.y, m_width, m_height));
	    this->m_pLcd->FillColor(m_fg_color);
    }

    void clear(){
	    this->m_pLcd->SetRegion(Lcd::Rect(m_position.x, m_position.y, m_width, m_height));
	    this->m_pLcd->FillColor(m_bg_color);
    }
    /**
     * Renders the platform.
     */
    void render() override{
    	draw();
    }

};


#endif //INC_PLATFORM_H
