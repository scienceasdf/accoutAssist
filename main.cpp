#include "acountassit.h"
#include "connection.h"

#include <QApplication>
#include <QGuiApplication>

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::WA_WState_AcceptedTouchBeginEvent);
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_SynthesizeMouseForUnhandledTouchEvents);
    a.setAttribute(Qt::AA_SynthesizeTouchForUnhandledMouseEvents);
    QString qss;
        QFile qssFile(":/skin.qss");
        qssFile.open(QFile::ReadOnly);

        if(qssFile.isOpen())
        {
            qss = QLatin1String(qssFile.readAll());
            qApp->setStyleSheet(qss);
            qssFile.close();
        }
    if(!createConnection()) return 1;

    acountAssit w;
    w.show();





    //db1.close();

    return a.exec();
}
