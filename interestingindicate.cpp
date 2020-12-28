#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include "interestingindicate.h"

InterestingIndicate::InterestingIndicate(QWidget *parent) : QWidget(parent)
{
    setTriangleSize(8);
    srand(static_cast<unsigned int>(time(nullptr)));
}

void InterestingIndicate::setTopAndBottom(QWidget *top, QWidget *bottom)
{
    setGeometry(top->pos().x() - triangleSize, top->geometry().top(), triangleSize,
         bottom->geometry().bottom() - top->geometry().top());
}

void InterestingIndicate::setTriangleSize(int size)
{
    this->triangleSize = size;
    this->setFixedWidth(static_cast<int>(size * 1.732 / 2 + 0.5));
}

void InterestingIndicate::setColor(QColor c)
{
    this->lineColor = c;
    update();
}

void InterestingIndicate::setTotalCount(int count)
{
    this->totalCount = count;
}

void InterestingIndicate::setCurrentIndex(int index)
{
    this->currentIndex = index;
    setCenterY(getCenterYByIndex(index));
    update();
}

void InterestingIndicate::moveIndicate(int index)
{
    this->currentIndex = index;
    int y = getCenterYByIndex(index);

    const int baseTime = 250;
    const int deltaTime = 100;

    // 开启随机动画
    QPropertyAnimation* ani = new QPropertyAnimation(this, "line1");
    ani->setStartValue(line1);
    ani->setEndValue(getLineTopByCenterY(y, TL_Left));
    ani->setDuration(baseTime + qrand() % deltaTime);
    ani->start();
    connect(ani, &QPropertyAnimation::valueChanged, this, [=]{
        update();
    });
    connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));

    ani = new QPropertyAnimation(this, "line2");
    ani->setStartValue(line2);
    ani->setEndValue(getLineTopByCenterY(y, TL_TopRight));
    ani->setDuration(baseTime + qrand() % deltaTime);
    ani->start();
    connect(ani, &QPropertyAnimation::valueChanged, this, [=]{
        update();
    });
    connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));

    ani = new QPropertyAnimation(this, "line3");
    ani->setStartValue(line3);
    ani->setEndValue(getLineTopByCenterY(y, TL_BottomRight));
    ani->setDuration(baseTime + qrand() % deltaTime);
    ani->start();
    connect(ani, &QPropertyAnimation::valueChanged, this, [=]{
        update();
    });
    connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
}

int InterestingIndicate::currentIndicateIndex() const
{
    return currentIndex;
}

void InterestingIndicate::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(lineColor, 1));
    const int border = 1;

    painter.drawLine(border, line1, border, line1 + triangleSize);
    painter.drawLine(border, line2, width()-border, line2 + triangleSize/2);
    painter.drawLine(width()-border, line3, border, line3 + triangleSize/2);
}

int InterestingIndicate::getCenterYByIndex(int index)
{
    int everyHeight = (height() - itemSpacing * (totalCount - 1)) / totalCount;
    if (everyHeight <= 0) // 数据不正常
        return 0;

    return everyHeight * index + itemSpacing * qMin(0, index-1) + everyHeight / 2;
}

int InterestingIndicate::getLineTopByCenterY(int centerY, TriangleLIne line)
{
    switch (line)
    {
    case TL_Left:
        return centerY - triangleSize/2;
    case TL_TopRight:
        return centerY - triangleSize/2;
    case TL_BottomRight:
        return centerY;
    }
}

void InterestingIndicate::setCenterY(int y)
{
    this->currentY = y;
    line1 = getLineTopByCenterY(y, TL_Left);
    line2 = getLineTopByCenterY(y, TL_TopRight);
    line3 = getLineTopByCenterY(y, TL_BottomRight);
    update();
}

int InterestingIndicate::getLine1()
{
    return line1;
}

void InterestingIndicate::setLine1(int x)
{
    this->line1 = x;
}

int InterestingIndicate::getLine2()
{
    return line2;
}

void InterestingIndicate::setLine2(int x)
{
    this->line2 = x;
}

int InterestingIndicate::getLine3()
{
    return line3;
}

void InterestingIndicate::setLine3(int x)
{
    this->line3 = x;
}
