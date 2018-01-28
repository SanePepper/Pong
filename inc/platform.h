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

	Platform() : Sprite(30,4,0x0000,0xFFFF) {}

    explicit Platform(libsc::Lcd* pLcd);

    /**
     * Moves the platform one step to the left.
     */
    void moveLeft(){
    	m_position.x -= 4;
    	m_position.x < 4? 4: m_position.x;
    }

    /**
     * Moves the platform one step to the right.
     */
    void moveRight(){
    	m_position.x += 4;
    	m_position.x > 124? 124: m_position.x;
    }

    /**
     * Renders the platform.
     */
    void render() override{}

};


#endif //INC_PLATFORM_H
