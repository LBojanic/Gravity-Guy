#include "player.h"
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
#include <QGraphicsScene>
#include "enemy.h"
Player::Player(Enemy * enemy) : m_enemy(enemy)
{
    //representing player as rectangle 100x100
    setRect(0, 0, 100, 100);
    //setting initial gravity for player
    setGravity(1);
    //make rect focusable so we can use keyboard events
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    jumpSound = new QMediaPlayer();
    jumpSound->setMedia(QUrl("qrc:/sounds/jump.wav"));

    //creating timer object for movement
    QTimer * timer = new QTimer();
    //setting timer signals and slots
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    //starting the timer
    timer->start(30);
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
        if(y() + rect().height() < scene()->height()) {
            setPos(x(), y() + 10);
        } else {
            // gameover
        }
    }
    else
    {
        //if our player goes beyond upper limit then its gameover, otherwise translate
        if(y() > 0) {
            setPos(x(), y() - 10);
        } else {
            // gameover
        }

    }
}

//setting callback function for keypressevent
void Player::keyPressEvent(QKeyEvent *event)
{
    //if a space key is pressed, change gravity for player and after 500 ms send signal to enemy spaceEvent slot
    if(event->key() == Qt::Key_Space) {
        setGravity(!gravity());
        //The following code plays a sound when a player jumps
        // The if-else is added because if we simply put 'jumpSound->play' it will only play once
        // so we need to check if a sound has already been played and repeating if it was.
        if(jumpSound->state() == QMediaPlayer::PlayingState) { //If a sound has been played set it to beginning
            jumpSound->setPosition(0);
        } else if (jumpSound->state() == QMediaPlayer::StoppedState) { // if a sound is stopped just play
            jumpSound->play();
        }


        QTimer::singleShot(500, m_enemy, SLOT(spaceEvent()));
    }


}

