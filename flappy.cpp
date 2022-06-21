#include "files.h"
#include "iostream"
#include <SFML/Graphics.hpp>


sf::Texture ground;
sf::Texture upPipe;
sf::Image pipeImage2;
sf::Texture nizPipe;


sf::Clock pipeGenerating;
sf::Font font;

/**
 * Class BirdFlappy - отвечает за саму птицу.
 * currentFrame - частота процессора процессора.
 *
 */
float Y;
class BirdFlappy {
private:
    float Y;
    float vel;
    float currentFrame{};
    std::vector<sf::Texture > frames;

public:
    /**
     * Анимация птицы.
     * Начальное положение птицы.
     */

    BirdFlappy() {
        auto i = 0;
        for (const auto &path: {
                     "fordraw/bird/1.png",
                     "fordraw/bird/2.png"
             }){
            frames.emplace_back();
            frames[i].loadFromFile(path);
        }
        Y = 500;
        vel = 0;

    }


    /**
     * Расстояние, от трубы при столкновении с птицей.
     * @return - прошла ли птица трубу
     */

    sf::FloatRect getRect() {
        sf::Texture &texture = frames[(int) currentFrame];
        auto size = texture.getSize();
        return {
                50, Y, (float) size.x, (float) size.y};
    }
    /**
     * Гравитация, с которой птица летит.
     */
    void flap() {
        if (not gamerun or gameOvered) return;
        vel = -255;
    }
    /**
     * отрисовка наклона птицы.(наклона квадрата птицы)
     */
    void draw(sf::RenderWindow &window) {
        sf::Texture &texture = frames[(int) currentFrame];
        sf::Sprite birdSprite(texture);
        birdSprite.setRotation(10 * (vel / 300));
        birdSprite.setPosition(70, Y);

        window.draw(birdSprite);

    }
    /**
     *Отрисовка частоты картинки.
     * отрисовка обновления частоты процессора
     *
     */

    void update() {
        currentFrame += proc * 10;
        if (currentFrame >= frames.size()) {
            currentFrame -= frames.size();
        }
        sf::Texture &texture = frames[(int) currentFrame];

        if (gamerun) {
            vel += proc * 900;
            Y += vel * proc;

            if (Y < 0 or Y + texture.getSize().y > backgroundTexture.getSize().y) {
                gameOvered = true;
            }

            if (Y < 0 or Y + texture.getSize().y > backgroundTexture.getSize().y) {
                Y = (float) backgroundTexture.getSize().y + texture.getSize().y;
                vel = 0;
            }
        }
    }
};
auto bird = BirdFlappy();


/**
 *
 * @param s принимает прошлый счет.
 * @return  возвращает обновленный счет.
 */
int Upscore(int s) {
    s++;
    return s;
}



class Pipe {
private:
    float x;
    float y;
    bool scored;

public:
    Pipe(sf::RenderWindow &window) {
        x = (float) (window.getSize().x + upPipe.getSize().x);
        y = 100.0f + (float) (rand() % 5 - 2) * 50;
        scored = false;
    }
    sf::FloatRect getUpperRect() const {
        auto size = upPipe.getSize();
        return {
                x, y + 340,
                (float) size.x, (float) size.y};
    }

    sf::FloatRect getLowerRect() const {
        auto size = upPipe.getSize();
        return {
                x, y - 340,
                (float) size.x, (float) size.y};
    }

    /**
 * Положение позиции труб:
 * sf::Sprite upperSprite(*upPipe);
upperSprite.setPosition(x, y+340);
sf::Sprite lowerSprite(*nizPipe);
lowerSprite.setPosition(x, y-340);


Отрисовка позиции труб:
window->draw(upperSprite);
window->draw(lowerSprite);
 */
    void draw(sf::RenderWindow &window)
            const {
        sf::Sprite upperSprite(upPipe);
        upperSprite.setPosition(x, y + 300);
        sf::Sprite lowerSprite(nizPipe);
        lowerSprite.setPosition(x, y - 300);

        window.draw(upperSprite);
        window.draw(lowerSprite);
    }
    /**
         * if (birdRect.intersects(getUpperRect()) or birdRect.intersects(getLowerRect())) - если птица упала - игра завершается.
         * if (x + upPipe->getSize().x < birdRect.left and not scored){
           scored = true;
           score++;  -- обновление счета.
         */
    void update() {
        if (not gamerun or gameOvered) return;
        x -= 100 * proc;
        auto birdRect = bird.getRect();
        if (birdRect.intersects(getUpperRect()) or birdRect.intersects(getLowerRect())) {
            gameOvered = true;
        }
        if (x + upPipe.getSize().x < birdRect.left and not scored) {
            scored = true;
            score = Upscore(score);
        }
    }
};

