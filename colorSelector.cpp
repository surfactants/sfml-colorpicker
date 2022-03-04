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

#include "colorSelector.h"

///////////////////////////////////
//BEGIN COLOR_SELECTOR IMPLEMENTATION
///////////////////////////////////

Color_Selector::Color_Selector(){
    alpha = 255;
    sf::Vector2f size = sf::Vector2f(256,256);

    colors.setPrimitiveType(sf::Points);
    colors.resize(256*256);


    selector = sf::RoundedRectangleShape(sf::Vector2f(8, 8), 2, 4);
        selector.setSize(sf::Vector2f(8,8));
        selector.setOrigin(sf::Vector2f(selector.getSize().x/2, selector.getSize().y/2));
        selector.setOutlineThickness(1);
        selector.setOutlineColor(sf::Color(0,0,0));

    selecting = false;

    slider.setPrimitiveType(sf::Quads);
    slider.resize(24);
        sf::Color slider_color = sf::Color(255,0,0);
            slider[0].color = slider_color;
            slider[1].color = slider_color;
        slider_color = sf::Color(255,255,0);
            slider[2].color = slider_color;
            slider[3].color = slider_color;
            slider[4].color = slider_color;
            slider[5].color = slider_color;
        slider_color = sf::Color(0,255,0);
            slider[6].color = slider_color;
            slider[7].color = slider_color;
            slider[8].color = slider_color;
            slider[9].color = slider_color;
        slider_color = sf::Color(0,255,255);
            slider[10].color = slider_color;
            slider[11].color = slider_color;
            slider[12].color = slider_color;
            slider[13].color = slider_color;
        slider_color = sf::Color(0,0,255);
            slider[14].color = slider_color;
            slider[15].color = slider_color;
            slider[16].color = slider_color;
            slider[17].color = slider_color;
        slider_color = sf::Color(255,0,255);
            slider[18].color = slider_color;
            slider[19].color = slider_color;
            slider[20].color = slider_color;
            slider[21].color = slider_color;
        slider_color = sf::Color(255,0,0);
            slider[22].color = slider_color;
            slider[23].color = slider_color;

    setPosition(sf::Vector2f(16,16));

    slider_handle = sf::RoundedRectangleShape(sf::Vector2f(slider[1].position.x-slider[0].position.x, 12), 8, 8);
    slider_handle.setPosition(slider[0].position);
    slider_handle.setOrigin(sf::Vector2f(0, slider_handle.getSize().y/2));
    slider_handle.setFillColor(sf::Color(250,250,250,250));
    sliding = false;


    setHue();
}

void Color_Selector::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    target.draw(colors, states);
        target.draw(selector, states);

    target.draw(slider, states);
        target.draw(slider_handle, states);

    target.draw(selected);
}

void Color_Selector::reset(){
    slider_handle.setPosition(slider[0].position);
    selector.setPosition(colors[0].position);
    setHue();
}

void Color_Selector::setPosition(sf::Vector2f pos){
    sf::Vector2f size = sf::Vector2f(256,256);

    for(unsigned int y = 0; y < 256; y++){
        for(unsigned int x = 0; x < 256; x++){
            colors[x + y*256].position.x = pos.x + x;
            colors[x + y*256].position.y = pos.y + y;
        }
    }


    sf::Vector2f slider_pos = sf::Vector2f(pos.x + size.x + 8, pos.y);
    sf::Vector2f slider_size = sf::Vector2f(16, size.y/6);

    int siter = 0;
    while(siter < slider.getVertexCount()){
        slider[siter++].position = slider_pos;
            slider_pos.x += slider_size.x;
        slider[siter++].position = slider_pos;
            slider_pos.y += slider_size.y;
        slider[siter++].position = slider_pos;
            slider_pos.x -= slider_size.x;
        slider[siter++].position = slider_pos;
    }

    slider_handle.setPosition(slider[0].position);

    sf::Vector2f selected_pos = sf::Vector2f(colors[0].position.x, colors[256*256-1].position.y + 8);
    sf::Vector2f selected_size = sf::Vector2f(slider[1].position.x - colors[0].position.x, 32);

    selected.setPosition(selected_pos);
    selected.setSize(selected_size);

    setHue();
}

void Color_Selector::setAlpha(unsigned int a){
    alpha = a;
    setHue();
}

