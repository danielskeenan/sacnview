#ifndef SRC_DELETELATER_H
#define SRC_DELETELATER_H

#include <QObject>
#include <type_traits>

/**
 * Deleter for use with std smart pointers that calls deleteLater() on a QObject.
 */
template<class T, std::enable_if_t<std::is_base_of_v<QObject, T>, bool> = true>
struct DeleteLater
{
    void operator()(T * ptr) { ptr->deleteLater(); }
};

#endif //SRC_DELETELATER_H
