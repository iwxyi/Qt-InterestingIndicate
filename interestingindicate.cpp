#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QDebug>
#include "interestingindicate.h"

InterestingIndicate::InterestingIndicate(QWidget *parent) : QWidget(parent)
{
    setTriangleSize(16);
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

void InterestingIndicate::setItemSpacing(int spacing)
{
    this->itemSpacing = spacing;
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
    const bool down = currentY > y;
    const int distance = qAbs(currentY - y);
    if (distance <= 0) // 不需要移动
    {
        // TODO: 来一个有意思的动画？
        return ;
    }
    this->currentY = y;

    const int baseTime = 150 + 350 * distance / height();
    const int deltaTime = 100;

    // 开启随机动画
    aniPV = 3;
    QPropertyAnimation* ani = new QPropertyAnimation(this, "line1");
    ani->setStartValue(line1);
    ani->setEndValue(getLineTopByCenterY(y, TL_Left));
    ani->setDuration(baseTime);
    ani->setEasingCurve(QEasingCurve::InOutCubic);
    ani->start();
    connect(ani, &QPropertyAnimation::valueChanged, this, [=]{
        update();
    });
    connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
    connect(ani, &QPropertyAnimation::finished, this, [=]{
        aniPV--;
        update();
    });


    ani = new QPropertyAnimation(this, "line2");
    ani->setStartValue(line2);
    ani->setEndValue(getLineTopByCenterY(y, TL_TopRight));
    ani->setDuration(baseTime + deltaTime * (down ? 1 : 2));
    ani->setEasingCurve(QEasingCurve::InOutCubic);
    ani->start();
    connect(ani, &QPropertyAnimation::valueChanged, this, [=]{
        update();
    });
    connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
    connect(ani, &QPropertyAnimation::finished, this, [=]{
        aniPV--;
        update();
    });

    ani = new QPropertyAnimation(this, "line3");
    ani->setStartValue(line3);
    ani->setEndValue(getLineTopByCenterY(y, TL_BottomRight));
    ani->setDuration(baseTime + deltaTime * (down ? 2 : 1));
    ani->setEasingCurve(QEasingCurve::InOutCubic);
    ani->start();
    connect(ani, &QPropertyAnimation::valueChanged, this, [=]{
        update();
    });
    connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
    connect(ani, &QPropertyAnimation::finished, this, [=]{
        aniPV--;
        update();
    });
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
    double halfLine = 0.5;

    if (!aniPV) // 线两端重合的点不好看，还是一口气画完吧！
    {
        QPainterPath path;
        path.moveTo(border, line1);
        path.lineTo(border, line1 + triangleSize);
        path.lineTo(width()-border, line3);
        path.lineTo(border, line2);
        painter.drawPath(path);
    }
    else // 运动中，挨个画每一条线
    {
        painter.drawLine(QPointF(border, line1+halfLine), QPointF(border, line1 + triangleSize-halfLine));
        painter.drawLine(QPointF(border, line2), QPointF(width()-border-halfLine, line2 + triangleSize/2-halfLine));
        painter.drawLine(QPointF(width()-border-halfLine, line3+halfLine), QPointF(border, line3 + triangleSize/2));
    }
}

int InterestingIndicate::getCenterYByIndex(int index)
{
    if (totalCount == 0)
        return 0;
    double everyHeight = static_cast<double>(height() - itemSpacing * (totalCount - 1)) / totalCount;
    if (everyHeight <= 0) // 数据不正常
        return 0;

    return static_cast<int>((everyHeight + itemSpacing) * index + everyHeight / 2);
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
