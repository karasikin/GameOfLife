#ifndef WORLDEDITOR_H
#define WORLDEDITOR_H

#include <map>
#include <QWidget>
#include "world.h"

class QPlainTextEdit;
class QPushButton;


class WorldEditor : public QWidget
{

    Q_OBJECT

    struct WorldMask {
        explicit WorldMask(std::unique_ptr<World> world);

        std::unique_ptr<World> world;
        QRect rect;
    };


public:

    explicit WorldEditor(QWidget *parent = nullptr);

public slots:

    void onStartBtnClicked();
    void onReturnBtnClicked();

signals:

    void becomeHidden();
    void becomeClosed();

protected:

    void closeEvent(QCloseEvent *event) override;

private:

    std::unique_ptr<QStringList> split(const QString &text) const;
    bool loadWorld(const QString &codeline);

    QString createQString(QString::ConstIterator begin, QString::ConstIterator end) const;

private:

    std::unique_ptr<World> _world;
    std::map<QString, WorldMask> loadedWorlds;

    QPlainTextEdit *_codeEdit;
    QPlainTextEdit *_errorReport;
    QPushButton *_startBtn;
    QPushButton *_returnBtn;

};

#endif // WORLDEDITOR_H
