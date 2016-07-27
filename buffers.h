/***************************************************************************
 *                                                                         *
 *   This code is distribuited under LGPL v2.1                             *
 *                                                                         *
 *   QAudioLib by Andrea993                                                *
 *                                                                         *
 ***************************************************************************/

#ifndef BUFFERS_H
#define BUFFERS_H

#include <QAudioBuffer>
#include <QVector>
#include <complex>

typedef QAudioBuffer::S16S S16S;
typedef std::complex<double> qcomplex;

class QAudioLibBuffer : public QAudioBuffer
{
private:
    class DefaultAudioFormat : public QAudioFormat
    {
    public:
        DefaultAudioFormat()
        {
            setByteOrder(QAudioFormat::LittleEndian);
            setChannelCount(2);
            setCodec("audio/pcm");
            setSampleRate(44100);
            setSampleSize(16);
            setSampleType(QAudioFormat::SignedInt);
        }
    };

    static const DefaultAudioFormat DEFAULT_FORMAT;

public:
    QAudioLibBuffer() : QAudioBuffer(0,DEFAULT_FORMAT) {}
    QAudioLibBuffer(const QAudioBuffer & other) : QAudioBuffer(other) {}
    QAudioLibBuffer(const QByteArray & data) : QAudioBuffer(data,DEFAULT_FORMAT,-1) {}
    QAudioLibBuffer(int numFrames) : QAudioBuffer(numFrames,DEFAULT_FORMAT,-1)
    {
        memset(this->data(),0,this->byteCount());
    }
    QAudioLibBuffer(qreal sec) : QAudioBuffer(static_cast<int>(sec*DEFAULT_FORMAT.sampleRate()),DEFAULT_FORMAT,-1)
    {
        memset(this->data(),0,this->byteCount());
    }

    static const QAudioFormat & getDefaultFormat()
    {
        return DEFAULT_FORMAT;
    }

    const S16S* constData() const
    {
        return QAudioBuffer::constData<QAudioLibBuffer::S16S>();
    }

    S16S* data()
    {
        return QAudioBuffer::data<QAudioLibBuffer::S16S>();
    }

    QAudioLibBuffer operator<<(const QAudioLibBuffer &x) const;
    QAudioLibBuffer operator*(const QAudioLibBuffer &x) const;
    QAudioLibBuffer operator*(const qreal x) const;
    QAudioLibBuffer operator+(const QAudioLibBuffer &x) const;
    QAudioLibBuffer subBuffer(int k0, int k1) const; //return a subBuffer
};

class QAudioLibFreqBuffer
{
public:
    //This is a modified version of StereoFrame from qaudiobuffer.h that works with complex
   typedef struct ComplexStereoFrame {
        ComplexStereoFrame(): left(qcomplex(0)), right(qcomplex(0)) {}
        ComplexStereoFrame(qcomplex leftSample, qcomplex rightSample): left(leftSample), right(rightSample) {}

        ComplexStereoFrame& operator=(const ComplexStereoFrame &other)
        {
            left = other.left;
            right = other.right;
            return *this;
        }

        qcomplex left;
        qcomplex right;

        qcomplex average() const
        {
            return left/qcomplex(2)+right/qcomplex(2);
        }
        void clear() {left = right = qcomplex(0);}
    }S64CD;


private:
    QVector<S64CD> _zdata; //S32F manages left and right

public:
     QAudioLibFreqBuffer(int numFrames)
     {
         _zdata.resize(numFrames);
     }
     QAudioLibFreqBuffer(const QAudioLibFreqBuffer & other)
     {
         _zdata.resize(other.frameCount());
         memcpy(_zdata.data(),other.constData(),other.byteCount());
     }
    static QAudioLibFreqBuffer newByRealBuffer(const QAudioLibBuffer & x);

     S64CD* data()
     {
         return _zdata.data();
     }
     const S64CD* constData() const
     {
         return _zdata.constData();
     }
     int frameCount() const
     {
         return _zdata.size();
     }
     int byteCount() const
     {
         return _zdata.count()*sizeof(S64CD);
     }
     qreal deltaf() //Interval between frequncies
     {
         return QAudioLibBuffer::getDefaultFormat().sampleRate()/static_cast<qreal>(frameCount());
     }
     qreal nyquistfreq()
     {
         return QAudioLibBuffer::getDefaultFormat().sampleRate()/2.0;
     }
     void appendZeros(int n) //used by fft
     {
         _zdata.resize(_zdata.size()+n);
     }
     void clear()
     {
         _zdata.clear();
     }


     QAudioLibFreqBuffer operator*(const QAudioLibFreqBuffer &x);
     QAudioLibFreqBuffer operator*(const qreal x);
     void operator=(const QAudioLibFreqBuffer &x);

     QAudioLibFreqBuffer mod() const;
     QAudioLibFreqBuffer phase() const;
     QAudioLibFreqBuffer conj() const;

};


#endif // BUFFERS_H