sf::Color Color_Selector::getColor(){
    return selected_color;
}

void Color_Selector::readEvent(sf::Event& event){
    sf::Vector2i mousePos = sf::Mouse::getPosition();
    if(event.type == sf::Event::MouseButtonReleased){
        selecting = false;
        sliding = false;
    }

    if(slider.getBounds().contains(mousePos.x,mousePos.y)){
        if(event.type == sf::Event::MouseButtonPressed){
            sliding = true;
            slide(mousePos);
        }
    }
    else if(colors.getBounds().contains(mousePos.x,mousePos.y)){
        if(event.type == sf::Event::MouseButtonPressed) selecting = true;
    }

    if(event.type == sf::Event::MouseMoved){
        if(selecting) select(mousePos);
        else if(sliding) slide(mousePos);
    }
}

void Color_Selector::slide(sf::Vector2i mousePos){
    slider_handle.setPosition(sf::Vector2f(slider[0].position.x, mousePos.y));

    if(slider_handle.getPosition().y < slider[0].position.y){
        slider_handle.setPosition(sf::Vector2f(slider_handle.getPosition().x, slider[0].position.y));
    }
    else if(slider_handle.getPosition().y > slider[23].position.y){
        slider_handle.setPosition(sf::Vector2f(slider_handle.getPosition().x, slider[23].position.y));
    }

    setHue();
}

void Color_Selector::setHue(){
    const float hue = (slider_handle.getPosition().y - slider[0].position.y)/(slider[23].position.y - slider[0].position.y); /**<Hue, 0-1*/
    const int hue_point = hue * 6; /**<Determines the hue's color domain*/
    const float hue_remainder = 1.f - std::abs(fmod(hue*6.f, 2.f) - 1.f); /**<used in the calculation of the intermediate value*/
    for(unsigned int y = 0; y < 256; y++){
        for(unsigned int x = 0; x < 256; x++){
            const float sat = x / 255.f; /**<saturation*/
            const float val = 1 - y / 255.f; /**<value*/

            float c = val * sat; /**<chroma*/
            float z = c * hue_remainder; /**<intermediate value*/
            float m = val - c; /**<match*/
            c += m;
                c *= 255;
            z += m;
                z *= 255;

            switch(hue_point){
            case 0:
            case 6: //(C,Z,0)
                colors[y * 256 + x].color = sf::Color(c, z, m*255, alpha);
                break;
            case 5: //(C,0,Z)
                colors[y * 256 + x].color = sf::Color(c, m*255, z, alpha);
                break;
            case 4: //(Z,0,C)
                colors[y * 256 + x].color = sf::Color(z, m*255, c, alpha);
                break;
            case 3: //(0,Z,C)
                colors[y * 256 + x].color = sf::Color(m*255, z, c, alpha);
                break;
            case 2: //(0,C,Z)
                colors[y * 256 + x].color = sf::Color(m*255, c, z, alpha);
                break;
            case 1: //(Z,C,0)
                colors[y * 256 + x].color = sf::Color(z, c, m*255, alpha);
                break;
            }
        }
    }

    slider_handle.setFillColor(colors[255].color);

    select(sf::Vector2i(selector.getPosition().x, selector.getPosition().y));
}

void Color_Selector::select(sf::Vector2i mousePos){
    selector.setPosition(mousePos.x,mousePos.y);

    if(selector.getPosition().x < colors[0].position.x){
        selector.setPosition(colors[0].position.x, selector.getPosition().y);
    }
    else if(selector.getPosition().x > colors[256*256-1].position.x){
        selector.setPosition(colors[256*256-1].position.x, selector.getPosition().y);
    }

    if(selector.getPosition().y < colors[0].position.y){
        selector.setPosition(selector.getPosition().x, colors[0].position.y);
    }
    else if(selector.getPosition().y > colors[256*256-1].position.y){
        selector.setPosition(selector.getPosition().x, colors[256*256-1].position.y);
    }

    selected_color = colors[(selector.getPosition().y - colors[0].position.y) * 256 + selector.getPosition().x - colors[0].position.x].color;
    selected.setFillColor(selected_color);
    selector.setFillColor(selected_color);
}

///////////////////////////////////
//END COLOR_SELECTOR IMPLEMENTATION
///////////////////////////////////
