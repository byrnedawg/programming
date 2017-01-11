#include <iostream>
#include <QCoreApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    cout << "Hello World!!!";

    return a.exec();
}
