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
    static void bitreverse(QAudioLibFreqBuffer & x);
    static int distanceFromPowerOf2(int n);
    static inline void swap(qcomplex & a, qcomplex & b)
    {
        qcomplex tmp=a;
        a=b;
        b=tmp;
    }

};


#endif // MATHEMATICS_H
