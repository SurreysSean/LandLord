#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <QMainWindow>
#include <QLabel>
#include <QMap>

#include "cardpanel.h"
#include "gamecontrol.h"
#include "animationwindow.h"
#include "player.h"
#include "countdown.h"
#include "bgmcontrol.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class GamePanel : public QMainWindow
{
    Q_OBJECT

public:
    GamePanel(QWidget *parent = nullptr);
    ~GamePanel();
    enum AnimationType{ShunZi, LianDui, Plane, JokerBomb, Bomb, Bet};
    void gameControlInit();
    void updatePlayerScore();
    void initCardMap();
    void cropImage(QPixmap& pix, int x, int y, Card& c);
    void initButtonsGroup();
    void initPlayerContext();
    void initGameScene();
    void gameStatusPrecess(GameControl::GameStatus status);
    void startDispatchCard();
    void cardMoveStep(Player* player, int curPos);
    void disposeCard(Player* player, const Cards& cards);
    void updatePlayerCards(Player* player);
    QPixmap loadRoleImage(Player::Sex sex, Player::Direction direct, Player::Role role);

    void onDispatchCard();
    void onPlayerStatusChanged(Player* player, GameControl::PlayerStatus status);
    void onGrabLordBet(Player* player, int bet, bool flag);
    void onDisposePlayHand(Player* player, const Cards& cards);
    void onCardSelected(Qt::MouseButton button);
    void onUserPlayHand();
    void onUserPass();

    void showAnimation(AnimationType type, int bet = 0);
    void hidePlayerDropCards(Player* player);
    void showEndingScorePanel();
    void initCountDown();

private:
    Ui::MainWindow *ui;
};
#endif // GAMEPANEL_H
