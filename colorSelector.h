////////////////////////////////////////////////////////////
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
// you must not claim that you wrote the original software.
// If you use this software in a product, an acknowledgment
// in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
// and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////
#ifndef COLORSELECT_H
#define COLORSELECT_H

#include <SFML/Graphics.hpp>
#include <iostream>

////////////////////////////////////////////////////////////
/// \brief Color selection class
///
class Color_Selector : public sf::Drawable{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor. Creates the frame and slider,
    /// and places them at (16, 16)
    ///
    Color_Selector();

    ////////////////////////////////////////////////////////////
    /// \brief Sets hue to 0 and returns selector to start position.
    ///
    void reset();

    ////////////////////////////////////////////////////////////
    /// \brief Places selector and hue slider.
    ///
    /// \param pos top left position
    ///
    void setPosition(sf::Vector2f pos);

    ////////////////////////////////////////////////////////////
    /// \brief Sets transparency.
    ///
    /// \param a -> alpha
    ///
    void setAlpha(unsigned int a);

    ////////////////////////////////////////////////////////////
    /// \brief Returns the selected color.
    ///
    /// \return selected_color
    ///
    sf::Color getColor();

    ////////////////////////////////////////////////////////////
    /// \brief Main event loop, checks for mouse pos, click, and release.
    ///
    /// \param event reference to event object created in main loop
    ///
    void readEvent(sf::Event& event);

private:
    unsigned int alpha; /**<Transparency*/

    sf::VertexArray colors; /**<256x256 grid for HSV handling*/
        sf::RectangleShape selector; /**<Handle for color selection*/
        bool selecting; /**<For tracking mouse events*/

    sf::VertexArray slider; /**<Hue selector*/
        sf::RectangleShape slider_handle; /**<Handle for hue selection*/
        bool sliding; /**<For tracking mouse events*/

    sf::Color selected_color; /**<Tracks selected color*/
        sf::RectangleShape selected; /**<Example object*/

    ////////////////////////////////////////////////////////////
    /// \brief Places hue slider according to the mouse position,
    /// and calls setHue().
    /// Internal.
    ///
    /// \brief mousePos -> slider_handle.setPosition(), within bounds of slider
    ///
    void slide(sf::Vector2i mousePos);

    ////////////////////////////////////////////////////////////
    /// \brief Sets the frame to a new hue based on slider position,
    /// and recalls select().
    /// Internal.
    ///
    void setHue();

    ////////////////////////////////////////////////////////////
    /// \brief Places the selector.
    /// Internal.
    ///
    /// \param mousePos -> selector.setPosition(), within bounds of colors
    ///
    void select(sf::Vector2i mousePos);

    ////////////////////////////////////////////////////////////
    /// \brief Draws the frame, slider, and handles to the render target.
    /// Internal.
    ///
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif
