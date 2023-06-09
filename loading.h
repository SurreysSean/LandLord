#ifndef LOADING_H
#define LOADING_H

#include <QWidget>

class Loading :public QWidget
{
//    singleton
    Q_OBJECT
private:
    explicit Loading(QWidget* parent = nullptr);
    Loading (const Loading &) =delete;
    Loading (const Loading &&) =delete;
public:
    static std::shared_ptr<Loading> getInstance();
protected:
    void paintEvent(QPaintEvent* event);
private:
    QPixmap m_bk;
    QPixmap m_progress;
    int m_dist;
//    static Loading * instance;
    static std::shared_ptr<Loading> instance;
};

#endif // LOADING_H
