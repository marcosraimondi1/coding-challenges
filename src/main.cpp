#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

void drawGui();

int main() {
  auto window = sf::RenderWindow(sf::VideoMode(sf::VideoMode::getDesktopMode()),
                                 "Fourier Visualization");
  window.setFramerateLimit(144);
  ImGui::SFML::Init(window);
  sf::Clock clock;

  while (window.isOpen()) {
    for (auto event = sf::Event(); window.pollEvent(event);) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      ImGui::SFML::ProcessEvent(event);
    }

    sf::Time elapsed = clock.restart();
    ImGui::SFML::Update(window, elapsed);

    drawGui();

    window.clear();
    ImGui::SFML::Render(window);

    window.display();
  }
  ImGui::SFML::Shutdown();
}

void drawGui() {
  ImGui::Begin("Tools");
  ImGui::End();
}
