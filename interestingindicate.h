#ifndef INTERESTINGINDICATE_H
#define INTERESTINGINDICATE_H

#include <QObject>
#include <QWidget>

class InterestingIndicate : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int line1 READ getLine1 WRITE setLine1)
    Q_PROPERTY(int line2 READ getLine2 WRITE setLine2)
    Q_PROPERTY(int line3 READ getLine3 WRITE setLine3)
public:
    InterestingIndicate(QWidget *parent = nullptr);

    enum TriangleLIne
    {
        TL_Left,
        TL_TopRight,
        TL_BottomRight
    };

    /// 根据控件调整自己的位置
    void setTopAndBottom(QWidget* top, QWidget* bottom);

    /// 设置三角形边长的尺寸
    void setTriangleSize(int size);

    /// 设置绘制的颜色
    void setColor(QColor c);

    /// 设置总数量
    void setTotalCount(int count);

    /// 设置每两个项目之间的spacing
    void setItemSpacing(int spacing);

    /// 设置索引（不用动画）
    void setCurrentIndex(int index);

    /// 设置缩影（带动画的)
    void moveIndicate(int index);

    /// 当前指向的索引
    int currentIndicateIndex() const;

protected:
    void paintEvent(QPaintEvent *) override;

signals:

public slots:

private:
    /// 获取某一个index的中心点Y坐标
    int getCenterYByIndex(int index);
    /// 根据中间点，获取线绘制的Y
    int getLineTopByCenterY(int centerY, TriangleLIne line);
    /// 设置指示条的中心点Y
    void setCenterY(int y);

private:
    int getLine1();
    void setLine1(int x);
    int getLine2();
    void setLine2(int x);
    int getLine3();
    void setLine3(int x);

    int line1 = 0;
    int line2 = 0;
    int line3 = 0;

private:
    int triangleSize = 8;
    QColor lineColor = Qt::black;
    int totalCount = 0;
    int itemSpacing = 0;
    int currentIndex = -1;
    int currentY = -1;
};

#endif // INTERESTINGINDICATE_H
