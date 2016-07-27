#include "buffers.h"
/***************************************************************************
 *                                                                         *
 *   This code is distribuited under LGPL v2.1                             *
 *                                                                         *
 *   QAudioLib by Andrea993                                                *
 *                                                                         *
 ***************************************************************************/

/*
******QAudioLibBuffer******
*/
const QAudioLibBuffer::DefaultAudioFormat QAudioLibBuffer::DEFAULT_FORMAT;

QAudioLibBuffer QAudioLibBuffer::operator<<(const QAudioLibBuffer &x) const
{
    QAudioLibBuffer y(this->frameCount()+x.frameCount());

    memcpy(y.data(),this->constData(),this->byteCount());
    memcpy(y.data()+this->frameCount(),x.constData(),x.byteCount());

    return y;
}

 QAudioLibBuffer QAudioLibBuffer::operator+(const QAudioLibBuffer &x) const
 {
     const QAudioLibBuffer* shortb=(this->frameCount()<=x.frameCount()) ? this : &x;
     const QAudioLibBuffer* longb=(this->frameCount()>=x.frameCount()) ? &x : this;

     QAudioLibBuffer y(longb->frameCount());

     for(int i=0;i<shortb->frameCount();i++)
     {
         y.data()[i].left=shortb->constData()[i].left+longb->constData()[i].left;
         y.data()[i].right=shortb->constData()[i].right+longb->constData()[i].right;
     }

     memcpy(y.data()+shortb->byteCount(),longb->constData(),longb->byteCount()-shortb->byteCount());

     return y;
 }

 QAudioLibBuffer QAudioLibBuffer::operator*(qreal x) const
 {
     QAudioLibBuffer y(this->frameCount());

     for (int i=0;i<y.frameCount();i++)
     {
         y.data()[i].left=this->constData()[i].left*x;
         y.data()[i].right=this->constData()[i].right*x;
     }
     return y;
 }

 QAudioLibBuffer QAudioLibBuffer::operator*(const QAudioLibBuffer &x) const
 {
     const QAudioLibBuffer* shortb=(this->frameCount()<=x.frameCount()) ? this : &x;
     const QAudioLibBuffer* longb=(this->frameCount()>=x.frameCount()) ? &x : this;

     QAudioLibBuffer y(longb->frameCount());

     for(int i=0;i<shortb->frameCount();i++)
     {
         y.data()[i].left=shortb->constData()[i].left*longb->constData()[i].left;
         y.data()[i].right=shortb->constData()[i].right*longb->constData()[i].right;
     }

     memcpy(y.data()+shortb->byteCount(),longb->constData(),longb->byteCount()-shortb->byteCount());

     return y;
 }

  QAudioLibBuffer QAudioLibBuffer::subBuffer(int k0, int k1) const
  {
      QAudioLibBuffer y(qAbs(k0-k1)+1);
      int k=qMin(k0,k1);
      for (int i=0;i<y.frameCount();i++)
      {
          y.data()[i].left=this->constData()[i+k].left;
          y.data()[i].right=this->constData()[i+k].right;
      }

      return y;
  }


 /*
 ******QAudioLibFreqBuffer******
 */
 QAudioLibFreqBuffer QAudioLibFreqBuffer::newByRealBuffer(const QAudioLibBuffer & x)
 {
     QAudioLibFreqBuffer y(x.frameCount());
     for(int i=0;i<x.frameCount();i++)
     {
         y.data()[i].left=qcomplex(x.constData()[i].left);
         y.data()[i].right=qcomplex(x.constData()[i].right);
     }

     return y;
 }

 QAudioLibFreqBuffer QAudioLibFreqBuffer::operator*(const QAudioLibFreqBuffer &x)
 {
     const QAudioLibFreqBuffer* shortb=(this->frameCount()<=x.frameCount()) ? this : &x;
     const QAudioLibFreqBuffer* longb=(this->frameCount()>=x.frameCount()) ? &x : this;

     QAudioLibFreqBuffer y(longb->frameCount());

     for(int i=0;i<shortb->frameCount();i++)
     {
         y.data()[i].left=shortb->constData()[i].left*longb->constData()[i].left;
         y.data()[i].right=shortb->constData()[i].right*longb->constData()[i].right;
     }

     memcpy(y.data()+shortb->byteCount(),longb->constData(),longb->byteCount()-shortb->byteCount());

     return y;
 }

 void QAudioLibFreqBuffer::operator=(const QAudioLibFreqBuffer &x)
 {
     _zdata.resize(x.frameCount());
     memcpy(this->data(),x.constData(),x.byteCount());

 }

 QAudioLibFreqBuffer QAudioLibFreqBuffer::operator*(const qreal x)
 {
     QAudioLibFreqBuffer y(this->frameCount());

     for (int i=0;i<y.frameCount();i++)
     {
         y.data()[i].left=this->constData()[i].left*x;
         y.data()[i].right=this->constData()[i].right*x;
     }

     return y;
 }


 QAudioLibFreqBuffer QAudioLibFreqBuffer::mod() const
 {
     QAudioLibFreqBuffer y(this->frameCount());
     for(int i=0;i<y.frameCount();i++)
     {
         y.data()[i].left=abs(this->constData()[i].left);
         y.data()[i].right=abs(this->constData()[i].right);
     }

     return y;
 }

 QAudioLibFreqBuffer QAudioLibFreqBuffer::phase() const
 {
     QAudioLibFreqBuffer y(this->frameCount());
     for(int i=0;i<y.frameCount();i++)
     {
         y.data()[i].left=atan2(this->constData()[i].left.imag(),this->constData()[i].left.real());
         y.data()[i].right=atan2(this->constData()[i].right.imag(),this->constData()[i].right.real());
     }

     return y;
 }

QAudioLibFreqBuffer QAudioLibFreqBuffer::conj() const
{
     QAudioLibFreqBuffer y(this->frameCount());
     for(int i=0;i<y.frameCount();i++)
     {
         y.data()[i].left=std::conj(this->constData()[i].left);
         y.data()[i].right=std::conj(this->constData()[i].right);
     }

     return y;
}
