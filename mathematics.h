/***************************************************************************
 *                                                                         *
 *   This code is distribuited under LGPL v2.1                             *
 *                                                                         *
 *   QAudioLib by Andrea993                                                *
 *                                                                         *
 ***************************************************************************/

#ifndef MATHEMATICS_H
#define MATHEMATICS_H

#include "buffers.h"
#include <qmath.h>
#include <limits>
#include <QObject>

class Mathematics
{
public:
    explicit Mathematics(QObject *parent = 0);


    /******SOUND GENERATION******/

    //Generate some waves with freq f and duration sec
    static QAudioLibBuffer sinwave(qreal f, qreal sec);
    static QAudioLibBuffer squarewave(qreal f, qreal sec);


    /******SOUND MANIPULATION******/

    //Convolution of 2 signal
    static QAudioLibBuffer conv(QAudioLibBuffer &u ,QAudioLibBuffer &v);
    //Correlation of 2 signal
    static QAudioLibBuffer corr(QAudioLibBuffer &u ,QAudioLibBuffer &v);
    //FIR with hanning window (type can be 'l': lowpass, 'h' highpass, 'b' bandpass) set f1 only for bandpass filters
    static QAudioLibBuffer filter(QAudioLibBuffer &x, int N, char type, int f0, int f1=0);

    /******FREQUENCY DOMAIN******/

    //dft of a power of 2 length signal
    static QAudioLibFreqBuffer fft(const QAudioLibFreqBuffer & x);
    //dft of a generic signal
    static QAudioLibFreqBuffer dft(const QAudioLibFreqBuffer & x);
    //dft or fft of a QAudioLibFreqBuffer signal
    static QAudioLibFreqBuffer fft(const QAudioLibBuffer & x, bool zeropadding=true);
    //inverse dft or fft of a signal
    static QAudioLibBuffer ifft(const QAudioLibFreqBuffer & x);

private:
    static QAudioLibBuffer bandpass(QAudioLibBuffer &x, int N, int f0, int f1);
    static void bitreverse(QAudioLibFreqBuffer & x);
    static int distanceFromPowerOf2(int n);
    static inline void swap(qcomplex & a, qcomplex & b)
    {
        qcomplex tmp=a;
        a=b;
        b=tmp;
    }
    static inline double hanning(qreal t, int N)
    {
        return 0.5*(1+qCos(M_PI*t/(N-1.0)));
    }
    static inline double sinc(qreal t)
    {
        if(t==0)
            return 1;

        return qSin(M_PI*t)/M_PI/t;
    }

};


#endif // MATHEMATICS_H
