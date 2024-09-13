#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

int main()
{
    // file input handlers
    std::ifstream fin("C:\\Users\\vipri\\Downloads\\config.txt");
    std::string rowHeader;

    // window properties
    float wWidth = 0;
    float wHeight = 0;
    
    // shape container
    std::vector<std::shared_ptr<sf::Shape>> shapes;

    // keep track of shape speeds and names
    std::vector<float> xSpeeds;
    std::vector<float> ySpeeds;
    std::vector<sf::Text> texts;

    // font properties
    sf::Font font;
    std::string fontFilename;
    int fontSize = 0;

    // font colors
    int fontR, fontG, fontB;

    // shape properties
    std::string shapeName;
    float xPos;
    float yPos;
    float xSpeed;
    float ySpeed;
    int r;
    int g;
    int b;
    float circleRadius;
    float rectangleWidth;
    float rectangleHeight;

    // read through each file token
    while (fin >> rowHeader)
    {
        if (rowHeader == "Window")
        {
            fin >> wWidth >> wHeight;
        }
        else if (rowHeader == "Font")
        {
            fin >> fontFilename >> fontSize >> fontR >> fontG >> fontB;

            // test if font could be loaded
            if (!font.loadFromFile(fontFilename))
            {
                std::cerr << "Could not load font.\n";
                exit(-1);
            }
        }
        else if (rowHeader == "Circle")
        {
            fin >> shapeName >> xPos >> yPos >> xSpeed >> ySpeed >> r >> g >> b >> circleRadius;
            std::shared_ptr<sf::Shape> circle = std::make_shared<sf::CircleShape>(circleRadius);
            circle->setPosition(sf::Vector2f(xPos, yPos));
            circle->setFillColor(sf::Color(r, g, b));
            xSpeeds.push_back(xSpeed);
            ySpeeds.push_back(ySpeed);
            shapes.push_back(circle);
            texts.push_back(sf::Text(shapeName, font, fontSize));
        }
        else if (rowHeader == "Rectangle")
        {
            fin >> shapeName >> xPos >> yPos >> xSpeed >> ySpeed >> r >> g >> b >> rectangleWidth >> rectangleHeight;
            std::shared_ptr<sf::Shape> rectangle = std::make_shared<sf::RectangleShape>(sf::Vector2f(rectangleWidth, rectangleHeight));
            rectangle->setPosition(sf::Vector2f(xPos, yPos));
            rectangle->setFillColor(sf::Color(r, g, b));
            xSpeeds.push_back(xSpeed);
            ySpeeds.push_back(ySpeed);
            shapes.push_back(rectangle);
            texts.push_back(sf::Text(shapeName, font, fontSize));
        }
    }

    // initialize window
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "BIG BIG CHUNGUS BIG CHUNGUS BIG CHUNGUS");

    // main game/window loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // stops the program if window is closed
            if (event.type == sf::Event::Closed)
            {
                window.close();
                exit(0);
            }       
        }

        // clear previous frame
        window.clear();

        int i = 0;

        // update and draw each shape
        for (auto& shape : shapes)
        {
            if (shape->getGlobalBounds().left < 0 || shape->getGlobalBounds().left + shape->getGlobalBounds().width > wWidth)
            {
                xSpeeds.at(i) *= -1;
            }
            else if (shape->getGlobalBounds().top < 0 || shape->getGlobalBounds().top + shape->getGlobalBounds().height > wHeight)
            {
                ySpeeds.at(i) *= -1;
            }

            shape->setPosition(shape->getPosition().x + xSpeeds.at(i), shape->getPosition().y + ySpeeds.at(i));
            i++;
           
            window.draw(*shape);
        }

        i = 0;

        // update and draw each text
        for (auto& text : texts)
        {
            text.setPosition(shapes.at(i)->getGlobalBounds().left + (shapes.at(i)->getGlobalBounds().width / 2) - (text.getGlobalBounds().width / 2), shapes.at(i)->getGlobalBounds().top + (shapes.at(i)->getGlobalBounds().height / 2) - text.getGlobalBounds().height);
            i++;

            window.draw(text);
        }

        // display window components
        window.display();
    }

    return 0;
}