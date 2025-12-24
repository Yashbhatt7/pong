#include<SFML/Graphics/Rect.hpp>
#include<SFML/Graphics/Sprite.hpp>
#include<iostream>
#include<SFML/Graphics.hpp>
#include<SFML/Window/Keyboard.hpp>

class Bar {
private:
    float barSpeed = 500;
    bool dir;

public:
    sf::Texture textureBar;
    sf::Sprite spriteBar;
    float a, b;

    Bar()
        : textureBar("bar.png", false, sf::IntRect({0, 0}, {2, 16})), spriteBar(textureBar) {
        spriteBar.setScale({4.f, 4.f});
        spriteBar.setPosition({10.f, 250.f});
    }

    void draw(sf::RenderWindow& window) {
        window.draw(spriteBar);
    }

    void barStatus(const float& dt) {
        sf::Vector2f move(0, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            move.y -= 1.f;
            dir = false;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            move.y += 1.f;
            dir = true;
        }

        move *= barSpeed * dt;

        sf::Vector2f pos = spriteBar.getPosition();
        a = pos.y;
        b = pos.y + 64;
        // std::cout << "bar's Y pos: " << b << "\n";

        sf::FloatRect bounds = spriteBar.getGlobalBounds();
        if (bounds.position.y + move.y >= 4.f && bounds.position.y + move.y <= 600 - 68) {
            spriteBar.move(move);
        }
    }
};

class Ball {
private:
    sf::Texture textureBall;
    sf::Sprite spriteBall;
    sf::Vector2f valocity;
    Bar curBar;

public:
    float p, q;

    Ball(Bar& bar)
        : textureBall("ball.png", false, sf::IntRect({0, 0}, {4, 4})), spriteBall(textureBall), curBar(bar), valocity(400.f, 200.f) {
        spriteBall.setScale({2.f, 2.f});
        spriteBall.setPosition({400.f, 300.f});
    }

    void draw(sf::RenderWindow& window) {
        window.draw(spriteBall);
        curBar.draw(window);
    }

    void collideWallStatus(const float& dt) {
        curBar.barStatus(dt);

        sf::FloatRect bounds = spriteBall.getGlobalBounds();

        if (spriteBall.getPosition().x <= 0.f || spriteBall.getPosition().x >= 792) {
            valocity.x = -valocity.x;
        }
        if (spriteBall.getPosition().y <= 0.f || spriteBall.getPosition().y >= 592) {
            valocity.y = -valocity.y;
        }
        if (bounds.findIntersection(curBar.spriteBar.getGlobalBounds())) {
            valocity.x = -valocity.x;
        }

        spriteBall.move(valocity * dt);
    }
};

class AIBar {
public:
    float AIBarSpeed = 500;
};

int main() {
    float barSpeed = 500;
    float ballSpeed = 300;

    sf::RenderWindow window(sf::VideoMode({800, 600}), "My Window");
    sf::RectangleShape rectangle1({10.f, 10.f});
    rectangle1.setPosition({18.f, 314.f});

    sf::RectangleShape rectangle2({10.f, 10.f});
    rectangle2.setPosition({18.f, 250.f});

    Bar bar;
    Ball ball(bar);

    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        float dt = clock.restart().asSeconds();
        ball.collideWallStatus(dt);

        window.clear(sf::Color::Black);
        ball.draw(window);
        // window.draw(rectangle1);
        // window.draw(rectangle2);
        window.display();
    }

    return 0;
}

