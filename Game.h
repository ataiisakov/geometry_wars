//
// Created by cpp on 08.04.22.
//

#ifndef GEOMETRY_WARS_GAME_H
#define GEOMETRY_WARS_GAME_H
#include <SFML/Graphics.hpp>
#include <ostream>
#include "EntityManager.h"
#include "Entity.h"
#include "Common.h"

//width,height,frame limit,full screen mode
struct WindowConfig{ int W,H,FL,FS;

    friend std::ostream &operator<<(std::ostream &os, const WindowConfig &config);
};
//font, size, RGB
struct FontConfig{
    std::string F;
    int S,R,G,B;

    friend std::ostream &operator<<(std::ostream &os, const FontConfig &config);
};
//radius,collision radius,speed,fill color rgb,outline color rgb,outline thickness,shape vertices
struct PlayerConfig{
    int SR,CR;
    float S;
    int FR,FG,FB;
    int OR,OG,OB;
    int OT,V;

    friend std::ostream &operator<<(std::ostream &os, const PlayerConfig &config);
};
//shape rad.,col. rad.,min max speed,outline rgb,outline thickness,min max vertices,small lifespan,spawn interval
struct EnemyConfig{
    int SR,CR;
    float SMIN,SMAX;
    int OR,OG,OB;
    int OT,VMIN,VMAX,L,SI;

    friend std::ostream &operator<<(std::ostream &os, const EnemyConfig &config);
};
//rad,col. rad,speed,fill color rgb,outline color rgb,outline thickness,shaper ver.,lifespan
struct BulletConfig{
    int SR,CR;
    float S;
    int FR,FG,FB;
    int OR,OG,OB;
    int OT,V,L;

    friend std::ostream &operator<<(std::ostream &os, const BulletConfig &config);
};
class Game {
    sf::RenderWindow mWindow;
    EntityManager    mEntities;
    ptr<Entity>      mPlayer;
    bool             mPaused{false};
    bool             mRunning{true};
    WindowConfig     mWindowConfig{};
    FontConfig       mFontConfig;
    EnemyConfig      mEnemyConfig{};
    PlayerConfig     mPlayerConfig{};
    BulletConfig     mBulletConfig{};
    int              mCurrentFrame{0};
    int              mLastEnemySpawnTime{0};
    int              score{0};
    int              mSpecialWeaponCoolDownSec = 3;
    int              mIsSpecialWeaponAvailable = true;
    int              mLastSpecialWeapon= 0;


    std::ostringstream mScoreSS;
    std::ostringstream mSpecialWeaponSS;
    sf::Text         mScoreText;
    sf::Text         mSpecialWeaponText;
    sf::Font         mFont;

    std::string      mGameName{"Geometry Wars"};

    void init(const std::string& config);

    void readConfigFile(const std::string& config);
    void setScoreText();
    void setSpecialWeaponText();
    void resetScore();
    void initText();

    void setPaused();
    void sMovement();
    void sUserInput();
    void sRender();
    void sEnemySpawner();
    void sCollision();
    void sLifespan();

    bool inBounds(ptr<Entity> entity);
    bool isCollided(ptr<Entity> e1,ptr<Entity> e2);

    void spawnPlayer();
    void spawnEnemy();
    void spawnSmallEnemies(ptr<Entity> entity);
    void spawnBullet(ptr<Entity> entity, const Vec2& mousePos,bool isSpecialBullet);
    void spawnSpecialWeapon(ptr<Entity> entity,Vec2& mousePos);
public:
    Game(const std::string filename);
    void run();
};


#endif //GEOMETRY_WARS_GAME_H
