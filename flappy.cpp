#include <SFML/Graphics.hpp>
#include "iostream"


bool gamerun,gameOvered;
int score;
sf::RenderWindow *window;
float proc;
float setground;
sf::Clock* aClok;
sf::Texture *backgroundTexture;

class BirdFlappy{
private:
    sf::Texture*texture;
    float Y;
    float vel;
    float currentFrame{};
    sf::Clock* animationClock;
    std::vector<sf::Texture*> frames;
public:
    /**
     * Анимация птицы.
     * Начальное положение птицы.
     */

    BirdFlappy(){
        aClok = new sf::Clock;
        for (const auto& path :{
                     "fordraw/bird/1.png",
                     "fordraw/bird/2.png"
             }){
            auto frame = new sf::Texture();
            frame ->loadFromFile(path);
            frames.push_back(frame);
        }
        texture = frames[0];
        Y = 500;
        vel =0;
    }

    ~BirdFlappy(){
        for (const auto& ptr : frames){
            delete ptr;
        }
        delete aClok;
    }
    /**
     * Расстояние, от трубы при столкновении с птицей.
     * @return - прошла ли птица трубу
     */

    sf::FloatRect getRect(){
        auto size = texture->getSize();
        return {
                50,Y,(float)size.x,(float)size.y
        };
    }
    /**
     * Гравитация, с которой птица летит.
     */
    void flap(){
        if (not gamerun or gameOvered) return;
        vel = -255;
    }
    /**
     * отрисовка наклона птицы.(наклона квадрата птицы)
     */
    void draw(){
        sf::Sprite birdSprite(*texture);
        birdSprite.setRotation(10*(vel / 300));
        birdSprite.setPosition(70,Y);

        window->draw(birdSprite);

    }
    /**
     *Отрисовка частоты картинки.
     *
     */

    void update(){
        currentFrame +=proc * 10;
        if (currentFrame > frames.size()){
            currentFrame -= frames.size();
        }
        texture = frames[(int)currentFrame];

        if (gamerun) {
            vel += proc * 900;
            Y += vel * proc;

            if (Y<0 or Y+texture->getSize().y > backgroundTexture->getSize().y){
                gameOvered=true;
            }

            if (Y < 0 or Y + texture->getSize().y > backgroundTexture->getSize().y) {
                Y= (float)backgroundTexture->getSize().y + texture->getSize().y;
                vel = 0;
            }
        }

    }
};
BirdFlappy *bird;
sf::Texture* upPipe;
sf::Texture* nizPipe;
/**
 *
 * @param s принимает прошлый счет.
 * @return  возвращает обновленный счет.
 */
int Upscore(int s){
    s++;
    return s;
}

class Pipe{
private:
    float x;
    float y;
    bool scored;
public:
    Pipe(){
        x = (float)(window -> getSize().x + upPipe -> getSize().x);
        y = 100.0f +(float)(rand()%5 - 2)*50;
        scored = false;
    }
    sf::FloatRect getUpperRect() const {
        auto size = upPipe->getSize();
        return {
                x, y + 340,
                (float)size.x, (float)size.y
        };
    }

