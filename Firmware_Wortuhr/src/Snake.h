#pragma once

#include "Arduino.h"
#include "Configurations.h"

class Snake
{
  unsigned long previousMillis;
  uint8_t interval;
  uint8_t startIndex;
  pixel_t snakeColor;
  pixel_t fruitColor;
  uint32_t count10ms;
  uint8_t snakeLength;
  uint8_t snakeX[MAX_SNAKE_LENGTH];
  uint8_t snakeY[MAX_SNAKE_LENGTH];
  uint8_t fruitX;
  uint8_t fruitY;
  boolean paused;

public:
  Snake();
  void setup();
  boolean loop();

private:
  void makeFruit();
  void drawFruit();
  void nextStep();
  void reset();
  boolean inPlayField(int x, int y);
  boolean isPartOfSnake(int x, int y);
  boolean isPartOfSnakeBody(int x, int y);
  void FillSnakeWithColour();
};