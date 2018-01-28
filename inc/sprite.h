//
// Created by Daniel on 27/12/2017.
//

#ifndef INC_SPRITE_H
#define INC_SPRITE_H


#include <cstdint>
#include <vector>
#include <libsc/lcd.h>

/**
 * Coordinate data.
 */
struct Coord {
    int x;
    int y;

    Coord(): x(0), y(0) {}

    Coord(int _x, int _y): x(_x), y(_y) {}
};

/**
 * Rectangular blob for a sprite. Used for extending to become a more specialized derived sprite.
 */
class Sprite {
public:
//    void setConfig(int width, int height, uint16_t fg_color, uint16_t bg_color){
//    	m_width = width;
//    	m_height = height;
//    	m_fg_color = fg_color;
//    	m_bg_color = bg_color;
//    }

	Sprite(int width, int height, uint16_t fg_color, uint16_t bg_color)
		: m_width(width), m_height(height), m_fg_color(fg_color), m_bg_color(bg_color){

	}

    /**
     * A config structure for constructing a sprite.
     */
    struct Config {
        uint16_t fg_color = 0x0000;
        uint16_t bg_color = 0xFFFF;
        unsigned int width = 10;
        unsigned int height = 10;
        int x = 10;
        int y = 10;
        libsc::Lcd* pLcd = nullptr;
    };

    /**
     * Constructor of sprite which accepts a sprite config.
     * @param config Config object to initialize default values.
     */
    explicit Sprite(Config config);

    /**
     * Renders the sprite
     */
    virtual void render() = 0;

    /**
     * Sets position of sprite using 2 integers x and y.
     * @param x Position x.
     * @param y Position y.
     */
    void setPosition(int x, int y){
    	m_position.x = x;
    	m_position.y = y;
    }

    /**
     * Sets position of sprite using a Coord.
     * @param coord Coordinate containing position x and y.
     */
    void setPosition(Coord coord){
    	m_position = coord;
    }

    /**
     * Gets position of the sprite.
     * @return position of sprite with Coord.
     */
    Coord getPosition(){
    	return m_position;
    }

protected:
    /**
     * Width of a sprite.
     */
    const unsigned int m_width;

    /**
     * Height of a sprite.
     */
    const unsigned int m_height;

    /**
     * Fill color of a sprite.
     * Color in RGB565 representation.
     */
    const uint16_t m_fg_color;

    /**
     * Background color of a sprite.
     * Color in RGB565 representation.
     */
    const uint16_t m_bg_color;

    /**
     * Current position of the sprite.
     */
    Coord m_position;

    /**
     * Pointer to the LCD object for independent rendering.
     */
    libsc::Lcd* m_pLcd;

    /**
     * Draws the sprite in the new frame.
     */
    virtual void draw(){}

    /**
     * Removes the sprite with the previous location in the new frame.
     */
    virtual void clean(){}
};


#endif //INC_SPRITE_H
