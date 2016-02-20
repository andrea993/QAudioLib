#include "complex.h"

Complex:: Complex()
{
    _re=_im=0;
}

Complex::Complex(double re, double im)
{
    _re=re;
    _im=im;
}

void Complex::setRe(double re)
{
    _re=re;
}

void Complex::setIm(double im)
{
    _im=im;
}

double Complex::re() const
{
    return _re;
}
double Complex::im() const
{
    return _im;
}
double Complex::mod() const
{
    return sqrt(_re*_re+_im*_im);
}
double Complex::arg() const
{
    return atan2(_im,_re);
}
Complex Complex::abs() const
{
    Complex y(qAbs(_re),qAbs(_im));
    return y;
}
void Complex::operator= (Complex c)
{
    setRe(c.re());
    setIm(c.im());
}

bool Complex::operator== (Complex c) const
{
    if(c.re()==re() && c.im()==im())
        return true;

    return false;
}

bool Complex::operator!= (Complex c) const
{
    return !(this->operator==(c));
}

Complex Complex::operator* (Complex c) const
{
    Complex y(re()*c.re()-im()*c.im(),
              re()*c.im()+im()*c.re());
    return y;
}

Complex Complex::operator/ (Complex c) const
{
    Complex y((re()*c.re()+im()*c.im())/(c.re()*c.re()+c.im()*c.im()),
              (im()*c.re()-re()*c.im())/(c.re()*c.re()+c.im()*c.im()));
    return y;
}

Complex Complex::operator+ (Complex c) const
{
    Complex y(re()+c.re(),im()+c.im());
    return y;
}

Complex Complex::operator- (Complex c) const
{
    Complex y(re()-c.re(),im()-c.im());
    return y;
}

