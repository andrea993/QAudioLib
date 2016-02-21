/***************************************************************************
 *                                                                         *
 *   This code is distribuited under LGPL v2.1                             *
 *                                                                         *
 *   QAudioLib by Andrea993                                                *
 *                                                                         *
 ***************************************************************************/

#include <QCoreApplication>
#include <QObject>
#include <iostream>
#include <QAudioOutput>
#include <QBuffer>
#include <QTime>
#include <QThread>
#include <QtDebug>

#include <qaudiolib.h>
#include "stopobject.h"

using namespace std;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTime timer;

    cout<<"Generate 1s square wave..."<<endl;
    timer.start();
    QAudioLibBuffer w=Mathematics::squarewave(500,1.0);
    cout<<"Done in "<<timer.elapsed()<<"ms"<<endl;
    cout<<endl;

    cout<<"Calculate fft..."<<endl;
    timer.start();
    QAudioLibFreqBuffer fft= Mathematics::fft(w);
    cout<<"Done in "<<timer.elapsed()<<"ms"<<endl;
    cout<<endl;

    cout<<"Calculate ifft..."<<endl;
    timer.start();
    Mathematics::ifft(fft);
    cout<<"Done in "<<timer.elapsed()<<"ms"<<endl;
    cout<<endl;

    cout<<"Generate 5000 samples sin wave..."<<endl;
    timer.start();
    w=Mathematics::sinwave(500,5000/44100.0);
    cout<<"Done in "<<timer.elapsed()<<"ms"<<endl;
    cout<<endl;

    cout<<"Calculate dft...(take a while)"<<endl;
    timer.start();
    Mathematics::fft(w,false);
    cout<<"Done in "<<timer.elapsed()<<"ms"<<endl;
    cout<<endl;

    cout<<"Generate 1s sin wave..."<<endl;
    timer.start();
    w=Mathematics::sinwave(500,1.0);
    cout<<"Done in "<<timer.elapsed()<<"ms"<<endl;
    cout<<endl;

    cout<<"Generate 1s white noise..."<<endl;
    timer.start();
    for(int i=0;i<w.frameCount();i++)
        w.data()[i].left=w.data()[i].right=qrand()%SHRT_MAX;
    int lengthw=w.byteCount();
    cout<<"Done in "<<timer.elapsed()<<"ms"<<endl;
    cout<<endl;

    cout<<"Lowpass 2000Hz filtering..."<<endl;
    timer.start();
    fft=Mathematics::fft(w);
    for(int i=2000/fft.deltaf();i<fft.frameCount();i++)
        fft.data()[i].left=fft.data()[i].right=qcomplex(0);
    w=Mathematics::ifft(fft);
    cout<<"Done in "<<timer.elapsed()<<"ms"<<endl;
    cout<<endl;

    cout<<"Play noise..."<<endl;
    QAudioFormat format=QAudioLibBuffer::getDefaultFormat();
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
        return 1;
    }
    QBuffer buffer(&a);
    buffer.setData(reinterpret_cast<const char*>(w.constData()),lengthw);
    buffer.open(QIODevice::ReadOnly);
    QAudioOutput* audio = new QAudioOutput(format, &a);
    audio->start(&buffer);

    StopObject *stop = new StopObject(&a);

    QObject::connect(audio,SIGNAL(stateChanged(QAudio::State)),stop,SLOT(end(QAudio::State)));



    return a.exec();
}
