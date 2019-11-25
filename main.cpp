#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include<time.h> 

#include "Level.hpp"

int main() {

    sf::Time dt;
    sf::Clock speedClock;
    sf::Clock gameClock;
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    sf::Font font;
    if (!font.loadFromFile("Minecraft.ttf")) {
        std::cout << "Could not load font" << std::endl;
        return 1;
    }
    sf::Text scoreText;

    const float SCREEN_W = desktopMode.width;
    const float SCREEN_H = desktopMode.height;

    bool upPressed = false;
    bool dnPressed = false;
    bool rtPressed = false;
    bool lfPressed = false;

    float screenX = SCREEN_W/2;
    float screenY = SCREEN_H/2;

    const int numberLevels = 10;

    // Level(maxTime, numberGreen, numberRed, greenMaxSpeed, redMaxSpeed)
    Level levels[numberLevels] = {Level(60, 1, 1, 50, 130), Level(60, 2, 2, 60, 150), Level(60, 3, 3, 70, 170), Level(60, 4, 4, 80, 190), Level(60, 4, 4, 100, 210), Level(60, 4, 4, 130, 250), Level(60, 4, 4, 160, 270), Level(60, 4, 4, 190, 290), Level(60, 4, 4, 210, 310), Level(60, 4, 4, 230, 330)};

    srand(time(0));

    int USER_RADIUS = 50;
    int USER_SPEED = 1000; // px/s

    int health = 1000;
    float FPS = (float)(1.0/120);
    sf::RenderWindow window(sf::VideoMode(SCREEN_W, SCREEN_H), "Bumpy Circles", sf::Style::Fullscreen);

    sf::CircleShape circle(USER_RADIUS);
    int curLvl = 0;

    circle.setFillColor(sf::Color(66,135,245));
    circle.setOrigin(50,50);

    circle.setPosition(SCREEN_W/2, SCREEN_H/2);

    while (window.isOpen()) {
      dt += gameClock.restart();
        sf::Event event;

        while (window.pollEvent(event)) {

          switch (event.type) {

            case sf::Event::Closed:
              window.close();
              break;

            case sf::Event::KeyPressed: {
              switch (event.key.code){
                case sf::Keyboard::Slash:
                  window.close();
                  break;

                case sf::Keyboard::W:
                  upPressed = true;
                  break;

                case sf::Keyboard::S:
                  dnPressed = true;
                  break;

                case sf::Keyboard::A:
                  lfPressed = true;
                  break;

                case sf::Keyboard::D:
                  rtPressed = true;
                  break;
                default:
                  break;
              }
              break;
            }
            case sf::Event::KeyReleased: {
              switch (event.key.code){
                case sf::Keyboard::W:
                  upPressed = false;
                  break;

                case sf::Keyboard::S:
                  dnPressed = false;
                  break;

                case sf::Keyboard::A:
                  lfPressed = false;
                  break;

                case sf::Keyboard::D:
                  rtPressed = false;
                  break;
                default:
                  break;
              }
              break;
          }
          default:
            break;
        }

      }

        if (dt.asSeconds() > FPS) {
          if (upPressed){
            if (screenY - (USER_SPEED * dt.asSeconds()) > circle.getRadius()){
              circle.move(0,-USER_SPEED * dt.asSeconds());
              screenY -= USER_SPEED * dt.asSeconds();
            }
          }
          if (dnPressed){
            if ((screenY + circle.getRadius()) + (USER_SPEED * dt.asSeconds()) < SCREEN_H){
              circle.move(0,USER_SPEED * dt.asSeconds());
              screenY += USER_SPEED * dt.asSeconds();
            }
          }
          if (rtPressed){
            if ((screenX + circle.getRadius()) + (USER_SPEED * dt.asSeconds()) < SCREEN_W){
              circle.move(USER_SPEED * dt.asSeconds(),0);
              screenX += USER_SPEED * dt.asSeconds();
            }
          }
          if (lfPressed){
            if (screenX - (USER_SPEED * dt.asSeconds()) > circle.getRadius()){
              circle.move(-USER_SPEED * dt.asSeconds(),0);
              screenX -= USER_SPEED * dt.asSeconds();
            }
          }
            dt -= sf::seconds(FPS);
        }

        window.clear();

        sf::Vector2f circlePos = circle.getPosition();
        health = levels[curLvl].handleCollisions(health);
        levels[curLvl].registerCollisions(circlePos, USER_RADIUS);

        scoreText.setFont(font);
        scoreText.setString(std::to_string(health));
        scoreText.setFillColor(sf::Color::Yellow);
        scoreText.setCharacterSize(80);
        //scoreText.setPosition(25, 25);

        levels[curLvl].update(dt);

        window.draw(circle);
        window.draw(scoreText);

        levels[curLvl].draw(window);
        
        if (levels[curLvl].cleared && curLvl + 1 < numberLevels) {
          curLvl++;
        } 
        if (curLvl + 1 == numberLevels && levels[curLvl].cleared) {
            std::cout << "You Won!. Congrats you aren't useless!" << std::endl;
            exit(1);
        }
        if(health <= 0){
          std::cout << "You Lost. You are a waste of life." << std::endl;
          exit(1);
        }



        window.display();
    }
}
