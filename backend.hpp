#ifndef BACKEND_HPP
#define BACKEND_HPP

#include <QObject>
#include <visa.h>

class backend : public QObject
{
    Q_OBJECT
    static constexpr unsigned long STABLIZE_TIME_NS = 1200000;
public:
    explicit backend(QObject *parent = nullptr);
    unsigned long keyseyhehyt_handle;
    unsigned long defaultRM;

    std::string visa_address_keyseyhehyt;
    bool keyseyhehytStatus();
    void fail_cleanup(QString message);
    void send(QString message);
    std::optional<QString> receive(QString message, const size_t max_length = 512);
signals:
};

#endif // BACKEND_HPP
