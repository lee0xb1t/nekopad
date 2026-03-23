#include "favitemmodel.h"

FavItemModel::FavItemModel(QObject *parent)
    : QObject{parent}
{}

QString FavItemModel::favId() const
{
    return m_favId;
}

void FavItemModel::setFavId(const QString &newFavId)
{
    m_favId = newFavId;
}

QString FavItemModel::uid() const
{
    return m_uid;
}

void FavItemModel::setUid(const QString &newUid)
{
    m_uid = newUid;
}

QString FavItemModel::title() const
{
    return m_title;
}

void FavItemModel::setTitle(const QString &newTitle)
{
    m_title = newTitle;
}

QString FavItemModel::cover() const
{
    return m_cover;
}

void FavItemModel::setCover(const QString &newCover)
{
    m_cover = newCover;
}

qint64 FavItemModel::ctime() const
{
    return m_ctime;
}

void FavItemModel::setCtime(qint64 newCtime)
{
    m_ctime = newCtime;
}
