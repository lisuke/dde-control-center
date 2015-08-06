#ifndef DISKITEM_H
#define DISKITEM_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QDebug>
#include "dbus/dbusdiskmount.h"
#include "diskicon.h"
#include "umountbutton.h"

class DiskItem : public QLabel
{
    Q_OBJECT
public:
    explicit DiskItem(const QString &id,DBusDiskMount * diskMount , QWidget *parent = 0);

signals:

public slots:
    void updateData();

private:
    void initWidgets();
    QString bitToHuman(qint64 value);
private:
    DBusDiskMount * m_diskMount = NULL;
    QString m_id = "";
    QString m_diskUuid = "";
    QLabel * m_titleLabel = NULL;
    DiskIcon * m_diskIcon = NULL;
    QLabel * m_progressLabel = NULL;
    QProgressBar * m_usedBar = NULL;
};

#endif // DISKITEM_H
