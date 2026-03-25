#include "nammgr.h"

NamManager::NamManager() {

}

NamManager *NamManager::instance() {
    static NamManager inst;
    return &inst;
}

QNetworkAccessManager *NamManager::nam() {
    return &this->m_nam;
}
