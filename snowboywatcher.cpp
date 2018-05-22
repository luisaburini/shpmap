#include "snowboywatcher.h"

SnowboyWatcher::SnowboyWatcher(QObject *parent) :
    QObject(parent)
{

}

bool SnowboyWatcher::snowboyRecog(){
    snowboyHotwordWatch.addPath("/home/lburini/Documents/snowboy.txt");
    //snowboyHotwordWatch.addPath("/home/root/snowboy.txt");
    QObject::connect(&snowboyHotwordWatch, SIGNAL(fileChanged(QString)), this, SLOT(showModified(QString)));
    return m_snowboyRecog;
}

void SnowboyWatcher::setSnowboyRecog(bool snowboyRecog){
    if(snowboyRecog == m_snowboyRecog)
        return;
    m_snowboyRecog = snowboyRecog;
    emit snowboyRecogChanged();
}
