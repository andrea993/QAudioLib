/***************************************************************************
 *                                                                         *
 *   This code is distribuited under LGPL v2.1                             *
 *                                                                         *
 *   QAudioLib by Andrea993                                                *
 *                                                                         *
 ***************************************************************************/

#ifndef STOPOBJECT_H
#define STOPOBJECT_H

#include <QObject>
#include <QAudio>
class StopObject : public QObject
{
     Q_OBJECT
public:
    StopObject(QObject *parent) : QObject(parent) {}

public slots:
    void end(QAudio::State s)
    {
        if (s==QAudio::IdleState)
            exit(0);
    }

};

#endif // STOPOBJECT_H
