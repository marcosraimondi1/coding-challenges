#include "element.hpp"
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <thread>
#include <unistd.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define MAX_BARS 1000

// sorting algorithms
void bubbleSort(std::vector<Element> &array);
void quickSort(std::vector<Element> &array, int start, int end);
void mergeSort(std::vector<Element> &array, int start, int end);
void heapSort(std::vector<Element> &array, int size);
void selectionSort(std::vector<Element> &array);

// utils
void randomize(std::vector<Element> &array);

// globals
int maxBars = 1000;
int auxMaxBars = maxBars;
int delay_ms = 0;
bool isSorting = false;

int main() {
  auto window = sf::RenderWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
                                 "Sorting Algorithms");
  window.setFramerateLimit(144);

  ImGui::SFML::Init(window);
  sf::Clock clock;
  // std::srand(time(NULL));

  std::vector<Element> elements;
  elements.resize(maxBars);
  randomize(elements);

  const char *algorithmsList[] = {"bubbleSort", "quickSort", "mergeSort",
                                  "heapSort", "selectionSort"};
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
      randomize(elements);
    }

    if (ImGui::SliderInt("NumBars", &auxMaxBars, 10, 1000)) {
      if (isSorting)
        auxMaxBars = maxBars;
      else {
        maxBars = auxMaxBars;
        elements.resize(maxBars);
        randomize(elements);
      }
    }

    if (ImGui::Button("Sort")) {
      if (!isSorting) {
        isSorting = true;
        sortingTime = 0;
        if (currentItem == algorithmsList[0])
          sortingThread = std::thread(bubbleSort, std::ref(elements));
        else if (currentItem == algorithmsList[1])
          sortingThread = std::thread(quickSort, std::ref(elements), 0,
                                      elements.size() - 1);
        else if (currentItem == algorithmsList[2])
          sortingThread = std::thread(mergeSort, std::ref(elements), 0,
                                      elements.size() - 1);
        else if (currentItem == algorithmsList[3])
          sortingThread =
              std::thread(heapSort, std::ref(elements), elements.size());
        else if (currentItem == algorithmsList[4])
          sortingThread = std::thread(selectionSort, std::ref(elements));

        sortingThread.detach();
      }
    }

    ImGui::Text("Time: %.5f s", sortingTime);

    ImGui::End();

    window.clear();

    for (int i = 0; i < elements.size(); i++) {
      elements[i].draw(window, i, window.getSize().x / elements.size());
    }

    ImGui::SFML::Render(window);

    window.display();
  }
  ImGui::SFML::Shutdown();
}

void randomize(std::vector<Element> &array) {
  for (Element &e : array) {
    e.value = float(rand()) / RAND_MAX;
    e.color = {255, 255, 255};
  }
}

void swap(int &a, int &b) {
  int temp = a;
  a = b;
  b = temp;
}

void bubbleSort(std::vector<Element> &array) {
  for (int i = array.size() - 1; i > 0; i--) {
    for (int j = 0; j < i; j++) {
      if (array[j] > array[j + 1]) {
        array[j].swap(array[j + 1]);

        array[j].color = {255, 0, 0};
        if (delay_ms > 0)
          usleep(1000 * delay_ms);
        array[j].color = {255, 255, 255};
      }
    }
  }
  isSorting = false;
}

// count how many numbers are smaller than the pivot
// put those numbers at the beginning of the array
// put the pivot after those numbers
// return pivot final position
int partition(std::vector<Element> &array, int start, int end) {
  Element pivot = array[end]; // select last element as pivot
  int count = 0;              // start less count at 0

  pivot.color = {255, 0, 0};

  for (int i = start; i < end; i++) {
    array[i].color = {0, 255, 0};

    if (delay_ms > 0)
      usleep(1000 * delay_ms / 2);

    if (array[i] <= pivot) {
      array[i].color = {255, 255, 255};
      array[i].swap(array[start + count]);
      count++;
    }

    if (delay_ms > 0)
      usleep(1000 * delay_ms / 2);
  }

  array[end].color = {255, 255, 255};
  array[start + count].color = {255, 255, 255};

  array[end].swap(array[start + count]); // put pivot into position

  return start + count;
}

