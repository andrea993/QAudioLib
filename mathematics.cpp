/***************************************************************************
 *                                                                         *
 *   This code is distribuited under LGPL v2.1                             *
 *                                                                         *
 *   QAudioLib by Andrea993                                                *
 *                                                                         *
 ***************************************************************************/

#include "mathematics.h"
#include <assert.h>
#include <QtDebug>

QAudioLibBuffer Mathematics::conv(QAudioLibBuffer &u ,QAudioLibBuffer &v)
{
    QAudioLibBuffer y(u.frameCount()+v.frameCount());

    for(int k=0;k<y.frameCount();k++)
    {
        for (int h=qMax(0,k-v.frameCount());h<=qMin(k,u.frameCount()-1);h++)
        {
            y.data()[k].left=y.constData()[k].left+u.constData()[h].left*v.constData()[k-h].left;
            y.data()[k].right=y.constData()[k].right+u.constData()[h].right*v.constData()[k-h].right;
        }
    }
    return y; 
}

QAudioLibBuffer Mathematics::corr(QAudioLibBuffer &u ,QAudioLibBuffer &v)
{
    QAudioLibBuffer y(u.frameCount()+v.frameCount());

    for(int k=0;k<y.frameCount();k++)
    {
        for (int h=0;h<=qMin(v.frameCount()-1,qMin(k,u.frameCount()-1));h++)
        {
            y.data()[k].left=y.constData()[k].left+u.constData()[h].left*v.constData()[k+h].left;
            y.data()[k].right=y.constData()[k].right+u.constData()[h].right*v.constData()[k+h].right;
        }
    }
    return y;
}

QAudioLibBuffer Mathematics::sinwave(qreal f, qreal sec)
{
    QAudioLibBuffer out(sec);
    double alpha=2*M_PI/(QAudioLibBuffer::getDefaultFormat().sampleRate()/f);
    double c=qCos(alpha);
    double s=qSin(alpha);

    int smpPeriod=QAudioLibBuffer::getDefaultFormat().sampleRate()/f;
    double x0=SHRT_MAX,y0=0,x1,y1;

    int end=qMin(smpPeriod, out.frameCount());
    for (int i=0;i<end;i++)
    {
        out.data()[i].right=out.data()[i].left=x0;
        //rotation matrix multiplication
        x1=c*x0+s*y0;
        y1=-s*x0+c*y0;
        x0=x1;
        y0=y1;
    }
    for (int i=smpPeriod-1; i<out.frameCount();i++)
        out.data()[i].right=out.data()[i].left=out.constData()[i%smpPeriod].left; //TO DO: Do not use %

    return out;
}


QAudioLibBuffer Mathematics::squarewave(qreal f, qreal sec)
{
    QAudioLibBuffer out(sec);
    int smpPeriod=QAudioLibBuffer::getDefaultFormat().sampleRate()/f;
    int smpHalfPeriod=smpPeriod/2;

    for (int i=0;i<out.frameCount();i++)
        out.data()[i].left=out.data()[i].right=(i%smpPeriod<smpHalfPeriod) ? SHRT_MAX : SHRT_MIN; //TO DO: Do not use %

    return out;

}

int Mathematics::distanceFromPowerOf2(int n)
{
    qreal nlfloat=log2(n);
    int nlint=nlfloat;
    if(nlint==nlfloat)
        return 0;

    return (2<<(nlint+1))-n;
}

void Mathematics::bitreverse(QAudioLibFreqBuffer & x)
{
    int ir,l;
    int n=x.frameCount();

    ir=0;
    for (int i=1;i<n;i++)
    {
        l=n>>1;
        while(ir+l > n-1)
           l=l>>1;

         ir=ir%l+l;
         if(ir>i)
         {
            swap(x.data()[i].left,x.data()[ir].left);
            swap(x.data()[i].right,x.data()[ir].right);
         }
    }
}

QAudioLibFreqBuffer Mathematics::dft(const QAudioLibFreqBuffer & x)
{
    int n=x.frameCount();
    QAudioLibFreqBuffer out(n);

    for (int i=0;i<n;i++)
    {
        out.data()[0].left+=x.constData()[i].left;
        out.data()[0].right+=x.constData()[i].right;
    }

    qcomplex e00=exp(qcomplex(0,-2*M_PI/n));
    qcomplex e,e0=e00;
    for (int k=1;k<n;k++)
    {
        out.data()[k].left+=x.constData()[0].left;
        out.data()[k].right+=x.constData()[0].right;
        e=e0;
        for (int i=1;i<n;i++)
        {
            out.data()[k].left+=x.constData()[i].left*e;
            out.data()[k].right+=x.constData()[i].right*e;
            e*=e0;
        }
        e0*=e00;
    }
    return out;
}

