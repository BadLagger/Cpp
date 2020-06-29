#ifndef CUSTOMDOUBLEVALIDATOR_H
#define CUSTOMDOUBLEVALIDATOR_H

#include <QDoubleValidator>

class CustomDoubleValidator : public QDoubleValidator
{
public:
    CustomDoubleValidator(QObject *parent = nullptr);
    CustomDoubleValidator(double bottom, double top, int decimal, QObject *parent = nullptr);
    virtual QValidator::State validate(QString& s, int& pos) const override;
};

#endif // CUSTOMDOUBLEVALIDATOR_H
