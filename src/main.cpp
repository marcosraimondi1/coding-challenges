#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <thread>
#include <unistd.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define MAX_BARS 1000

#define BAR_WIDTH WINDOW_WIDTH / MAX_BARS
#define BAR_STEP WINDOW_HEIGHT / MAX_BARS

void bubbleSort(int array[MAX_BARS]);
void quickSort(int array[MAX_BARS], int start, int end);
void mergeSort(int array[MAX_BARS], int start, int end);

int colors[MAX_BARS] = {};
int delay_ms = 0;
bool isSorting = false;

int main() {
  auto window = sf::RenderWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
                                 "Sorting Algorithms");
  window.setFramerateLimit(144);

  ImGui::SFML::Init(window);
  sf::Clock clock;
  std::srand(time(NULL));

  int array[MAX_BARS] = {};
  for (int &e : array) {
    e = rand() % MAX_BARS;
  }

  const char *algorithmsList[] = {"bubbleSort", "quickSort", "mergeSort"};
  static const char *currentItem = algorithmsList[0];

  std::thread sortingThread;
  float sortingTime = 0;

  while (window.isOpen()) {
    for (auto event = sf::Event(); window.pollEvent(event);) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      ImGui::SFML::ProcessEvent(event);
    }

    sf::Time elapsed = clock.restart();

    if (isSorting)
      sortingTime += elapsed.asSeconds();

    ImGui::SFML::Update(window, elapsed);

    ImGui::Begin("Tools");

    if (ImGui::BeginCombo("##combo", currentItem)) {
      for (int i = 0; i < IM_ARRAYSIZE(algorithmsList); i++) {
        bool is_selected = (currentItem == algorithmsList[i]);

        if (ImGui::Selectable(algorithmsList[i], is_selected))
          currentItem = algorithmsList[i];

        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    ImGui::SliderInt("Delay", &delay_ms, 0, 200);
    if (ImGui::Button("Reset")) {
      for (int &e : array) {
        e = rand() % MAX_BARS;
      }
    }

    if (ImGui::Button("Sort")) {
      if (!isSorting) {
        isSorting = true;
        sortingTime = 0;
        if (currentItem == algorithmsList[0])
          sortingThread = std::thread(bubbleSort, array);
        else if (currentItem == algorithmsList[1])
          sortingThread = std::thread(quickSort, array, 0, MAX_BARS - 1);
        else if (currentItem == algorithmsList[2])
          sortingThread = std::thread(mergeSort, array, 0, MAX_BARS - 1);
        sortingThread.detach();
      }
    }

    ImGui::Text("Time: %.2f s", sortingTime);

    ImGui::End();

    window.clear();

    int barWidth = window.getSize().x / MAX_BARS;
    int barStep = window.getSize().y / MAX_BARS;

    int x = -barWidth;
    int y = 0;
    for (int i = 0; i < MAX_BARS; i++) {
      sf::RectangleShape rectangle;
      rectangle.setOutlineThickness(0);
      int e = array[i];

      rectangle.setSize({barWidth, e * barStep});
      rectangle.setPosition({x + barWidth, y});
      x += barWidth;

      switch (colors[i]) {
      case 1:
        rectangle.setFillColor(sf::Color(255, 0, 0));
        break;
      case 2:
        rectangle.setFillColor(sf::Color(0, 255, 0));
        break;
      default:
        rectangle.setFillColor(sf::Color(255, 255, 255));
        break;
      }

      window.draw(rectangle);
    }

    ImGui::SFML::Render(window);

    window.display();
  }
  ImGui::SFML::Shutdown();
}

void swap(int &a, int &b) {
  int temp = a;
  a = b;
  b = temp;
}

void bubbleSort(int array[MAX_BARS]) {
  for (int i = MAX_BARS - 1; i > 0; i--) {
    for (int j = 0; j < i; j++) {
      if (array[j] > array[j + 1]) {
        colors[j] = 1;
        swap(array[j], array[j + 1]);
        usleep(1000 * delay_ms);
        colors[j] = 0;
      }
    }
  }
  isSorting = false;
}

// count how many numbers are smaller than the pivot
// put those numbers at the beginning of the array
// put the pivot after those numbers
// return pivot final position
int partition(int array[MAX_BARS], int start, int end) {
  int pivot = array[end]; // select last element as pivot
  int count = 0;          // start less count at 0

  colors[end] = 1;

  for (int i = start; i < end; i++) {
    colors[i] = 2;
    usleep(1000 * delay_ms / 2);
    if (array[i] <= pivot) {
      swap(array[i], array[start + count]);
      count++;
    }
    usleep(1000 * delay_ms / 2);
    colors[i] = 0;
  }

  swap(array[end], array[start + count]); // put pivot into position

  colors[end] = 0;

  return start + count;
}

// optimized partition, find middle point comparing both ends
// return pivot final position
int partition_opt(int array[MAX_BARS], int start, int end) {
  int i = start, j = end - 1;
  colors[end] = 1;
  int pivot = array[end];

  while (i <= j) {
    colors[i] = 2;
    colors[j] = 2;
    usleep(1000 * delay_ms);

    if (array[i] >= pivot && array[j] <= pivot) {
      swap(array[i], array[j]);
      colors[i] = 0;
      colors[j] = 0;
      i++;
      j--;
    }

    if (array[i] < pivot) {
      colors[i] = 0;
      i++;
    }

    if (array[j] > pivot) {
      colors[j] = 0;
      j--;
    }
  }

  swap(array[end], array[i]);

  usleep(1000 * delay_ms);

  colors[i] = 0;
  colors[j] = 0;
  colors[end] = 0;

  return i;
}

void quickSort(int array[MAX_BARS], int start, int end) {
  if (start >= end)
    return;

  int pivotIndex = partition_opt(array, start, end);

  quickSort(array, start, pivotIndex - 1); // left subarray
  quickSort(array, pivotIndex + 1, end);   // right subarray

  if (start == 0 && end == MAX_BARS - 1)
    isSorting = false;
}

void merge(int array[MAX_BARS], int start, int end, int middle) {
  int i = start;            // left subarray index
  int j = middle + 1;       // rigth subarray index
  int cpy[end - start + 1]; // auxiliary copy
  int index = 0;            // auxiliary index

  while (true) {
    if (array[i] < array[j])
      cpy[index] = array[i++];
    else
      cpy[index] = array[j++];

    index++;

    if (i > middle) {
      // add all j
      for (int n = j; n < end + 1; n++)
        cpy[index++] = array[n];

      break;
    }

    if (j > end) {
      // add all i
      for (int n = i; n < middle + 1; n++)
        cpy[index++] = array[n];

      break;
    }
  }

  for (int i = start; i < end + 1; i++)
    colors[i] = 1;

  for (int i = start; i < end + 1; i++) {
    usleep(1000 * delay_ms / 2);
    array[i] = cpy[i - start];
  }

  for (int i = start; i < end + 1; i++)
    colors[i] = 0;
}

void mergeSort(int array[MAX_BARS], int start, int end) {
  if (start >= end)
    return;

  // split array
  int middle = (start + end) / 2;

  mergeSort(array, start, middle);
  mergeSort(array, middle + 1, end);

  // merge
  merge(array, start, end, middle);

  if (start == 0 && end == MAX_BARS - 1)
    isSorting = false;
}
