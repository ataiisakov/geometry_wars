//
// Created by cpp on 08.04.22.
//

#include "Game.h"

using namespace std;

Game::Game(const std::string filename) {
    std::cout << "game constructor" << std::endl;
    init(filename);
}

bool Game::inBounds(ptr<Entity> entity) {
    float radius = entity->cCollision->radius;
    Vec2 pos = entity->cTransform->pos;
    bool top = pos.mY - radius > 0;
    bool bottom = pos.mY + radius < mWindow.getSize().y;
    bool left = pos.mX - radius > 0;
    bool right = pos.mX + radius < mWindow.getSize().x;
    return top && bottom && left && right;
}

void Game::readConfigFile(const std::string &config) {
    std::ifstream fin;
    std::string componentCategory;
    fin.open(config);
    if (fin.is_open()) {
        while (!fin.eof()) {
            fin >> componentCategory;
            cout << componentCategory << endl;
            if (componentCategory == "Window") {
                fin >> mWindowConfig.W >> mWindowConfig.H >> mWindowConfig.FL >> mWindowConfig.FS;
                cout << mWindowConfig << endl;
            } else if (componentCategory == "Player") {
                fin >> mPlayerConfig.SR >> mPlayerConfig.CR >> mPlayerConfig.S >> mPlayerConfig.FR >> mPlayerConfig.FG
                    >> mPlayerConfig.FB >> mPlayerConfig.OR >> mPlayerConfig.OG >> mPlayerConfig.OB >> mPlayerConfig.OT
                    >> mPlayerConfig.V;
                cout << mPlayerConfig << endl;
            } else if (componentCategory == "Enemy") {
                fin >> mEnemyConfig.SR >> mEnemyConfig.CR >> mEnemyConfig.SMIN >> mEnemyConfig.SMAX >> mEnemyConfig.OR
                    >> mEnemyConfig.OG >> mEnemyConfig.OB >> mEnemyConfig.OT >> mEnemyConfig.VMIN >> mEnemyConfig.VMAX
                    >> mEnemyConfig.L >> mEnemyConfig.SI;
                cout << mEnemyConfig << endl;
            } else if (componentCategory == "Bullet") {
                fin >> mBulletConfig.SR >> mBulletConfig.CR >> mBulletConfig.S >> mBulletConfig.FR >> mBulletConfig.FG
                    >> mBulletConfig.FB >> mBulletConfig.OR >> mBulletConfig.OG >> mBulletConfig.OB >> mBulletConfig.OT
                    >> mBulletConfig.V >> mBulletConfig.L;
                cout << mBulletConfig << endl;
            } else if (componentCategory == "Font") {
                fin >> mFontConfig.F >> mFontConfig.S >> mFontConfig.R >> mFontConfig.G >> mFontConfig.B;
                cout << mFontConfig << endl;
            }
        }
    } else {
        std::cout << "file not open" << std::endl;
    }

}

void Game::initText() {
    if (!mFont.loadFromFile(mFontConfig.F)) {
        std::cout << "can't load font" << std::endl;
    }
    mScoreText.setFont(mFont);
    mSpecialWeaponText.setFont(mFont);
    mScoreText.setString(mScoreSS.str());
    mSpecialWeaponText.setString(mSpecialWeaponSS.str());
    mScoreText.setCharacterSize(mFontConfig.S);
    mSpecialWeaponText.setCharacterSize(mFontConfig.S);
    mSpecialWeaponText.setCharacterSize(mFontConfig.S);
    mScoreText.setFillColor(sf::Color(mFontConfig.R, mFontConfig.G, mFontConfig.B));
    mSpecialWeaponText.setFillColor(sf::Color(mFontConfig.R, mFontConfig.G, mFontConfig.B));
    mScoreText.setPosition(0, 0);
    mSpecialWeaponText.setPosition(250, 0);
}
void Game::init(const std::string &config) {
    readConfigFile(config);
    mWindow.create((mWindowConfig.FS == 0) ?
                   sf::VideoMode(mWindowConfig.W, mWindowConfig.H) :
                   sf::VideoMode::getFullscreenModes()[0], mGameName);
    mWindow.setFramerateLimit(mWindowConfig.FL);
    initText();
    setScoreText();
    setSpecialWeaponText();
    spawnPlayer();
}


