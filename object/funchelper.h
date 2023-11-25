#ifndef FUNCHELPER_H
#define FUNCHELPER_H

#include<QByteArray>
class FuncHelper
{
public:
    FuncHelper();
    static bool isSmallEnd();
    static int8_t byteToInt8(QByteArray by);

    //小端低放高 无需在意符号位
    static int16_t byteToInt16(QByteArray val,bool smallEnd=true);
    static QByteArray int16ToByte(int16_t val,bool smallEnd=true);

    static int32_t byteToInt32(QByteArray val,bool smallEnd=true);
    static QByteArray int32ToByte(int32_t val,bool smallEnd=true);

    static float byteToFloat32(QByteArray val,bool smallEnd=true);
    static QByteArray float32ToByte(float val,bool smallEnd=true);

    static double byteToDouble64(QByteArray val,bool smallEnd=true);
    static QByteArray double64ToByte(double val,bool smallEnd=true);

};

#endif // FUNCHELPER_H
