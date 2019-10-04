#ifndef CBYTEARRAYTEXTEDIT_H
#define CBYTEARRAYTEXTEDIT_H

#include <QByteArray>
#include <QTextEdit>

class CByteArrayTextEdit : public QByteArray
{
    bool      HexPresintation;
    QTextEdit *tEdit;
public:
    CByteArrayTextEdit(QTextEdit *tEdit);
    void print(QByteArray bArray);
    void setHexView(bool state);
};

#endif // CBYTEARRAYTEXTEDIT_H
