#ifndef COMPLEX_H
#define COMPLEX_H
#include <qmath.h>

class Complex
{
public:
    Complex();
    Complex(double re, double im);
    void setRe(double re);
    void setIm(double im);
    double re() const;
    double im() const;
    double mod() const;
    double arg() const;
    Complex abs() const;
    void operator= (Complex c);
    bool operator== (Complex c) const;
    bool operator!= (Complex c) const;
    Complex operator* (Complex c) const;
    Complex operator/ (Complex c) const;
    Complex operator+ (Complex c) const;
    Complex operator- (Complex c) const;

private:
    double _re;
    double _im;
};

#endif // COMPLEX_H