    sf::FloatRect getLowerRect() const {
        auto size = upPipe->getSize();
        return {
                x, y - 340,
                (float)size.x, (float)size.y
        };
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
    void draw()
            const {
        sf::Sprite upperSprite(*upPipe);
        upperSprite.setPosition(x, y+300);
        sf::Sprite lowerSprite(*nizPipe);
        lowerSprite.setPosition(x, y-300);

        window->draw(upperSprite);
        window->draw(lowerSprite);
    }
    /**
         * if (birdRect.intersects(getUpperRect()) or birdRect.intersects(getLowerRect())) - если птица упала - игра завершается.
         * if (x + upPipe->getSize().x < birdRect.left and not scored){
           scored = true;
           score++;  -- обновление счета.
         */
    void update(){
        if (not gamerun or gameOvered) return;
        x -= 100*proc;
        auto birdRect= bird->getRect();
        if (birdRect.intersects(getUpperRect()) or birdRect.intersects(getLowerRect())) {
            gameOvered=true;
        }
        if (x + upPipe->getSize().x < birdRect.left and not scored){
            scored = true;
            score=Upscore(score);
        }


    }

};

std::vector<Pipe*> pipes;
sf::Texture* ground;
sf::Clock* pipeGeneratingClock;
sf::Font* font;

/**
 * Функция настройки параметров.
 *
 * Загрузка всех необходимых параметров:
 * font = new sf::Font(); - шрифт
 * backgroundTexture = new sf::Texture(); - загрузка заднего фона.
 * ground->loadFromFile("textures/ground.png"); - загрузка нижней части фона.
 * pipeImage.loadFromFile("textures/pipe.png"); - загрузка трубы.
 */

void setup(){
    srand((unsigned int) time(nullptr) );
    pipeGeneratingClock = new sf::Clock;

    font = new sf::Font();
    font ->loadFromFile("fonts/font.ttf");

    window = new sf::RenderWindow(sf::VideoMode(500, 700), "Flappy");
    window->setPosition({640,120});

    bird = new BirdFlappy();

    backgroundTexture = new sf::Texture();
    backgroundTexture->loadFromFile("fordraw/back.png");

    ground = new sf::Texture;
    ground->loadFromFile("fordraw/ground.png");

    upPipe = new sf::Texture;
    nizPipe = new sf::Texture;

    sf::Image pipeImage;
    pipeImage.loadFromFile("fordraw/pipe.png");
    upPipe->loadFromImage(pipeImage);
    pipeImage.flipVertically();
    nizPipe->loadFromImage(pipeImage);

    pipes.push_back(new Pipe());
}




/**
 * Функция направлена на обработку нажатий пользователем на интерфейс
 * MouseButtonPressed -- если пользователь нажал на кнопку мыши :
 * -pipeGeneratingClock->restart();
   -pipes.push_back(new Pipe());
   Продолжение генерации труб.

bird->flap(); - функция анимации крыльев птицы.
        * Closed -- При нажатии на кнопку закрытия , окно закрывается.
                * window->close();
*/
void whatEvent(sf::Event& event){
    if (event.type == sf::Event::MouseButtonPressed){
        if (not gamerun){
            gamerun = true;
            pipeGeneratingClock->restart();
        }
        bird->flap();

    }
    if (event.type == sf::Event::Closed){
        window->close();
    }

}
/**
 * Функция направлена на изменение пространства, где летит птица.
 * pipe->update -- создание новых труб.
 * Так же функция удаляет уже пролетевшие птицей трубы.
 */
void update(){
    bird->update();
    for (const auto& pipe : pipes) {
        pipe->update();
    }
    if(gamerun and not gameOvered){
        if (pipeGeneratingClock->getElapsedTime().asSeconds() > 3.5){
            pipeGeneratingClock->restart();
            pipes.push_back(new Pipe());
            if (pipes.size()>4){
                delete pipes[0];
                pipes.erase(pipes.begin());
            }

        }

    }
}

/**
 * Функция работает при выключении.
 */

void destroy(){
    delete window;
    delete bird;
    delete backgroundTexture;
    delete pipeGeneratingClock;
    delete font;
}

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
void drawing(){
    window->clear();

    window->draw(sf::Sprite(*backgroundTexture));

    for (const auto& pipe : pipes) {
        pipe->draw();
    }
    sf::Sprite groundSprite(*ground);
    if (not(not gamerun or gameOvered)) {
        setground -= proc * 100;
        if(setground <= -20){
            setground += 20;

        }

    }
    groundSprite.setPosition(setground,backgroundTexture->getSize().y);
    window->draw(groundSprite);
    sf::RectangleShape lowerRectangle({
            (float) window->getSize().x,
            (float) window->getSize().y - backgroundTexture->getSize().y - ground->getSize().y
    });
    lowerRectangle.setPosition(0, (float)backgroundTexture->getSize().y + ground->getSize().y);
    lowerRectangle.setFillColor({245, 228, 138});
    window->draw(lowerRectangle);


    bird->draw();
    sf::Text scoreText("SCORE:"+ std::to_string(score), *font);
    scoreText.setPosition(window->getSize().x/2-scoreText.getLocalBounds().width/2,5);

    window->draw(scoreText);


}



int Play(){
    setup();

    sf::Clock procClock;

    while (window -> isOpen()){
        sf::Event event{};

        while (window ->pollEvent(event)){
            whatEvent(event);
        }
        proc=procClock.getElapsedTime().asMicroseconds()/1000.0f/1000.0f; //
        procClock.restart();

        update();
        drawing();

        window->display();
    }

    destroy();

    return EXIT_SUCCESS;
}

