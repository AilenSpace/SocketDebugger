#include "funchelper.h"

FuncHelper::FuncHelper()
{

}

bool FuncHelper::isSmallEnd()
{
    int i = 1;
    char c = (*(char*)&i);

    if (c)
        return true;
    else
        return false;
}

int8_t FuncHelper::byteToInt8(QByteArray by)
{
    return by[0];
}

int16_t FuncHelper::byteToInt16(QByteArray val,bool smallEnd)
{
    int16_t tmp=0;
    for(int i=0;i<2;i++){
        int tmpI=smallEnd?1-i:i;

        tmp=(tmp <<8);
        tmp=(tmp|(uint8_t)(val[tmpI]));
    }
    return tmp;
}

QByteArray FuncHelper::int16ToByte(int16_t val,bool smallEnd)
{
    QByteArray by;
    by.resize(2);
    for(int i=0;i<2;i++){
        int tmpI=smallEnd?i:1-i;
        by[tmpI]=(val>>(i*8))&0xFF;
    }
    return by;
}

int32_t FuncHelper::byteToInt32(QByteArray val,bool smallEnd)
{
    int32_t tmp=0;
    for(int i=0;i<4;i++){
        int tmpI=smallEnd?3-i:i;

        tmp=(tmp<<8);
        tmp=(tmp|(uint8_t)(val[tmpI]));
    }
    return tmp;
}

QByteArray FuncHelper::int32ToByte(int32_t val,bool smallEnd)
{
    QByteArray by;
    by.resize(4);
    for(int i=0;i<4;i++){
        int tmpI=smallEnd?i:3-i;
        by[tmpI]=(val>>(i*8))&0xFF;
    }
    return by;
}

float FuncHelper::byteToFloat32(QByteArray val, bool smallEnd)
{
    float tmp=0;
    char* dest=(char*)(&tmp);
    if(isSmallEnd()){
        for(int i=0;i<4;i++){
            int tmpI=smallEnd?i:3-i;
            *(dest+i)=val[tmpI];
        }
    }else{
        for(int i=0;i<4;i++){
            int tmpI=smallEnd?3-i:i;
            *(dest+i)=val[tmpI];
        }
    }
    return tmp;
}

QByteArray FuncHelper::float32ToByte(float val, bool smallEnd)
{
    QByteArray by;
    by.resize(4);
    char* dest=(char*)(&val);
    if(isSmallEnd()){
        for(int i=0;i<4;i++){
            int tmpI=smallEnd?i:3-i;
            by[tmpI]=*(dest+i);
        }
    }else{
        for(int i=0;i<4;i++){
            int tmpI=smallEnd?3-i:i;
            by[tmpI]=*(dest+i);
        }
    }
    return by;
}

double FuncHelper::byteToDouble64(QByteArray val, bool smallEnd)
{
    double tmp=0;
    char* dest=(char*)(&tmp);
    if(isSmallEnd()){
        for(int i=0;i<8;i++){
            int tmpI=smallEnd?i:7-i;
            *(dest+i)=val[tmpI];
        }
    }else{
        for(int i=0;i<8;i++){
            int tmpI=smallEnd?7-i:i;
            *(dest+i)=val[tmpI];
        }
    }
    return tmp;
}

QByteArray FuncHelper::double64ToByte(double val, bool smallEnd)
{
    QByteArray by;
    by.resize(8);
    char* dest=(char*)(&val);
    if(isSmallEnd()){
        for(int i=0;i<8;i++){
            int tmpI=smallEnd?i:7-i;
            by[tmpI]=*(dest+i);
        }
    }else{
        for(int i=0;i<8;i++){
            int tmpI=smallEnd?7-i:i;
            by[tmpI]=*(dest+i);
        }
    }
    return by;
}
