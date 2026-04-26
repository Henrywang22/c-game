#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QDebug>
#include "GameWindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QString path = "C:\\Users\\Lenovo\\Desktop\\cpp\\c-game\\c-game\\fonts\\msyh.ttc";
    int fontId = QFontDatabase::addApplicationFont(path);
    qDebug() << "fontId:" << fontId;  // 如果是-1说明路径错了或文件格式不支持

    if (fontId != -1) {
        QStringList families = QFontDatabase::applicationFontFamilies(fontId);
        qDebug() << "families:" << families;
        if (!families.isEmpty()) {
            a.setFont(QFont(families.first(), 12));
            QFont::insertSubstitution("Microsoft YaHei", families.first());
            QFont::insertSubstitution("Arial", families.first());
        }
    }

    GameWindow w;
    w.show();
    return a.exec();
}