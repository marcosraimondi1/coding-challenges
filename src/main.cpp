#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <thread>
#include <unistd.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define MAX_BARS 1000
#define SWAP_SLEEP_MS 0

#define BAR_WIDTH WINDOW_WIDTH / MAX_BARS
#define BAR_STEP WINDOW_HEIGHT / MAX_BARS

void bubbleSort(int array[MAX_BARS]);
void quickSort(int array[MAX_BARS], int start, int end);
int colors[MAX_BARS] = {};

int main() {
  auto window = sf::RenderWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
                                 "Sorting Algorithms");
  window.setFramerateLimit(144);

  int array[MAX_BARS] = {};
  for (int &e : array) {
    e = rand() % MAX_BARS;
  }

  std::thread sortingThread = std::thread(quickSort, array, 0, MAX_BARS - 1);
  // std::thread sortingThread = std::thread(bubbleSort, array);

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    window.clear();

    int x = -BAR_WIDTH;
    int y = 0;
    for (int i = 0; i < MAX_BARS; i++) {

      sf::RectangleShape rectangle;
      int e = array[i];
      x += BAR_WIDTH;

      rectangle.setSize({BAR_WIDTH, e * BAR_STEP});
      rectangle.setPosition({x + BAR_WIDTH, y});

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

    window.display();
  }
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
        usleep(1000 * SWAP_SLEEP_MS);
        colors[j] = 0;
      }
    }
  }
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
    usleep(1000 * SWAP_SLEEP_MS / 2);
    if (array[i] <= pivot) {
      swap(array[i], array[start + count]);
      count++;
    }
    usleep(1000 * SWAP_SLEEP_MS / 2);
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
    usleep(1000 * SWAP_SLEEP_MS);

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

  usleep(1000 * SWAP_SLEEP_MS);

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
}

void mergeSort();
