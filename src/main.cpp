#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <imgui-SFML.h>
#include <imgui.h>

void drawGui(float &freq, int &nWaves, float &amplitude);

int main() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  auto window =
      sf::RenderWindow(sf::VideoMode(sf::VideoMode::getDesktopMode()),
                       "Fourier Visualization", sf::Style::Default, settings);
  window.setFramerateLimit(144);
  ImGui::SFML::Init(window);
  sf::Clock clock;

  int nWaves = 10;
  int nextSize = nWaves;
  float freq = 1;
  float time = 0;

  sf::VertexArray wave = sf::VertexArray(sf::PrimitiveType::LineStrip, 500);
  sf::VertexArray line = sf::VertexArray(sf::PrimitiveType::LineStrip, 2);

  sf::Vector2f initialPosition = {100, 400};
  float amplitude = 50;
  float nextAmplitude = amplitude;

  std::vector<sf::CircleShape> series;
  series.resize(nWaves);

  int n = 1;
  for (auto &s : series) {
    s.setRadius(amplitude / n);
    s.setOrigin({s.getRadius(), s.getRadius()});
    s.setPosition(initialPosition);
    s.setFillColor(sf::Color::Transparent);
    s.setOutlineThickness(1);
    n += 2;
  }

  while (window.isOpen()) {
    for (auto event = sf::Event(); window.pollEvent(event);) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      ImGui::SFML::ProcessEvent(event);
    }

    sf::Time elapsed = clock.restart();
    ImGui::SFML::Update(window, elapsed);

    drawGui(freq, nextSize, nextAmplitude);
    if (nextSize != nWaves || nextAmplitude != amplitude) {
      amplitude = nextAmplitude;
      if (nextSize < 0) {
        nextSize = nWaves;
      }

      nWaves = nextSize;
      series.resize(nWaves);
      int n = 1;
      for (auto &s : series) {
        s.setRadius(amplitude / n);
        s.setOrigin({s.getRadius(), s.getRadius()});
        s.setPosition(initialPosition);
        s.setFillColor(sf::Color::Transparent);
        s.setOutlineThickness(1);
        n += 2;
      }
    }

    time += elapsed.asSeconds();

    n = 1;
    float x = initialPosition.x, y = initialPosition.y;

    for (auto &s : series) {
      x += s.getRadius() * std::cos(n * time * freq);
      y += s.getRadius() * std::sin(n * time * freq);

      sf::Vector2f newPos = {x, y};

      s.setPosition(newPos);
      n += 2;
    }

    for (int i = wave.getVertexCount() - 1; i > 0; i--) {
      wave[i].position.y = wave[i - 1].position.y;
      wave[i].position.x = wave[i - 1].position.x + 1;
    }
    wave[0].position.x = initialPosition.x + 200;
    wave[0].position.y = y;

    line[0].position = series.back().getPosition();
    line[1].position = wave[0].position;

    window.clear();
    ImGui::SFML::Render(window);

    for (auto &s : series)
      window.draw(s);

    window.draw(wave);
    window.draw(line);

    window.display();
  }
  ImGui::SFML::Shutdown();
}

void drawGui(float &freq, int &nWaves, float &amplitude) {
  ImGui::Begin("Tools");
  ImGui::SliderFloat("Freq", &freq, 0, 10);
  ImGui::SliderFloat("Amp", &amplitude, 1, 100);
  ImGui::InputInt("Nwaves", &nWaves);
  ImGui::End();
}
