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

    struct WorldMaskRect {
        explicit WorldMaskRect(size_t x = 0, size_t y = 0, size_t row = 0, size_t col = 0);

        size_t x, y, row, col;
    };

    struct WorldMask {
        explicit WorldMask(World *worldPtr = nullptr);

        World *worldPtr;
        WorldMaskRect rect;
    };


public:

    explicit WorldEditor(QWidget *parent = nullptr);

public slots:

    void onStartBtnClicked();
    void onReturnBtnClicked();

signals:

    void becomeHidden();
    void createdWorld(const World &);

protected:

    void closeEvent(QCloseEvent *event) override;

private:

    std::unique_ptr<QStringList> split(const QString &text) const;

    bool loadWorld(const QString &codeline);
    bool returnWorld(const QString &codeLine);

    QString createQString(QString::ConstIterator begin, QString::ConstIterator end) const;

private:

    std::unique_ptr<World> _world;

    std::map<QString, WorldMask> _worlds;
    std::map<QString, World> _cache;

    QPlainTextEdit *_codeEdit;
    QPlainTextEdit *_reportEdit;
    QPushButton *_startBtn;
    QPushButton *_returnBtn;

};

#endif // WORLDEDITOR_H
