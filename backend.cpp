#include "backend.hpp"
#include <qdebug.h>
#include <QThread>

backend::backend(QObject *parent)
    : QObject{parent}
    , defaultRM(VI_NULL)
    , keyseyhehyt_handle(VI_NULL)
    , visa_address_keyseyhehyt("USB0::10893::6146::MY61002081::0::INSTR")
{
    ViStatus status;
    status = viOpenDefaultRM(&defaultRM);

    qDebug() << status;
    switch(status)
    {
    case VI_SUCCESS:
        qDebug() << "Visa RM ok";
        break;
    case VI_ERROR_ALLOC:
        qDebug() << "Problem z managerem zasobów VISA32 (Insufficient system resources to create a session to the Default Resource Manager resource.)";
        break;
    case VI_ERROR_INV_SETUP:
        qDebug() << "Problem z managerem zasobów VISA32 (Some implementation-specific configuration file is corrupt or does not exist.)";
        break;
    case VI_ERROR_SYSTEM_ERROR:
        qDebug() << "Problem z managerem zasobów VISA32 (The VISA system failed to initialize.)";
        break;
    }

    ViFindList findList;            // List of found resources
    ViUInt32 numInstr;              // Number of instruments found
    ViChar instrDesc[VI_FIND_BUFLEN]; // Description buffer

    // Find all VISA resources
    status = viFindRsrc(defaultRM, (ViString)"?*", &findList, &numInstr, instrDesc);
    if (status < VI_SUCCESS) {
        qDebug() << "Failed to find VISA resources. Status: " << status;
        viClose(defaultRM);
    }

    qDebug() << "Number of VISA devices found: " << (unsigned long)numInstr;

    // First device found
    qDebug() << "Device: " << instrDesc;

    // Loop through remaining devices
    for (ViUInt32 i = 1; i < numInstr; ++i) {
        status = viFindNext(findList, instrDesc);
        if (status < VI_SUCCESS) {
            qDebug() << "Error retrieving next device. Status: " << status;
            break;
        }
        qDebug() << "Device: " << instrDesc;
    }

    // // Cleanup
    // viClose(findList);
    // viClose(defaultRM);

}

bool backend::keyseyhehytStatus()
{
    ViStatus status;
    status = viOpen(defaultRM, const_cast<ViRsrc>(visa_address_keyseyhehyt.c_str()), VI_NULL, VI_NULL, &keyseyhehyt_handle);
    if (status != VI_SUCCESS)
        return false;

    viClose(keyseyhehyt_handle);
    return true;

}

void backend::fail_cleanup(QString message)
{
    qDebug() << "Problem: " << message;
    viClose(keyseyhehyt_handle);
}
void backend::send(QString command)
{
    ViUInt32 retCount;
    ViStatus status;
    status = viOpen(defaultRM, const_cast<ViRsrc>(visa_address_keyseyhehyt.c_str()), VI_NULL, VI_NULL, &keyseyhehyt_handle);
    if (status != VI_SUCCESS)
    {
        fail_cleanup("Nie udalo sie nawiazac polaczniea z keythleyem");
        return;
    }
    viSetAttribute(keyseyhehyt_handle, VI_ATTR_TMO_VALUE, 1000);

    status = viWrite(keyseyhehyt_handle, reinterpret_cast<ViConstBuf>(command.toStdString().c_str()), command.size(), &retCount);
    if (status != VI_SUCCESS)
    {
        fail_cleanup("Nie udalo sie przeslac skryptu na keythleya");
        return;
    }

    QThread::sleep(std::chrono::nanoseconds(STABLIZE_TIME_NS));
    viClose(keyseyhehyt_handle);
}

std::optional<QString> backend::receive(QString command, const size_t max_length)
{
    ViUInt32 retCount;
    ViStatus status;
    status = viOpen(defaultRM, const_cast<ViRsrc>(visa_address_keyseyhehyt.c_str()), VI_NULL, VI_NULL, &keyseyhehyt_handle);
    if (status != VI_SUCCESS)
    {
        fail_cleanup("Nie udalo sie nawiazac polaczniea z keythleyem");
        return std::nullopt;
    }
    viSetAttribute(keyseyhehyt_handle, VI_ATTR_TMO_VALUE, 1000);

    status = viWrite(keyseyhehyt_handle, reinterpret_cast<ViConstBuf>(command.toStdString().c_str()), command.size(), &retCount);
    if (status != VI_SUCCESS)
    {
        fail_cleanup("Nie udalo sie przeslac skryptu na keythleya");
        return std::nullopt;
    }
    QThread::sleep(std::chrono::nanoseconds(STABLIZE_TIME_NS));

    char* buffer = new char[max_length + 1];
    viRead(keyseyhehyt_handle, (ViBuf)buffer, max_length + 1, &retCount);
    QThread::sleep(std::chrono::nanoseconds(STABLIZE_TIME_NS));

    viClose(keyseyhehyt_handle);
    QString result =  QString::fromUtf8(buffer);
    delete[] buffer;
    return result;
}