std::vector<Pipe> pipes;
sf::Clock pipeGeneratingClock;


/**
 * Функция настройки параметров.
 *
 * Загрузка всех необходимых параметров:
 * font =  sf::Font(); - шрифт
 * backgroundTexture =  sf::Texture(); - загрузка заднего фона.
 * ground->loadFromFile("textures/ground.png"); - загрузка нижней части фона.
 * pipeImage.loadFromFile("textures/pipe.png"); - загрузка трубы.
 */

void setup(sf::RenderWindow &window) {
    srand((unsigned int) time(nullptr));
    pipeGeneratingClock = sf::Clock();


    font = sf::Font();
    font.loadFromFile("fonts/font.ttf");




    backgroundTexture = sf::Texture();
    backgroundTexture.loadFromFile("fordraw/back.png");


    ground.loadFromFile("fordraw/ground.png");

    sf::Image pipeImage;
    pipeImage.loadFromFile("fordraw/pipe.png");
    pipeImage2.loadFromFile("fordraw/pipe.png");
    upPipe.loadFromImage(pipeImage);
    pipeImage2.flipVertically();
    nizPipe.loadFromImage(pipeImage2);

    pipes.emplace_back(window);
}


/**
 * Функция направлена на обработку нажатий пользователем на интерфейс
 * MouseButtonPressed -- если пользователь нажал на кнопку мыши :
 * -pipeGeneratingClock->restart();
   -pipes.push_back( Pipe());
   Продолжение генерации труб.

bird->flap(); - функция анимации крыльев птицы.
        * Closed -- При нажатии на кнопку закрытия , окно закрывается.
                * window->close();
*/
void whatEvent(sf::Event &event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (not gamerun) {
            gamerun = true;
            pipeGeneratingClock.restart();
        }
        bird.flap();
    }
}
/**
 * Функция направлена на изменение пространства, где летит птица.
 * pipe->update -- создание новых труб.
 * Так же функция удаляет уже пролетевшие птицей трубы.
 */
void update(sf::RenderWindow &window) {
    bird.update();
    for (auto &pipe: pipes) {
        pipe.update();
    }
    if (gamerun and not gameOvered) {
        if (pipeGeneratingClock.getElapsedTime().asSeconds() > 3.5) {
            pipeGeneratingClock.restart();
            pipes.push_back( Pipe(window));
            if (pipes.size() > 4) {
                pipes.erase(pipes.begin());
            }
        }
    }
}

/**
 * Функция работает при выключении.
 */


/**
 * Функция отрисовки.
 * window->draw(sf::Sprite(*backgroundTexture)); - отрисовка фона.
 *
 * sf::Sprite groundSprite(*ground); - рисовка нижней полоски фона.
 *
 * bird->draw(); - отрисовка птицы.
 *
 * window->draw(scoreText); - отрисовка счета.
 */
void drawing(sf::RenderWindow &window) {
    window.clear();

    window.draw(sf::Sprite(backgroundTexture));

    for (const auto &pipe: pipes) {
        pipe.draw((sf::RenderWindow &) pipe);
    }
    sf::Sprite groundSprite(ground);
    if (not(not gamerun or gameOvered)) {
        setground -= proc * 100;
        if (setground <= -20) {
            setground += 20;
        }
    }
    groundSprite.setPosition(setground, backgroundTexture.getSize().y);
    window.draw(groundSprite);
    sf::RectangleShape lowerRectangle({(float) window.getSize().x,
                                       (float) window.getSize().y - backgroundTexture.getSize().y - ground.getSize().y});
    lowerRectangle.setPosition(0, (float) backgroundTexture.getSize().y + ground.getSize().y);
    lowerRectangle.setFillColor({245, 228, 138});
    window.draw(lowerRectangle);


    bird.draw(window);
    sf::Text scoreText("SCORE:" + std::to_string(score), font);
    scoreText.setPosition(window.getSize().x / 2 - scoreText.getLocalBounds().width / 2, 5);

    window.draw(scoreText);
}

int Play() {
    auto window = sf::RenderWindow(sf::VideoMode(500, 700), "Flappy");
    window.setPosition({640, 120});
    setup(window);


    sf::Clock procClock;

    while (window.isOpen()) {
        sf::Event event{};

        while (window.pollEvent(event)) {
            whatEvent(event);
        }
        proc = procClock.getElapsedTime().asMicroseconds() / 1000.0f / 1000.0f;//
        procClock.restart();

        update(window);
        drawing(window);

        window.display();
    }


    return EXIT_SUCCESS;
}
