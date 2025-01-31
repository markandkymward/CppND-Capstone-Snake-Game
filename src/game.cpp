#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height), bad_snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
  PlacePoison();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  bool old_collision = false;
  bool new_collision = false;


  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    controller.RandomInput(running, bad_snake);
    old_collision = collide;
    Update();
    new_collision = collide;
    if (new_collision != old_collision){
      lives --;
    } else{
      old_collision = new_collision;
    }
    renderer.Render(snake, food, bad_snake, poison);
    if (lives == 0){
      snake.alive = false;
      break;
    }
    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, lives);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::PlacePoison() {
  int x, y;
  for (int i = 0; i < 6; i++){
    //while (true) {
      x = random_w(engine);
      y = random_h(engine);
      // Check that the location is not occupied by a snake item before placing
      // food.
      if (!snake.SnakeCell(x, y)) {
        poison[i].x = x;
        poison[i].y = y;
      }
    //}
  }
  return;
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();
  bad_snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);
  score = snake.size;
  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    bad_snake.GrowBody();
    snake.speed += 0.01;
  }
  for (int i = 0; i < 3; i++){
    if (poison[i].x == new_x && poison[i].y == new_y) {
      snake.ShrinkBody();
      PlacePoison();
    }
  }
  collide = SnakeCollide(snake,bad_snake);
}
bool Game::SnakeCollide(Snake snake1, Snake snake2){
  bool collide {false};
  for (auto &item1 : snake1.body){
    for (auto &item2 : snake2.body){
      if (item1.x == item2.x && item1.y == item2.y){
        collide = true;
        break;
      }
    }
  }
  return collide; 
}
int Game::GetScore() const { return score; }
int Game::GetSnakeSize() const { return snake.size; }
int Game::GetBadSnakeSize() const { return bad_snake.size; }