#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "vector"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  bool SnakeCollide(Snake snake1, Snake snake2);
  int GetScore() const;
  int GetSnakeSize() const;
  int GetBadSnakeSize() const;
  

 private:
  Snake snake,bad_snake;
  SDL_Point food;
  SDL_Point poison[3];

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  int lives{5}; 
  bool collide{false};
  void PlaceFood();
  void PlacePoison();
  void Update();
};

#endif