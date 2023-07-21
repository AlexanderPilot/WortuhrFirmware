#include "Snake.h"

LED_Ausgabe *_snakeLedausgabe;

Snake::Snake()
{
    previousMillis = 0;
    interval = 250;
    snakeLength = 1;
    snakeColor = {255, 255, 255};
    fruitColor = {0, 0, 0};
    count10ms = 0;
    paused = false;
};

void Snake::setup()
{
    /*snakeX[0] = NUM_COLUMN / 2;
    snakeY[0] = NUM_ROW / 2;

    makeFruit();
    drawFruit();
    FillSnakeWithColour();*/
}

boolean Snake::loop()
{
/*
    unsigned long currentMillis = millis();

    if (paused)
    {
        // Black bar behind text
        for (int i = 48; i < 191; i++)
        {
            leds(i) = CRGB::Black;
        }
        if (ScrollingMsg.UpdateText() == -1)
        {
        ScrollingMsg.SetText((unsigned char *)txtSnake, sizeof(txtSnake) - 1);
        }
        if (SerialBT.available())
        {
            reset();
        }
    }
    else
    {
        // FastLED.clear();*/
    _snakeLedausgabe->clearLEDs();
    drawFruit();
    FillSnakeWithColour();
    /*        if (currentMillis - previousMillis >= interval)
            {
                previousMillis = currentMillis;
                nextStep();
            }

            if (SerialBT.available())
            {
                char keyPress = (char)SerialBT.read();
                switch (keyPress)
                {
                case 'w':
                    currentInput = UP;
                    break;
                case 'a':
                    currentInput = LEFT;
                    break;
                case 's':
                    currentInput = DOWN;
                    break;
                case 'd':
                    currentInput = RIGHT;
                    break;
                case 'm':
                    currentApp = -1;
                    return false;
                }
            }
        }

        FastLED.show();
        delay(10);
    */
    return true;
}

void Snake::makeFruit()
{
    uint8_t x;
    uint8_t y;
    
    x = random(0, NUM_COLUMN);
    y = random(0, NUM_ROW);
    
    while (isPartOfSnake(x, y))
    {
        x = random(0, NUM_COLUMN);
        y = random(0, NUM_ROW);
    }
    
    fruitX = x;
    fruitY = y;

    fruitColor.red = random(20, 255);   // TODO: prüfen ob min wert von 20 auf Matrix sichbar ist
    fruitColor.green = random(20, 255); // TODO: prüfen ob min wert von 20 auf Matrix sichbar ist
    fruitColor.blue = random(20, 255);  // TODO: prüfen ob min wert von 20 auf Matrix sichbar ist
}

void Snake::drawFruit()
{
    _snakeLedausgabe->setPixelToMatrix(fruitX, fruitY, fruitColor);
}

void Snake::nextStep()
{ /*
     for (int i = snakeLength - 1; i > 0; i--)
     {
         snakeX[i] = snakeX[i - 1];
         snakeY[i] = snakeY[i - 1];
     }
     switch (currentInput)
     {
     case SIGN_GAME_UP:
         snakeY[0] = snakeY[0] + 1;
         break;
     case SIGN_GAME_RIGHT:
         snakeX[0] = snakeX[0] + 1;
         break;
     case SIGN_GAME_DOWN:
         snakeY[0] = snakeY[0] - 1;
         break;
     case SIGN_GAME_LEFT:
         snakeX[0] = snakeX[0] - 1;
         break;
     }
*/
     // Check if head has hit fruit
     if ((snakeX[0] == fruitX) && (snakeY[0] == fruitY))
     {
         snakeLength++;

         if (snakeLength < MAX_SNAKE_LENGTH)
         {
             makeFruit();
         }
     }

     // Check if head has hit body or left play area
     if (isPartOfSnakeBody(snakeX[0], snakeY[0]) || !inPlayField(snakeX[0], snakeY[0]))
     {
         //currentInput = NONE;
         //TODO: Bildschirm kurz rot aufleuchten lassen, ggfs mehrfach blinken
         _snakeLedausgabe->clearLEDs();
         paused = true;
         //TODO: spielstand anzeigen
         //sprintf((char *)txtSnake, "   SCORE %u   ", snakeLength - 1);
         // ScrollingMsg.SetText(txtSnake, strlen((const char *)txtSnake));
     }
}

void Snake::reset()
{
    snakeLength = 1;
    // currentInput = SIGN_GAME_LEFT;
    snakeX[0] = NUM_COLUMN / 2;
    snakeY[0] = NUM_ROW / 2;
    for (int i = 1; i < MAX_SNAKE_LENGTH; i++)
    {
        snakeX[i] = snakeY[i] = -1;
    }
    paused = false;
}

boolean Snake::inPlayField(int x, int y)
{
    return (x >= 0) && (x < NUM_COLUMN) && (y >= 0) && (y < NUM_ROW);
    //TODO: prüfen ob nachfolgende Zeile auch funktioniert
    //return (0 <= x < NUM_COLUMN) && (0 <= y < NUM_ROW);
}

boolean Snake::isPartOfSnake(int x, int y)
{
    for (int i = 0; i < snakeLength - 1; i++)
    {
        if ((x == snakeX[i]) && (y == snakeY[i]))
        {
            return true;
        }
    }
    return false;
}

boolean Snake::isPartOfSnakeBody(int x, int y)
{
    for (int i = 1; i < snakeLength - 1; i++)
    {
        if ((x == snakeX[i]) && (y == snakeY[i]))
        {
            return true;
        }
    }
    return false;
}

void Snake::FillSnakeWithColour()
{
    uint8_t i = 0;
    for (i = 0; i < snakeLength; i++)
    {
        _snakeLedausgabe->setPixelToMatrix(snakeX[i], snakeY[i], snakeColor);
    }
}
