#ifndef PLAYBACK_H
#define PLAYBACK_H

#include <QDialog>
#include <QThread>

namespace Ui {
class PlayBack;
}

class PlayBackThread :public QThread
{
    Q_OBJECT
public:
    bool PlayBackRunning;

    PlayBackThread();
    virtual void run();  //线程函数，新创建的线程运行该函数内容。
    void stop();     //停止线程
signals:
    void sendplayImage(QImage msg);
private:

};

class PlayBack : public QDialog
{
    Q_OBJECT

public:
    explicit PlayBack(QWidget *parent = nullptr);
    ~PlayBack();

    PlayBackThread *PlayBack_thread = new (PlayBackThread);

private slots:
    void acceptplayImage(QImage msg);

private:
    Ui::PlayBack *ui;
};

#endif // PLAYBACK_H
