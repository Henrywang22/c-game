#include <QApplication>
#include <QFont>
#include "GameWindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    a.setFont(QFont("Microsoft YaHei", 12));
    QFont::insertSubstitution("Arial", "Microsoft YaHei");

    GameWindow w;
    w.show();
    return a.exec();
}