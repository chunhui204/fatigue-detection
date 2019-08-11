
#include "mainwindow.h"
#include <QApplication>

//using namespace std;

//string func(float a, string m)
//{
//    string s = std::to_string(a);
//    string::size_type p = s.find(".");
//    return m+": "+s.substr(0, p+3);
//}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

