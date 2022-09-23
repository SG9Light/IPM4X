#ifndef MODBUSSLAVE_H
#define MODBUSSLAVE_H

#include <QString>

class modbusslave
{

public:
    modbusslave();

    uint16_t crc16_modbus(uint8_t *_pBuf, uint16_t _usLen);
};

#endif // MODBUSSLAVE_H
