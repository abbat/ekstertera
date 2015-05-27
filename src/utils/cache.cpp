#include "cache.h"
//----------------------------------------------------------------------------------------------
static EteraMemoryCache* g_memory_cache = NULL;
//----------------------------------------------------------------------------------------------

void EteraMemoryCache::init()
{
    g_memory_cache = new EteraMemoryCache();
}
//----------------------------------------------------------------------------------------------

void EteraMemoryCache::cleanup()
{
    delete g_memory_cache;
}
//----------------------------------------------------------------------------------------------

EteraMemoryCache* EteraMemoryCache::instance()
{
    return g_memory_cache;
}
//----------------------------------------------------------------------------------------------

EteraMemoryCache::EteraMemoryCache():
    m_lock(QMutex::Recursive)
{
}
//----------------------------------------------------------------------------------------------

EteraMemoryCache::~EteraMemoryCache()
{
    clear();
}
//----------------------------------------------------------------------------------------------

void EteraMemoryCache::clear()
{
    m_lock.lock();

    EteraCacheHash::const_iterator i = m_cache.constBegin();
    while (i != m_cache.constEnd()) {
        EteraCacheItem* item = i.value();
        delete item;
        ++i;
    }

    m_lock.unlock();
}
//----------------------------------------------------------------------------------------------

QIODevice* EteraMemoryCache::prepare(const QUrl& url)
{
    m_lock.lock();

    EteraCacheItem* item = m_cache.value(url, NULL);
    if (item != NULL) {
        m_lock.unlock();
        return NULL;
    }

    item = new EteraCacheItem();

    item->Valid = false;

    m_cache.insert(url, item);

    m_lock.unlock();

    return &item->Device;
}
//----------------------------------------------------------------------------------------------

void EteraMemoryCache::remove(const QUrl& url)
{
    m_lock.lock();

    EteraCacheItem* item = m_cache.value(url, NULL);
    if (item == NULL) {
        m_lock.unlock();
        return;
    }

    delete item;

    m_cache.remove(url);

    m_lock.unlock();
}
//----------------------------------------------------------------------------------------------

void EteraMemoryCache::insert(const QUrl& url)
{
    m_lock.lock();

    EteraCacheItem* item = m_cache.value(url, NULL);
    if (item == NULL) {
        m_lock.unlock();
        return;
    }

    item->Valid = true;

    m_lock.unlock();
}
//----------------------------------------------------------------------------------------------
