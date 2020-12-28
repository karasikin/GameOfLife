#include "worldeditor.h"

#include <QPlainTextEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QCloseEvent>

#include <QDebug>

WorldEditor::WorldMask::WorldMask(std::unique_ptr<World> world) {
    this->world = std::move(world);
    rect = QRect{0, 0, int(this->world->row()), int(this->world->col()) };
}


WorldEditor::WorldEditor(QWidget *parent)
    : QWidget(parent),
      _world(nullptr),
      _codeEdit(new QPlainTextEdit{}),
      _errorReport(new QPlainTextEdit{}),
      _startBtn(new QPushButton{"&Start"}),
    _returnBtn(new QPushButton{"&Return"})

{
    _errorReport->setReadOnly(true);

    auto layout{new QGridLayout};
    layout->addWidget(_codeEdit, 0, 0, 6, 10);
    layout->addWidget(_errorReport, 6, 0, 2, 9);
    layout->addWidget(_startBtn, 6, 9, 1, 1);
    layout->addWidget(_returnBtn, 7, 9, 1, 1);
    setLayout(layout);

    connect(_startBtn, &QPushButton::clicked, this, &WorldEditor::onStartBtnClicked);
    connect(_returnBtn, &QPushButton::clicked, this, &WorldEditor::onReturnBtnClicked);
}

//void WorldEditor::setWorld(World *world) {
//    _world = world;
//}

void WorldEditor::onStartBtnClicked() {
    auto splittedText{ split(_codeEdit->toPlainText()) };

    auto it_load{ std::find(splittedText->cbegin(), splittedText->cend(), QString("load:")) };
    auto it_make{ std::find(splittedText->cbegin(), splittedText->cend(), QString("make:")) };
    auto it_return{ std::find(splittedText->cbegin(), splittedText->cend(), QString("return:")) };
    auto not_found{ splittedText->cend() };

    if(it_load == not_found || it_make == not_found || it_return == not_found) {
        _world = nullptr;     // Ошибка!!!!! Записать _errorReport
        _errorReport->appendPlainText("Not Found: \"load:\" and \"return:\"\n"); // Написать норамльно
        return;
    }

    for(auto it{ std::next(it_load) }; it != it_make && it != it_return && it != not_found; ++it) {
        if(!loadWorld(*it)) {
            _world = nullptr;  // Ошибка!!!!! Записать _errorReport
            _errorReport->appendPlainText(QString("bad operator: %1").arg(*it));  // то же самое !!!!!!!!!111!!!!!!!
            return;
        }
    }
}

void WorldEditor::onReturnBtnClicked() {
    this->hide();
    emit becomeHidden();
}

void WorldEditor::closeEvent(QCloseEvent *event) {
    this->hide();
    event->ignore();
    emit becomeHidden();
}


std::unique_ptr<QStringList> WorldEditor::split(const QString &text) const {
    auto lines{ std::make_unique<QStringList>() };
    for(auto &string : text.split('\n', Qt::SkipEmptyParts)) {
        lines->push_back(string.simplified());
    }

    for(auto it{lines->begin()}; it != lines->end();) {
        if(it->startsWith("#")) {
            lines->erase(it);
        } else {
            ++it;
        }
    }

    return lines;
}

bool WorldEditor::loadWorld(const QString &codeline) {
    auto it_equal_sign{ std::find(codeline.cbegin(), codeline.cend(), '=') };
    if(it_equal_sign == codeline.cend()) {
        return false;
    }

    auto var_name{ createQString(codeline.cbegin(), it_equal_sign) };
    auto file_name{ createQString(std::next(it_equal_sign), codeline.cend()) };

    ////// открыть filename
    ////// загрузить filename
    /// // записать в loadedWorlds

    return true;
}

QString WorldEditor::createQString(QString::ConstIterator begin, QString::ConstIterator end) const {
    QString result{};
    while(begin != end) {
        result += *begin++;
    }

    return result;
}
