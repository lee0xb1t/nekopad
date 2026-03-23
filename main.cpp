#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
// #include "appmgr.h"
#include "loginmgr.h"
#include "usermgr.h"
#include "favmgr.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // AppManager appm(&app);
    LoginManager login_mgr(&app);
    UserManager user_mgr(&app);
    FavManager fav_mgr(&app);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("loginMgr", &login_mgr);
    engine.rootContext()->setContextProperty("userMgr", &user_mgr);
    engine.rootContext()->setContextProperty("favMgr", &fav_mgr);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.addImportPath(":/");
    engine.loadFromModule("nicopad", "Main");

    return QCoreApplication::exec();
}
