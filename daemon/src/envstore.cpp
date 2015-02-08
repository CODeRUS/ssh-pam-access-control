#include "envstore.h"

EnvStore::EnvStore(QObject *parent) :
    QObject(parent)
{
}

void EnvStore::fromConnection(const QVariantMap &connection)
{
    m_pamRUser = connection["pamRUser"].toString();
    Q_EMIT pamRUserChanged();

    m_pamRHost = connection["pamRHost"].toString();
    Q_EMIT pamRHostChanged();

    m_pamUser = connection["pamUser"].toString();
    Q_EMIT pamUserChanged();

    m_pamType = connection["pamType"].toString();
    Q_EMIT pamTypeChanged();

    m_pamService = connection["pamService"].toString();
    Q_EMIT pamServiceChanged();

    m_pamTty = connection["pamTty"].toString();
    Q_EMIT pamTtyChanged();

    m_service = connection["service"].toString();
    Q_EMIT serviceChanged();

}
