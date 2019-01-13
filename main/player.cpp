#include "player.h"
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
#include <QGraphicsScene>
#include <QThread>
#include "enemy.h"
#include <QDebug>
#include "game.h"
#include <QScrollBar>
#include "globals.h"
#include <QtMath>
#include "block.h"
#include "coin.h"

Player::Player(Enemy * enemy) : m_enemy(enemy)
{
    //draw graphics
    playerCurrentImage = 0;
    //setting initial gravity for player
    setGravity(1);
    setPixmap(QPixmap(":/images/player/run/" + QString::number(playerCurrentImage) + QString::number(gravity()) + ".png").scaled(125, 125));

    //make rect focusable so we can use keyboard events
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    jumpSound = new QMediaPlayer();
    jumpSound->setMedia(QUrl("qrc:/sounds/jump.wav"));

    //creating timer object for movement
    timerMove = new QTimer();
    //setting timer signals and slots
    connect(timerMove, SIGNAL(timeout()), this, SLOT(move()));
    //starting the timer
    timerMove->start(5);

    //creating timer object for animation
    timerChangeImagePlayer = new QTimer();
    //setting timer signals and slots
    connect(timerChangeImagePlayer, SIGNAL(timeout()), this, SLOT(changeImage()));
    connect(timerChangeImagePlayer, SIGNAL(timeout()), m_enemy, SLOT(changeImage()));
    //starting the timer
    timerChangeImagePlayer->start(60);
}

//gravity setter
void Player::setGravity(int value)
{
    m_gravity = value;
}

//gravity getter
int Player::gravity()
{
    return m_gravity;
}
//player destructor
Player::~Player()
{

}
//player movement
void Player::move()
{
    //if gravity == 1 then he must be moving in downward position
    if(gravity() == 1)
    {
        //if our player goes beneath window lower limit then its gameover, otherwise translate
        if(y() + sceneBoundingRect().height() < scene()->height() && !collidesWithBlocks(game->blocks)) {
            setPos(x(), y() + 1);
        } else if(y() + sceneBoundingRect().height() >= scene()->height() && !collidesWithBlocks(game->blocks)) {
            setPos(x(), y() + 2);
        }
    }
    else
    {
        //if our player goes beyond upper limit then its gameover, otherwise translate
        if(y() > 0 && !collidesWithBlocks(game->blocks)) {
            setPos(x(), y() - 1);
        } else if(y() <= 0 && !collidesWithBlocks(game->blocks)) {
            setPos(x(), y() - 2);
        }

    }
}

void Player::changeImage()
{
    playerCurrentImage = (playerCurrentImage + 1) % 8;
    setPixmap(QPixmap(":/images/player/run/" + QString::number(playerCurrentImage) + QString::number(gravity()) + ".png").scaled(125, 125));
}

void Player::advance()
{
    if(qAbs(game->horizontalScrollBar()->value() - 81*125) <= 2)
    {
        qDebug() << game->horizontalScrollBar()->value();
        game->setBackgroundBrush(QBrush(QImage(":/images/background2.png").scaled(1280, 700)));
    }

    if(!crashesIntoBlock(game->blocks) &&
            qFabs((game->horizontalScrollBar()->value()*1.0 + game->width()/2.0) - (x() + boundingRect().width()/2)) > 1)
    {
        QTimer::singleShot(5, this, SLOT(goToMiddle()));
    }

    if(!m_enemy->crashesIntoBlock(game->blocks) &&
            qFabs((game->horizontalScrollBar()->value() + 100) - m_enemy->x()) > 1)
    {
        QTimer::singleShot(5, m_enemy, SLOT(goToPosition()));
    }

    auto a = collidingItem(game->blocks);
    if(a && typeid(*(a)) == typeid(Coin)){
        if(!game->backgroundMusic->isMuted()){
            QMediaPlayer* coinSound = new QMediaPlayer();
            coinSound->setMedia(QUrl("qrc:/sounds/coin.wav"));
            coinSound->play();
        }

        game->score->setScore(game->score->getScore() + 30);

        int i = game->blocks.indexOf(a);
        game->blocks.removeAt(i);
        game->scene->removeItem(a);
        delete a;

    }

    if(!m_enemy->crashesIntoBlock(game->blocks))
         m_enemy->setPos(m_enemy->x() + 2, m_enemy->y());

    auto b = crashesIntoBlock(game->blocks);
    if(b == nullptr || typeid(*b) == typeid(Coin)) //move only if player doesn't collide with block on it's path
        setPos(x() + 2, y());
    game->horizontalScrollBar()->setValue(game->horizontalScrollBar()->value() + 2);
    if(game->horizontalScrollBar()->value() != game->horizontalScrollBar()->maximum()){
        game->score->setPos(game->score->x() + 2, game->score->y());
        game->soundButton->setPos(game->soundButton->x() + 2, game->soundButton->y());
        game->pauseButton->setPos(game->pauseButton->x() + 2, game->pauseButton->y());
    }

    /* if the distance between the enemy and the position where the player has jumped is less than
    2 then the enemy should also change gravity because the enemy needs to jump on the same
    position where the player has jumped */
    if(!coordinatesWhereEnemyChanges.isEmpty()){
        if(game->distance(coordinatesWhereEnemyChanges.first().first,
                    coordinatesWhereEnemyChanges.first().second,
                    m_enemy->x(),
                    m_enemy->y()) < 2) {
                m_enemy->spaceEvent();
                coordinatesWhereEnemyChanges.pop_front();
        }
    }

    if(!m_enemy->collidingItems().isEmpty()){
        auto tmp = m_enemy->collidingItems();
        for(auto i : tmp){
            if(typeid (*i) == typeid (Player))
                game->gameOver();
        }
    }
}

