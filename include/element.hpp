#ifndef ELEMENT_H
#define ELEMENT_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Element {
public:
  float value;
  sf::Color color = {255, 255, 255};

  Element() : value(0) {};
  Element(int v) : value(v) {};

  void swap(Element &e) {
    float temp = value;
    value = e.value;
    e.value = temp;

    sf::Color aux = color;
    color = e.color;
    e.color = aux;
  };

  bool operator<(Element const &e) { return value < e.value; };
  bool operator>(Element const &e) { return value > e.value; };
  bool operator<=(Element const &e) { return value <= e.value; };
  bool operator>=(Element const &e) { return value >= e.value; };

  void draw(sf::RenderWindow &window, int position, float barWidth) {
    float height = value * window.getSize().y;
    sf::RectangleShape rect = sf::RectangleShape({});

    rect.setSize({barWidth / 2, height});
    rect.setPosition({barWidth * position / 2, window.getSize().y - height});
    rect.setFillColor(color);
    window.draw(rect);
  };
};

#endif // !ELEMENT_H
