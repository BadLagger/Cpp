#include "customdoublevalidator.h"

CustomDoubleValidator::CustomDoubleValidator(QObject *parent) : QDoubleValidator (parent)
{

}

CustomDoubleValidator::CustomDoubleValidator(double bottom, double top, int decimal, QObject *parent) : QDoubleValidator (bottom, top, decimal, parent)
{

}

QValidator::State CustomDoubleValidator::validate(QString& s, int& pos) const
{
    bool    isNumber;
    //QString st = s.replace(".", ",");

    if(s.isEmpty() || (s.startsWith('-') && s.length() == 1 && bottom() < 0))
        return QValidator::Intermediate;

    QChar point = locale().decimalPoint();
    if(s.indexOf(point) != -1)
    {
        int lenDec = s.length() - s.indexOf(point) - 1;
        if(lenDec > decimals())
        {
            return QValidator::Invalid;
        }
    }


    double value = locale().toDouble(s, &isNumber);
    if(isNumber && bottom() <= value && value <= top())
        return QValidator::Acceptable;
    return QValidator::Invalid;
}