void Player::goToMiddle()
{
    setPos(x() + 1, y());
}


//setting callback function for keypressevent
void Player::keyPressEvent(QKeyEvent *event)
{
    //if a space key is pressed, change gravity for player and after 500 ms send signal to enemy spaceEvent slot
    if(event->key() == Qt::Key_Space && collidesWithBlocks(game->blocks)) {
        setGravity(!gravity());
        //The following code plays a sound when a player jumps
        // The if-else is added because if we simply put 'jumpSound->play' it will only play once
        // so we need to check if a sound has already been played and repeating if it was.
        if(jumpSound->state() == QMediaPlayer::PlayingState) { //If a sound has been played set it to beginning
            jumpSound->setPosition(0);
        } else if (jumpSound->state() == QMediaPlayer::StoppedState) { // if a sound is stopped just play
            jumpSound->play();
        }
        coordinatesWhereEnemyChanges.push_back(QPair<qreal, qreal>(x(), y()));
    }
}

QGraphicsPixmapItem* Player::collidesWithBlocks(QList<QGraphicsPixmapItem *> blocks) {
    mutex->lock();
    qreal playerTop = pos().y(); //player top coordinate
    qreal playerBottom = pos().y() + boundingRect().height(); //player bottom coordinate
    qreal playerLeft = pos().x(); // player left coordinate
    qreal playerRight = pos().x() + boundingRect().width(); // player right coordinate
    for(int i = 0; i < blocks.length(); i++) {
        qreal blockTop = blocks[i]->pos().y(); //block top coordinate
        qreal blockBottom = blocks[i]->pos().y() + blocks[i]->boundingRect().height();//block bottom coordinate
        qreal blockLeft = blocks[i]->pos().x();// block left coordinate
        qreal blockRight = blocks[i]->pos().x() + blocks[i]->boundingRect().width();// block right coordinate

        //if gravity is 1 then player is moving downward so we need to check if he touches block top with his bottom meaning
        //the difference between player bottom and block top should be max 1 pixel since player moves 1 pixel down with gravity
        // after that we must check if players left edge or right edge is between block's x left and rigth edges
        // we have 2 cases, players right edge is between block's left and right and case where player's left edge is between
        if(gravity() == 1 && qFabs(playerBottom - blockTop) < 1 && ((playerLeft <= blockRight && playerLeft >= blockLeft) || (playerRight >= blockLeft && playerRight <= blockRight))) {
            mutex->unlock();
            return blocks[i];
        }
        if(gravity() == 0 && qFabs(playerTop - blockBottom) < 1 && ((playerRight >= blockLeft && playerRight <= blockRight) || (playerLeft >= blockLeft && playerLeft <= blockRight))) {
            mutex->unlock();
            return blocks[i];
        }
    }
    mutex->unlock();
    return nullptr;
}

QGraphicsPixmapItem *Player::crashesIntoBlock(QList<QGraphicsPixmapItem *> blocks)
{
    mutex->lock();
    qreal playerTop = pos().y(); //player top coordinate
    qreal playerBottom = pos().y() + boundingRect().height(); //player bottom coordinate
    qreal playerRight = pos().x() + boundingRect().width(); // player right coordinate
    for(int i = 0; i < blocks.length(); i++) {
        qreal blockTop = blocks[i]->pos().y(); //block top coordinate
        qreal blockBottom = blocks[i]->pos().y() + blocks[i]->boundingRect().height();//block bottom coordinate
        qreal blockLeft = blocks[i]->pos().x();// block left coordinate

        if(qFabs(playerRight - blockLeft) <= 2 && ((playerTop <= blockBottom && playerTop >= blockTop) || (playerBottom >= blockTop && playerBottom <= blockBottom))) {
            mutex->unlock();
            return blocks[i];
        }
    }
    mutex->unlock();
    return nullptr;
}

QGraphicsPixmapItem *Player::collidingItem(QList<QGraphicsPixmapItem *> blocks)
{
    for(int i = 0; i < blocks.size(); i++)
    {
        if(collidesWithItem(blocks.at(i)))
            return blocks[i];
    }
}

Enemy* Player::enemy()
{
    return m_enemy;
}

void Player::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    this->setFocus();
}
void Player::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
}

QMediaPlayer *Player::getJumpSound()
{
    return jumpSound;
}