// optimized partition, find middle point comparing both ends
// return pivot final position
int partition_opt(std::vector<Element> &array, int start, int end) {
  int i = start, j = end - 1;
  Element pivot = array[end];
  array[end].color = {255, 0, 0};

  while (i <= j) {
    array[i].color = {0, 255, 0};
    array[j].color = {0, 255, 0};
    if (delay_ms > 0)
      usleep(1000 * delay_ms);

    if (array[i] >= pivot && array[j] <= pivot) {
      array[i].swap(array[j]);
      array[i].color = {255, 255, 255};
      array[j].color = {255, 255, 255};
      i++;
      j--;
    }

    if (array[i] < pivot) {
      array[i].color = {255, 255, 255};
      i++;
    }

    if (array[j] > pivot) {
      array[j].color = {255, 255, 255};
      j--;
    }
  }

  array[end].color = {255, 255, 255};

  array[end].swap(array[i]);

  return i;
}

void quickSort(std::vector<Element> &array, int start, int end) {
  if (start >= end)
    return;

  int pivotIndex = partition_opt(array, start, end);

  quickSort(array, start, pivotIndex - 1);
  quickSort(array, pivotIndex + 1, end);

  if (start == 0 && end >= array.size() - 1)
    isSorting = false;
}

void merge(std::vector<Element> &array, int start, int end, int middle) {
  int i = start;      // left subarray index
  int j = middle + 1; // rigth subarray index

  std::vector<Element> cpy;
  cpy.resize(end - start + 1);
  int index = 0; // auxiliary index

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
    array[i].color = {255, 0, 0};

  for (int i = start; i < end + 1; i++) {
    if (delay_ms > 0)
      usleep(1000 * delay_ms);
    array[i] = cpy[i - start];
  }

  for (int i = start; i < end + 1; i++)
    array[i].color = {255, 255, 255};
}

void mergeSort(std::vector<Element> &array, int start, int end) {
  if (start >= end)
    return;

  // split array
  int middle = (start + end) / 2;

  mergeSort(array, start, middle);
  mergeSort(array, middle + 1, end);

  // merge
  merge(array, start, end, middle);

  if (start == 0 && end >= array.size() - 1)
    isSorting = false;
}

int getLeftChild(int parent) { return 2 * parent + 1; };

int getRightChild(int parent) { return 2 * parent + 2; };

int getParent(int child) { return (child - 1) / 2; };

void siftDown(std::vector<Element> &array, int root, int size) {
  // while children exists
  while (getLeftChild(root) < size) {
    int childL = getLeftChild(root);
    int childR = getRightChild(root);
    int greaterChild = childL;

    if (childR < size && array[childL] < array[childR]) {
      greaterChild = childR;
    }

    if (array[greaterChild] > array[root]) {

      array[greaterChild].swap(array[root]);

      // fix possibly broken heap from new child value
      siftDown(array, greaterChild, size);

    } else {
      // root is greater than children (assuming children are valid heaps)
      return;
    }
  }
};

// create heap from array
void heapify(std::vector<Element> &array, int size) {
  // build heap from the last leaf to the root
  int root = getParent(size - 1) + 1;

  while (root > 0) {
    root--;
    // check if root complies with property (root > its 2 children)

    array[root].color = {0, 255, 0};
    if (delay_ms > 0)
      usleep(1000 * delay_ms / 2);
    array[root].color = {255, 255, 255};

    siftDown(array, root, size);
  }
};

void heapSort(std::vector<Element> &array, int size) {
  // create heap data structure from array
  heapify(array, size);

  int end = size;
  while (end > 1) {
    end--;                     // reduce the heap size
    array[0].swap(array[end]); // move greater value to end of the array
    siftDown(array, 0, end);   // fix heap

    array[end].color = {255, 0, 0};
    if (delay_ms > 0)
      usleep(1000 * delay_ms / 2);
    array[end].color = {255, 255, 255};
  }

  isSorting = false;
}

void selectionSort(std::vector<Element> &array) {
  for (int i = 0; i < MAX_BARS; i++) {
    for (int j = i + 1; j < MAX_BARS; j++) {
      if (array[j] < array[i]) {
        array[j].color = {255, 0, 0};
        array[i].color = {255, 0, 0};
        if (delay_ms > 0)
          usleep(1000 * delay_ms / 2);

        array[i].swap(array[j]);

        if (delay_ms > 0)
          usleep(1000 * delay_ms / 2);
        array[j].color = {255, 255, 255};
        array[i].color = {255, 255, 255};
      }
    }
  }

  isSorting = false;
}