void Game::run() {
    while (mRunning) {
        mEntities.update();
        if (!mPaused) {
            sEnemySpawner();
            sMovement();
            sLifespan();
            mCurrentFrame++;
        }
        sUserInput();
        sCollision();
        sRender();
    }
}

void Game::setScoreText() {
    mScoreSS.str("");
    mScoreSS << "Score: " << score;
    mScoreText.setString(mScoreSS.str());
}

void Game::setSpecialWeaponText() {
    mSpecialWeaponSS.str("");
    mSpecialWeaponSS << "Special Weapon: " << (mIsSpecialWeaponAvailable?"Available":"Used");
    mSpecialWeaponText.setString(mSpecialWeaponSS.str());
}

void Game::sRender() {
    mWindow.clear();

    for (const auto entity: mEntities.getEntities()) {
        if (entity->getTag() == EntityTag::Player || entity->getTag() == EntityTag::Enemy ||
            entity->getTag() == EntityTag::SmallEnemy) {
            entity->cTransform->angle += 1;
            entity->cShape->circle.setRotation(mPlayer->cTransform->angle);
        }
        if (entity->cShape) {
            entity->cShape->circle.setPosition(entity->cTransform->pos.mX, entity->cTransform->pos.mY);
            mWindow.draw(entity->cShape->circle);
        }
    }
    mWindow.draw(mScoreText);

    if (!mIsSpecialWeaponAvailable &&
        (float(mCurrentFrame - mLastSpecialWeapon) / mWindowConfig.FL) >= mSpecialWeaponCoolDownSec) {
        mIsSpecialWeaponAvailable = true;
    }
    setSpecialWeaponText();
    mWindow.draw(mSpecialWeaponText);
    mWindow.display();
}

void Game::resetScore() {
    score = 0;
    mPlayer->cScore->score = 0;
    setScoreText();
}

void Game::spawnPlayer() {
    auto playerEntity = mEntities.addEntity(EntityTag::Player);
    playerEntity->cTransform = std::make_shared<CTransform>(
            Vec2(mWindow.getView().getCenter().x, mWindow.getView().getCenter().y), Vec2(0, 0), 0);
    playerEntity->cShape = std::make_shared<CShape>(mPlayerConfig.SR, mPlayerConfig.V,
                                                    sf::Color(mPlayerConfig.FR, mPlayerConfig.FG, mPlayerConfig.FB),
                                                    sf::Color(mPlayerConfig.OR, mPlayerConfig.OG, mPlayerConfig.OB),
                                                    mPlayerConfig.OT);
    playerEntity->cInput = std::make_shared<CInput>();
    playerEntity->cCollision = std::make_shared<CCollision>(mPlayerConfig.CR);
    playerEntity->cScore = std::make_shared<CScore>(0);
    mPlayer = playerEntity;
}

void Game::sEnemySpawner() {

    if ((mCurrentFrame - mLastEnemySpawnTime) >= mEnemyConfig.SI) {
        spawnEnemy();
        mLastEnemySpawnTime = mCurrentFrame;
    }
}

void Game::spawnEnemy() {
    float randomY = mEnemyConfig.CR + (std::rand() % int(1 + mWindow.getSize().y - mEnemyConfig.CR));
    float randomX = mEnemyConfig.CR + (std::rand() % int(1 + mWindow.getSize().x - mEnemyConfig.CR));

    float randomPoints = mEnemyConfig.VMIN + (std::rand() % (1 + mEnemyConfig.VMAX - mEnemyConfig.VMIN));
    float randomSpeed = mEnemyConfig.SMIN + (std::rand() % int(1 + mEnemyConfig.SMAX - mEnemyConfig.SMIN));
    int randomAngle = (rand() % 361);

    int randomFR = (std::rand() % (256));
    int randomFG = (std::rand() % (256));
    int randomFB = (std::rand() % (256));

    int OR = 255;
    int OG = 255;
    int OB = 255;
    auto entity = mEntities.addEntity(EntityTag::Enemy);

    entity->cTransform = std::make_shared<CTransform>(Vec2(randomX, randomY), Vec2(0, 0), randomAngle);
    entity->cShape = std::make_shared<CShape>(mEnemyConfig.SR, randomPoints, sf::Color(randomFR, randomFG, randomFB),
                                              sf::Color(OR, OG, OB), mEnemyConfig.OT);
    entity->cCollision = std::make_shared<CCollision>(mEnemyConfig.CR);

    float x = cos(toRadians(entity->cTransform->angle));
    float y = sin(toRadians(entity->cTransform->angle));
    Vec2 vel(x, y);
    vel.normalize();
    vel = vel * randomSpeed;
    entity->cTransform->velocity = vel;
}

