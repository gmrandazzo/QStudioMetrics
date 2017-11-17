#include <QSqlDatabase>

class QSMDataManager
{
public:
    QSMDataManager(const QString& path);
private:
    QSqlDatabase m_db;
};
