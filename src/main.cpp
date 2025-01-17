#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <imgui-SFML.h>
#include <imgui.h>

void drawGui(float &freq);

int main() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  auto window =
      sf::RenderWindow(sf::VideoMode(sf::VideoMode::getDesktopMode()),
                       "Fourier Visualization", sf::Style::Default, settings);
  window.setFramerateLimit(144);
  ImGui::SFML::Init(window);
  sf::Clock clock;

  sf::CircleShape other = sf::CircleShape(50);
  sf::CircleShape circle = sf::CircleShape(100, 50);

  other.setOrigin(50, 50);
  circle.setOrigin(100, 100);

  circle.setPosition({200, 200});
  other.setPosition({300, 200});

  circle.setFillColor(sf::Color::Transparent);
  circle.setOutlineThickness(2);
  other.setFillColor(sf::Color::Transparent);
  other.setOutlineThickness(1);

  float freq = 1;
  float amplitude = 100;
  float time = 0;
  float phase = 0;

  sf::VertexArray wave = sf::VertexArray(sf::PrimitiveType::LineStrip, 400);
  sf::VertexArray line = sf::VertexArray(sf::PrimitiveType::LineStrip, 2);

  while (window.isOpen()) {
    for (auto event = sf::Event(); window.pollEvent(event);) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      ImGui::SFML::ProcessEvent(event);
    }

    sf::Time elapsed = clock.restart();
    ImGui::SFML::Update(window, elapsed);

    drawGui(freq);

    time += elapsed.asSeconds();
    float x = amplitude * std::cos(time * freq + phase);
    float y = amplitude * std::sin(time * freq + phase);
    other.setPosition(x + 200, y + 200);

    for (int i = wave.getVertexCount() - 1; i > 0; i--) {
      wave[i].position.y = wave[i - 1].position.y;
      wave[i].position.x = 400 + i;
    }
    wave[0].position = sf::Vector2f({400, y + 200});

    line[0].position = other.getPosition();
    line[1].position = wave[0].position;

    window.clear();
    ImGui::SFML::Render(window);

    window.draw(other);
    window.draw(circle);
    window.draw(wave);
    window.draw(line);

    window.display();
  }
  ImGui::SFML::Shutdown();
}

void drawGui(float &freq) {
  ImGui::Begin("Tools");
  ImGui::SliderFloat("Freq", &freq, 0, 10);
  ImGui::End();
}
