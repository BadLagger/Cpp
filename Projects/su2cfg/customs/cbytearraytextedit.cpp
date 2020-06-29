#include "cbytearraytextedit.h"

CByteArrayTextEdit::CByteArrayTextEdit(QTextEdit *tEdit) : QByteArray ()
{
    HexPresintation = true;
    this->tEdit = tEdit;
    tEdit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

void CByteArrayTextEdit::print(QByteArray bArray)
{
   this->append(bArray);
   QString    printStr = "";
   if(HexPresintation)
   {
       for(int i = 0; i < bArray.size(); i++)
       {
           printStr += QString("%1 ").arg(int(bArray.at(i)), 0, 16).rightJustified(3, '0').right(3).toUpper();
       }
   } else {
       printStr += QString::fromUtf8(bArray);
   }
   tEdit->insertPlainText(printStr);
   tEdit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

void CByteArrayTextEdit::setHexView(bool state)
{
    HexPresintation = state;
}
