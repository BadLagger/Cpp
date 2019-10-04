#ifndef BLOCKBTN_H
#define BLOCKBTN_H

#include <QPushButton>

#define BLOCKBTN_ARROW_VARIANT_1

#ifdef BLOCKBTN_ARROW_VARIANT_1
#define BLOCKBTN_ARROW_LEFT_UNICODE  (8592)
#define BLOCKBTN_ARROW_UP_UNICODE    (8593)
#define BLOCKBTN_ARROW_RIGHT_UNICODE (8594)
#define BLOCKBTN_ARROW_DOWN_UNICODE  (8595)
#else
#define BLOCKBTN_ARROW_LEFT_UNICODE  (8678)
#define BLOCKBTN_ARROW_UP_UNICODE    (8679)
#define BLOCKBTN_ARROW_RIGHT_UNICODE (8680)
#define BLOCKBTN_ARROW_DOWN_UNICODE  (8681)
#endif

/*typedef enum
{
    BtnBlockOrientation_Horizontal = 0,
    BtnBlockOrientation_Vertical,
}BtnBlockOrientations;

typedef enum
{
    BLockBtn
}BtnBlockDirections;*/

class BlockBtn : public QPushButton
{
    Q_OBJECT

   public:

   enum Orientation {
        Horizontal = 0,
        Vertical,
        MaxOrientation
   };

   enum Direction {
       Left = 0,
       Up,
       Right,
       Down,
       MaxDirection
   };
       BlockBtn(QWidget * parent = nullptr);
       BlockBtn(const QString & text, QWidget *parent = nullptr);
       BlockBtn(const QIcon & icon, const QString & text, QWidget *parent = nullptr);

       void setOrientation(const Orientation &orientation);
       void setDirection(const Direction &direction);
       void toggleDirection();
       void rotateDirection();
       void setDirectionEnable(bool state);

   private:
       Orientation orientation;
       Direction   direction;
       bool        showDir;

       void defineOrientation();
       void showDirection();
       void updateTextOrientation();
};

#endif // BLOCKBTN_H
