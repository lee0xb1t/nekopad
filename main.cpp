// #include "appmgr.h"
#include "loginmgr.h"
#include "usermgr.h"
#include "favmgr.h"
#include "requesttag.h"
#include "nammgr.h"

#include <vlc/vlc.h>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIconEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setWindowIcon(QIcon("qrc:/resources/logo_32x32.ico"));

    qmlRegisterSingletonInstance("netutils", 1, 0, "NamMgr", &NamManager::instance());

    qRegisterMetaType<RequestTag>("RequestTag");

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
    engine.loadFromModule("nekopad", "Main");

    int result = QCoreApplication::exec();

    return result;
}