void Game::spawnBullet(ptr<Entity> entity, const Vec2 &mousePos, bool isSpecialBullet = false) {

    auto bullet = mEntities.addEntity(EntityTag::Bullet);
    Vec2 diff = mousePos - entity->cTransform->pos;
    float angle = atan2f(diff.mY, diff.mX);
    sf::Color fillColor(mBulletConfig.FR, mBulletConfig.FG, mBulletConfig.FB);
    if (isSpecialBullet){
        fillColor = sf::Color::Red;
    }
    sf::Color outlineColor(mBulletConfig.OR, mBulletConfig.OG, mBulletConfig.OB);
    bullet->cCollision = make_shared<CCollision>(mBulletConfig.CR);
    bullet->cTransform = make_shared<CTransform>(entity->cTransform->pos, Vec2(0, 0), angle);
    bullet->cShape = make_shared<CShape>(mBulletConfig.SR, mBulletConfig.V,
                                         fillColor,outlineColor,mBulletConfig.OT);
    bullet->cLifespan = make_shared<CLifespan>(mBulletConfig.L);
    diff.normalize();
    bullet->cTransform->velocity = diff * mBulletConfig.S;

}

void Game::spawnSmallEnemies(ptr<Entity> entity) {

    int numPoints = entity->cShape->circle.getPointCount();
    int angle = 0;

    float parentThickness = entity->cShape->circle.getOutlineThickness();
    sf::Color parentOutlineColor = entity->cShape->circle.getOutlineColor();
    sf::Color parentFillColor = entity->cShape->circle.getFillColor();

    Vec2 parentPos = Vec2(entity->cShape->circle.getPosition().x, entity->cShape->circle.getPosition().y);
    Vec2 parentVel = entity->cTransform->velocity;

    for (int i = 0; i < numPoints; ++i) {
        auto smallEnemy = mEntities.addEntity(EntityTag::SmallEnemy);

        smallEnemy->cTransform = make_shared<CTransform>(parentPos, parentVel, entity->cTransform->angle);
        smallEnemy->cCollision = make_shared<CCollision>(entity->cCollision->radius / 2);
        smallEnemy->cShape = make_shared<CShape>(entity->cShape->circle.getRadius() / 2,
                                                 numPoints, parentFillColor,
                                                 parentOutlineColor,
                                                 parentThickness);

        smallEnemy->cLifespan = make_shared<CLifespan>(mEnemyConfig.L);

        float pointAngle = angle * i;

        Vec2 vel{
                cos(toRadians(angle)),
                sin(toRadians(angle))
        };

        smallEnemy->cTransform->velocity = vel;
        angle += 360 / numPoints;

    }

}
// without sqrt operation what makes it more efficient
bool Game::isCollided(ptr<Entity> e1, ptr<Entity> e2) {
    Vec2 e1Pos = e1->cTransform->pos;
    Vec2 e2Pos = e2->cTransform->pos;
    double distSq = (e1Pos.mX - e2Pos.mX) * (e1Pos.mX - e2Pos.mX) + (e1Pos.mY - e2Pos.mY) * (e1Pos.mY - e2Pos.mY);
    int radSq = (e1->cCollision->radius + e2->cCollision->radius) * (e1->cCollision->radius + e2->cCollision->radius);
    return distSq < radSq;
}

