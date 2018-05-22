#ifndef SNOWBOYWATCHER_H
#define SNOWBOYWATCHER_H

#include <QObject>
#include <QString>
#include<QFileSystemWatcher>

class SnowboyWatcher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool snowboyRecog READ snowboyRecog WRITE setSnowboyRecog NOTIFY snowboyRecogChanged)
    Q_PROPERTY(string fileToWatch READ fileToWatch WRITE setFileToWatch NOTIFY fileToWatchChanged)

public:
    explicit SnowboyWatcher(QObject *parent = nullptr);

    bool snowboyRecog();
    QString fileToWatch();

    bool watchFile();
    QFileSystemWatcher snowboyHotwordWatch;
    void setSnowboyRecog(bool snowboyRecog);
    void setFileToWatch();

signals:
    void fileToWatchChanged();
    void snowboyRecogChanged();

public slots:
    void showModified(const QString& str){
        Q_UNUSED(str)
        setSnowboyRecog(true);
    }


private:
    bool m_snowboyRecog;
    QString m_fileToWatch;
};

#endif // SNOWBOYWATCHER_H
