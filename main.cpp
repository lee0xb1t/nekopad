// #include "appmgr.h"
#include "loginmgr.h"
#include "usermgr.h"
#include "favmgr.h"
#include "requesttag.h"

#include <vlc/vlc.h>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

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



    // const char* args[] = {
    //     "--verbose=2",
    //     "--no-video",
    // };
    // libvlc_instance_t *inst = libvlc_new(sizeof(args) / sizeof(args[0]), args);

    // if (!inst) {
    //     std::cerr << "Failed to initialize LibVLC!" << std::endl;
    //     return -1;
    // }

    // std::cout << "LibVLC Version: " << libvlc_get_version() << std::endl;
    // std::cout << "LibVLC Changes: " << libvlc_get_changeset() << std::endl;

    // const char *test_url = "https://sf1-cdn-tos.huoshanstatic.com/obj/media-fe/xgplayer_doc_video/mp4/xgplayer-demo-720p.mp4";

    // std::cout << "Playing: " << test_url << std::endl;

    // libvlc_media_t *media = libvlc_media_new_location(inst, test_url);

    // // 创建播放器实例 (如果需要画面，需要使用 libvlc_media_player_new_from_media)
    // // 这里仅做简单播放测试
    // libvlc_media_player_t *mp = libvlc_media_player_new_from_media(media);

    // libvlc_media_release(media);

    // libvlc_media_player_play(mp);

    int result = QCoreApplication::exec();

    // libvlc_media_player_stop(mp);
    // libvlc_media_player_release(mp);
    // libvlc_release(inst);

    // std::cout << "LibVLC initialized and released successfully!" << std::endl;

    return result;
}