void Game::sCollision() {
    for (const auto enemy: mEntities.getEntities()) {
        if (enemy->getTag() == EntityTag::Enemy || enemy->getTag() == EntityTag::SmallEnemy) {
            if (enemy->cTransform) {
                if (isCollided(mPlayer, enemy)) {
                    mPlayer->destroy();
                    resetScore();
                    spawnPlayer();
                }
            }
        }
    }
    for (auto &b: mEntities.getEntities(EntityTag::Bullet)) {
        if (!inBounds(b)) {
            b->destroy();
            continue;
        }
        for (auto &e: mEntities.getEntities()) {
            if (e->getTag() == EntityTag::Enemy || e->getTag() == EntityTag::SmallEnemy) {
                if (isCollided(b, e)) {
                    if (e->getTag() == EntityTag::Enemy){
                        score += e->cShape->circle.getPointCount() * 100;
                    } else if(e->getTag() == EntityTag::SmallEnemy){
                        score += e->cShape->circle.getPointCount() * 200 ;
                    }
                    e->destroy();
                    b->destroy();
                    setScoreText();
                    if (e->getTag() != EntityTag::SmallEnemy) spawnSmallEnemies(e);
                    mPlayer->cScore->score += score;
                }
            }
        }
    }


    for (const auto e: mEntities.getEntities()) {
        if (e->getTag() == EntityTag::Enemy || e->getTag() == EntityTag::SmallEnemy) {
            if (!e->cTransform || !e->cCollision)continue;

            int enemyRadius = e->cCollision->radius;

            int windowWidth = mWindow.getSize().x;
            int windowHeight = mWindow.getSize().y;

            bool outsideUpperBound = e->cTransform->pos.mY - enemyRadius < 0;
            bool outsideLowerBound = e->cTransform->pos.mY + enemyRadius > windowHeight;
            bool outsideLeftBound = e->cTransform->pos.mX - enemyRadius < 0;
            bool outsideRightBound = e->cTransform->pos.mX + enemyRadius > windowWidth;

            if (outsideLeftBound) e->cTransform->velocity.mX *= -1;
            if (outsideRightBound) e->cTransform->velocity.mX *= -1;
            if (outsideUpperBound) e->cTransform->velocity.mY *= -1;
            if (outsideLowerBound) e->cTransform->velocity.mY *= -1;
        }

    }


}

void Game::sLifespan() {
    for (const auto e: mEntities.getEntities()) {
        if (!e->cLifespan) continue;
        if (e->cLifespan->remaining > 0) {
            sf::Color color = e->cShape->circle.getFillColor();
            int newAlpha = (e->cLifespan->remaining * 1.0F / e->cLifespan->total) * 255;
            e->cShape->circle.setFillColor(sf::Color(color.r, color.g, color.b, newAlpha));
            e->cLifespan->remaining--;
        }
        if (e->cLifespan->remaining <= 0) {
            e->destroy();
        }

    }
}

void Game::sMovement() {
    if (!mPlayer->cTransform)return;

    Vec2 playerVelocity;
    if (mPlayer->cInput->left && mPlayer->cTransform->pos.mX - mPlayer->cCollision->radius > 0) {
        playerVelocity.mX -= mPlayerConfig.S;
    }
    if (mPlayer->cInput->right && mPlayer->cCollision->radius + mPlayer->cTransform->pos.mX < mWindow.getSize().x) {
        playerVelocity.mX += mPlayerConfig.S;
    }
    if (mPlayer->cInput->up && mPlayer->cTransform->pos.mY - mPlayer->cCollision->radius > 0) {
        playerVelocity.mY -= mPlayerConfig.S;
    }
    if (mPlayer->cInput->down && mPlayer->cCollision->radius + mPlayer->cTransform->pos.mY < mWindow.getSize().y) {
        playerVelocity.mY += mPlayerConfig.S;
    }
    mPlayer->cTransform->velocity = playerVelocity;


    for (const auto entity: mEntities.getEntities()) {
        if (entity->cTransform) {
            entity->cTransform->pos += entity->cTransform->velocity;
        }
    }

}

