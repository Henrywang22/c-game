#include <QApplication>
#include <QFont>
#include "GameWindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    // 加这两行解决中文乱码
    a.setFont(QFont("Microsoft YaHei", 12));
    QFont::insertSubstitution("Arial", "Microsoft YaHei");

    GameWindow w;
    w.show();
    return a.exec();
}