QAudioLibFreqBuffer Mathematics::fft(const QAudioLibFreqBuffer & x)
{
    assert(distanceFromPowerOf2(x.frameCount())==0); //fft works only with power of 2 length

    int n=x.frameCount();
    int nl=log2(n);

    QAudioLibFreqBuffer out(x);
    bitreverse(out);

    //complex lookup table
    qcomplex *etab = new qcomplex[nl];
    for (int i=0;i<nl;i++)
        etab[i]=exp(qcomplex(0,-M_PI/(2<<i)));

    //fft
    int nb=1; //n butterflies in each layer
    int istep,q;
    qcomplex w,wbm;
    for(int kl=0; kl<nl; kl++)
    {
        istep=nb<<1; //distance between two butterflies roots
        w=1;
        for(int kb=0;kb<nb;kb++) //butterfly index
        {
            for(int p=kb;p<n;p+=istep)
            {
                q=p+nb;
                //left
                wbm=w*out.constData()[q].left;  //current butterfly weight
                out.data()[q].left=out.constData()[p].left-wbm;
                out.data()[p].left+=wbm;
                //right
                wbm=w*out.constData()[q].right;  //current butterfly weight
                out.data()[q].right=out.constData()[p].right-wbm;
                out.data()[p].right+=wbm;
            } 
            w*=etab[kl];

        }
        nb=istep;
    }

    delete[] etab;
    return out;
}

QAudioLibFreqBuffer Mathematics::fft(const QAudioLibBuffer & x, bool zeropadding)
{
    //complexify
    QAudioLibFreqBuffer out=QAudioLibFreqBuffer::newByRealBuffer(x);
    if(zeropadding)
        out.appendZeros(distanceFromPowerOf2(x.frameCount()));

    return (distanceFromPowerOf2(out.frameCount())==0) ? fft(out) : dft(out);
}



QAudioLibBuffer Mathematics::ifft(const QAudioLibFreqBuffer & x)
{
    QAudioLibFreqBuffer fftBuffer=x.conj()*(1.0/x.frameCount());
    QAudioLibFreqBuffer c=(distanceFromPowerOf2(fftBuffer.frameCount())==0) ? fft(fftBuffer) : dft(fftBuffer);
    fftBuffer.clear();
    QAudioLibBuffer out(c.frameCount());
    for (int i=0;i<c.frameCount();i++)
    {
        out.data()[i].left=c.constData()[i].left.real();
        out.data()[i].right=c.constData()[i].right.real();
    }
    return out;
}

QAudioLibBuffer Mathematics::bandpass(QAudioLibBuffer &x, int N, int f0, int f1)
{
     QAudioLibBuffer y(x.frameCount());
     qreal T=1.0/QAudioLibBuffer::getDefaultFormat().sampleRate();
     QAudioBuffer::StereoFrame<qreal> y_k;
     qreal N_2=N/2.0;

     for (int k=0;k<y.frameCount();k++)
     {
         y_k.left=y_k.right=0;
         for(int h=qMax(0,k-x.frameCount());h<=qMin(k,N-1);h++)
         {
             qreal fir_h=2.0*(f1*sinc(2*f1*(h-N_2)*T)-f0*sinc(2*f0*(h-N_2)*T))*hanning(h-N_2,N)*T;
             y_k.left= y_k.left+fir_h*x.constData()[k-h].left;
             y_k.right=y_k.right+fir_h*x.constData()[k-h].right;
         }
         if (qAbs(y_k.left)>32767.0 || qAbs(y_k.right)>32767.0)
         {
             qDebug()<<"ERRORE "<<y_k.left<<" "<<y_k.right;
         }
         y.data()[k].left=static_cast<qint16>(y_k.left);
         y.data()[k].right=static_cast<qint16>(y_k.right);
     }

    return y;
}


QAudioLibBuffer Mathematics::filter(QAudioLibBuffer &x, int N, char type, int f0, int f1)
{
    assert(f0<QAudioLibBuffer::getDefaultFormat().sampleRate()/2);

    switch(type)
    {
        case 'l':
            return bandpass(x,N,0,f0);
        case 'h':
            return bandpass(x,N,f0,QAudioLibBuffer::getDefaultFormat().sampleRate()/2);
        case 'b':
            assert(f0<f1);
            return bandpass(x,N,f0,f1);
    }

    return QAudioLibBuffer(0);

}


