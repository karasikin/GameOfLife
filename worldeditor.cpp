#include "worldeditor.h"

#include <fstream>

#include <QPlainTextEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QCloseEvent>

#include <QDebug>

WorldEditor::WorldMaskRect::WorldMaskRect(size_t x, size_t y, size_t row, size_t col)
    :x(x), y(y), row(row), col(col)
{}

WorldEditor::WorldMask::WorldMask(World *worldPtr)
    :worldPtr(worldPtr)
{
    if(!worldPtr) {
        rect = WorldMaskRect{};
    } else {
        rect = WorldMaskRect{0, 0, worldPtr->row(), worldPtr->col()};
    }
}

WorldEditor::WorldEditor(QWidget *parent)
    : QWidget(parent),
      _world(nullptr),
      _codeEdit(new QPlainTextEdit{}),
      _reportEdit(new QPlainTextEdit{}),
      _startBtn(new QPushButton{"&Start"}),
    _returnBtn(new QPushButton{"&Return"})

{
    auto font{ this->font() };
    font.setPointSize(14);
    this->setFont(font);

    _reportEdit->setReadOnly(true);

    _codeEdit->appendPlainText("load:");
    _codeEdit->appendPlainText("\tx=../worlds/gl.wrld");
    _codeEdit->appendPlainText("make:");
    _codeEdit->appendPlainText("return:");

    auto layout{new QGridLayout};
    layout->addWidget(_codeEdit, 0, 0, 6, 10);
    layout->addWidget(_reportEdit, 6, 0, 2, 9);
    layout->addWidget(_startBtn, 6, 9, 1, 1);
    layout->addWidget(_returnBtn, 7, 9, 1, 1);
    setLayout(layout);

    connect(_startBtn, &QPushButton::clicked, this, &WorldEditor::onStartBtnClicked);
    connect(_returnBtn, &QPushButton::clicked, this, &WorldEditor::onReturnBtnClicked);
}

void WorldEditor::onStartBtnClicked() {
    _worlds.clear();

    auto splittedText{ split(_codeEdit->toPlainText()) };

    auto it_load{ std::find(splittedText->cbegin(), splittedText->cend(), QString("load:")) };
    auto it_make{ std::find(splittedText->cbegin(), splittedText->cend(), QString("make:")) };
    auto it_return{ std::find(splittedText->cbegin(), splittedText->cend(), QString("return:")) };
    auto not_found{ splittedText->cend() };

    if(it_load == not_found || it_make == not_found || it_return == not_found) {
        _world = nullptr;
        _reportEdit->appendPlainText("Not Found: \"load:\" and \"make\" and \"return:\""); // Написать норамльно
        return;
    }

    for(auto it{ std::next(it_load) }; it != it_make && it != it_return && it != not_found; ++it) {
        if(!loadWorld(*it)) {
            _world = nullptr;  // Ошибка!!!!! Записать _errorReport
            _reportEdit->appendPlainText(QString("bad operator: %1").arg(*it));  // то же самое !!!!!!!!!111!!!!!!!
            return;
        }
    }

    // ... Make

    auto return_code{ std::next(it_return) };
    if(return_code == not_found) {
        _world = nullptr;
        _reportEdit->appendPlainText("Nothing return!!");
        return;
    }

    returnWorld(*return_code);
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

    auto it_world_in_cache{ _cache.find(file_name) };

    if(it_world_in_cache != _cache.end()) {
        qDebug() << QString("file %1 loaded from <Cache>.").arg(file_name);
        _worlds[var_name] = WorldMask{&it_world_in_cache->second};
    } else {
        std::ifstream in{ file_name.toStdString() };
        if(!in) {
            qDebug() << QString("file %1 not found").arg(file_name);
            return false;
        }
        qDebug() << QString("file %1 loaded from <File>.").arg(file_name);
        _cache[file_name] = World{};
        in >> _cache[file_name];
        _worlds[var_name] = WorldMask{&_cache[file_name]};
    }

    return true;
}

bool WorldEditor::returnWorld(const QString &codeLine) {
    auto returnable_world{ _worlds.find(codeLine) };
    if(returnable_world == _worlds.end()) {
        _world = nullptr;
        _reportEdit->appendPlainText("Returnable world not found");
        return false;
    }

    emit createdWorld(*returnable_world->second.worldPtr);
    return true;
}

QString WorldEditor::createQString(QString::ConstIterator begin, QString::ConstIterator end) const {
    QString result{};
    while(begin != end) {
        result += *begin++;
    }

    return result;
}
