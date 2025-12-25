#include<SFML/Graphics/Rect.hpp>
#include<SFML/Graphics/Sprite.hpp>
#include<SFML/System/Vector2.hpp>
#include<iostream>
#include<SFML/Graphics.hpp>
#include<SFML/Window/Keyboard.hpp>

class Bar {
private:
    float barSpeed = 500.f;

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
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            move.y += 1.f;
        }

        move *= barSpeed * dt;

        sf::Vector2f pos = spriteBar.getPosition();
        a = pos.y;
        b = pos.y + 64.f;
        // std::cout << "bar's Y pos: " << b << "\n";

        sf::FloatRect bounds = spriteBar.getGlobalBounds();
        if (bounds.position.y + move.y >= 4.f && bounds.position.y + move.y <= 600.f - 68.f) {
            spriteBar.move(move);
        }
    }
};

class AIBar {
public:
    sf::Texture textureAIBar;
    sf::Sprite spriteAIBar;
    float AIBarSpeed = 500.f;
    float a, b;

    AIBar()
    : textureAIBar("bar.png", false, sf::IntRect({0, 0}, {2, 16})), spriteAIBar(textureAIBar) {
        spriteAIBar.setScale({4.f, 4.f});
        spriteAIBar.setPosition({783.f, 265.f});
    }

    void draw(sf::RenderWindow& window) {
        window.draw(spriteAIBar);
    }

    void AIBarStatus(const float& dt) {
        sf::Vector2f pos = spriteAIBar.getPosition();
        a = pos.y;
        b = pos.y + 64.f;
    }
};

class Ball {
public:
    sf::Texture textureBall;
    sf::Sprite spriteBall;
    sf::Vector2f valocity;
    Bar curBar;
    AIBar curAIBar;
    float p, q, r;

    Ball(Bar& bar, AIBar& curAiBar)
        : textureBall("ball.png", false, sf::IntRect({0, 0}, {4, 4})), spriteBall(textureBall), curBar(bar), curAIBar(curAiBar), valocity(400.f, 200.f) {
        spriteBall.setScale({2.f, 2.f});
        spriteBall.setPosition({400.f, 300.f});
    }

    void draw(sf::RenderWindow& window) {
        window.draw(spriteBall);
        curBar.draw(window);
        curAIBar.draw(window);
    }

    void collisionStatus(const float& dt) {
        curBar.barStatus(dt);
        curAIBar.AIBarStatus(dt);

        sf::FloatRect bounds = spriteBall.getGlobalBounds();
        sf::Vector2f pos = spriteBall.getPosition();
        p = pos.y;
        q = pos.y + 8.f;
        r = pos.x + 8.f;
        // std::cout << p << "\n";

        if (spriteBall.getPosition().x <= 0.f || spriteBall.getPosition().x >= 792.f) {
            valocity.x = -valocity.x;
        }
        if (spriteBall.getPosition().y <= 0.f || spriteBall.getPosition().y >= 592.f) {
            valocity.y = -valocity.y;
        }
        if (bounds.findIntersection(curBar.spriteBar.getGlobalBounds())) {
            if ((p >= curBar.a && p <= curBar.b) || (q >= curBar.a && q <= curBar.b)) {
                valocity.x = -valocity.x;
            }
        }

        if (bounds.findIntersection(curAIBar.spriteAIBar.getGlobalBounds())) {
            if ((p >= curAIBar.a && p <= curAIBar.b) || (q >= curAIBar.a && q <= curAIBar.b)) {
                valocity.x = -valocity.x;
            }
        }

        spriteBall.move(valocity * dt);
    }

    void AIBarStatus(const float& dt) {
        sf::Vector2f move(0, 0);

        sf::Vector2f aiBarPos = curAIBar.spriteAIBar.getPosition();

        if (r > 600.f) {
            if (p > aiBarPos.y + 32.f) {
                move.y += 1.f;
                move *= curAIBar.AIBarSpeed * dt;
                curAIBar.spriteAIBar.move(move);
            }

            if (p < aiBarPos.y + 32.f) {
                move.y -= 1.f;
                move *= curAIBar.AIBarSpeed * dt;
                curAIBar.spriteAIBar.move(move);
            }
        } else {
            if (aiBarPos.y + 32.f < 350.f) {
                move.y += 1.f;
                move *= 150.f * dt;
                curAIBar.spriteAIBar.move(move);
            }
            if (aiBarPos.y + 32.f > 350.f) {
                move.y -= 1.f;
                move *= 100.f * dt;
                curAIBar.spriteAIBar.move(move);
            }
        }
    }
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
    AIBar aiBar;
    Ball ball(bar, aiBar);

    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        float dt = clock.restart().asSeconds();
        ball.collisionStatus(dt);
        ball.AIBarStatus(dt);

        window.clear(sf::Color::Black);
        ball.draw(window);
        window.display();
    }

    return 0;
}

