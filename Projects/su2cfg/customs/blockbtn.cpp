#include <QPainter>
#include <QDebug>
#include <QStyleOptionButton>
#include <QStylePainter>
#include "blockbtn.h"

static const QChar ArrowSymbolsTab[BlockBtn::MaxDirection] = {

   QChar(BLOCKBTN_ARROW_LEFT_UNICODE),
   QChar(BLOCKBTN_ARROW_UP_UNICODE),
   QChar(BLOCKBTN_ARROW_RIGHT_UNICODE),
   QChar(BLOCKBTN_ARROW_DOWN_UNICODE)
};

BlockBtn::BlockBtn(QWidget *parent)
    : QPushButton(parent)
{
    defineOrientation();
    showDir = false;
    direction = MaxDirection;
}

BlockBtn::BlockBtn(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    defineOrientation();
    showDir = false;
    direction = MaxDirection;
}

BlockBtn::BlockBtn(const QIcon &icon, const QString &text, QWidget *parent)
    : QPushButton(icon, text, parent)
{
    defineOrientation();
    showDir = false;
    direction = MaxDirection;
}

void BlockBtn::setOrientation(const Orientation &orientation)
{
    if(this->orientation != orientation){
        QSize   size = this->size();
        QPoint  pos  = this->pos();
        QString text = this->text();
        int     coordCorrection = 0;

        this->orientation = orientation;

        coordCorrection = (size.width() - size.height())/2;

        this->setGeometry(pos.x() + coordCorrection, pos.y() - coordCorrection, size.height(), size.width());
        rotateDirection();
        //showDirection();
        this->repaint();

    }
}

void BlockBtn::setDirection(const Direction &direction)
{
    this->direction = direction;
    showDirection();
}

void BlockBtn::showDirection()
{
    if((showDir) && (direction < MaxDirection))
    {
        QString newText;

        newText += ArrowSymbolsTab[direction];

        foreach(QChar i, this->text()){
            if((i != ArrowSymbolsTab[Left])
            && (i != ArrowSymbolsTab[Up])
            && (i != ArrowSymbolsTab[Right])
            && (i != ArrowSymbolsTab[Down]))
                newText += i;
        }
        this->setText(newText);
    }
    updateTextOrientation();
}

void BlockBtn::setDirectionEnable(bool state)
{
    showDir = state;
}

void BlockBtn::toggleDirection()
{
    if(direction < MaxDirection){
        switch(direction){
        case Left: direction = Right;
            break;
        case Right: direction = Left;
            break;
        case Up: direction = Down;
            break;
        case Down: direction = Up;
            break;
        default:;
        }
        showDirection();
    }
}

void BlockBtn::rotateDirection(){
    if(direction < MaxDirection){
        switch(direction){
        case Left: direction = Up;
            break;
        case Right: direction = Down;
            break;
        case Up: direction = Right;
            break;
        case Down: direction = Left;
            break;
        default:;
        }
        showDirection();
    }
}

void BlockBtn::defineOrientation()
{
        if(this->size().height() > this->size().width())
           orientation = Vertical;
        else
           orientation = Horizontal;
}

void BlockBtn::updateTextOrientation()
{
    QString newText;

    switch(orientation){
        case Vertical:
            foreach(QChar i, text()){
                if((i != '\r') && (i != '\n')) {
                    newText += i;
                    newText += "\r\n";
                }
            }
            break;
        case Horizontal:
            foreach(QChar i, text()){
                if((i != '\r') && (i != '\n'))
                    newText += i;
            }
            break;
    default:;
    }

    this->setText(newText);
}