void Game::spawnSpecialWeapon(ptr<Entity> entity,Vec2& mousePos) {

    if (!mIsSpecialWeaponAvailable &&
        (float(mCurrentFrame - mLastSpecialWeapon) / mWindowConfig.FL) < mSpecialWeaponCoolDownSec)
        return;
    entity->cTransform->pos = mousePos;
    for (int i = 0; i < 360; i += 10) {
        float x = cos(toRadians(i));
        float y = sin(toRadians(i));
        Vec2 bulletDirection = mPlayer->cTransform->pos + Vec2(x, y);
        spawnBullet(entity, bulletDirection, true);
    }
    mLastSpecialWeapon = mCurrentFrame;
    mIsSpecialWeaponAvailable = false;
}

void Game::setPaused() {
    mPaused = !mPaused;
}

void Game::sUserInput() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            mRunning = false;
        }
        if (event.type == sf::Event::MouseButtonPressed && !mPaused) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                Vec2 mousePos(event.mouseButton.x, event.mouseButton.y);
                spawnBullet(mPlayer, mousePos);
            }
            if (event.mouseButton.button == sf::Mouse::Right) {
                Vec2 mousePos(event.mouseButton.x, event.mouseButton.y);
                spawnSpecialWeapon(mPlayer,mousePos);
            }
        }

        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::W:
                    mPlayer->cInput->up = true;
                    break;
                case sf::Keyboard::S:
                    mPlayer->cInput->down = true;
                    break;
                case sf::Keyboard::A:
                    mPlayer->cInput->left = true;
                    break;
                case sf::Keyboard::D:
                    mPlayer->cInput->right = true;
                    break;
                case sf::Keyboard::P:
                    setPaused();
                    break;
                case sf::Keyboard::Escape:
                    mRunning = false;
                    break;
                default:
                    break;
            }
        }
        if (event.type == sf::Event::KeyReleased) {
            switch (event.key.code) {
                case sf::Keyboard::W:
                    mPlayer->cInput->up = false;
                    break;
                case sf::Keyboard::S:
                    mPlayer->cInput->down = false;
                    break;
                case sf::Keyboard::A:
                    mPlayer->cInput->left = false;
                    break;
                case sf::Keyboard::D:
                    mPlayer->cInput->right = false;
                    break;
                default:
                    break;
            }
        }
    }
}

ostream &operator<<(ostream &os, const BulletConfig &config) {
    os << "SR: " << config.SR << " CR: " << config.CR << " S: " << config.S << " FR: " << config.FR << " FG: "
       << config.FG << " FB: " << config.FB << " OR: " << config.OR << " OG: " << config.OG << " OB: " << config.OB
       << " OT: " << config.OT << " V: " << config.V << " L: " << config.L;
    return os;
}

ostream &operator<<(ostream &os, const EnemyConfig &config) {
    os << "SR: " << config.SR << " CR: " << config.CR << " SMIN: " << config.SMIN << " SMAX: " << config.SMAX << " OR: "
       << config.OR << " OG: " << config.OG << " OB: " << config.OB << " OT: " << config.OT << " VMIN: " << config.VMIN
       << " VMAX: " << config.VMAX << " L: " << config.L << " SI: " << config.SI;
    return os;
}

ostream &operator<<(ostream &os, const PlayerConfig &config) {
    os << "SR: " << config.SR << " CR: " << config.CR << " S: " << config.S << " FR: " << config.FR << " FG: "
       << config.FG << " FB: " << config.FB << " OR: " << config.OR << " OG: " << config.OG << " OB: " << config.OB
       << " OT: " << config.OT << " V: " << config.V;
    return os;
}

ostream &operator<<(ostream &os, const FontConfig &config) {
    os << "F: " << config.F << " S: " << config.S << " R: " << config.R << " G: " << config.G << " B: " << config.B;
    return os;
}

ostream &operator<<(ostream &os, const WindowConfig &config) {
    os << "W: " << config.W << " H: " << config.H << " FL: " << config.FL << " FS: " << config.FS;
    return os;
}
