#ifndef STRUCTS_H
#define STRUCTS_H
#include <QHostAddress>
enum class ShowFormat{
    MIN,
    HEX,
    STRING,
    MAX
};

enum class ProtocolType{
    MIN,
    UDP_CLIENT,
    TCP_CLIENT,
    UDP_SERVRE,
    TCP_SERVRE,
    MAX
};

enum class ValueBitType{
    MIN,
    INT8_T,
    INT16_T,
    INT32_T,
    FLOAT32_T,
    DOUBLE64_T,
    MAX
};

enum class SignedType{
    MIN,
    Unsigned,
    Signed,
    MAX
};

enum class EndianType{
    MIN,
    SMALL,
    BIG,
    MAX
};


class ValueSetting{
public:
    unsigned int valueOffset;//值 位置的偏移量
    ValueBitType  valueBitType;//值 的字节数量
    SignedType signedType;//值 是否是无符号
    EndianType endianType;//值 大小端存储
};

class HeadSetting{
public:
    bool hasPacketHead;//是否具有包头 对于tcp应当具有包头head+body方式否者会有粘贴包的问题
    bool fullPacketSize;//是否是包含 包头的大小
    ValueSetting packageSize;//形容包大小
};


class DebugSetting{
public:
    ProtocolType protocolType;
    ShowFormat showFormat;
    HeadSetting head;
    QHostAddress ip;
    unsigned int port;
    ValueSetting value;
};

inline QString signedTypeToString(SignedType val){
    if(SignedType::Signed==val){
        return "Signed";
    }else if(SignedType::Unsigned==val){
        return "Unsigned";
    }
    return "";
}
inline QString endianTypeToString(EndianType val){
    if(EndianType::SMALL==val){
        return "small";
    }else if(EndianType::BIG==val){
        return "big";
    }
    return "";
}
inline QString showFormatToString(ShowFormat val){
    if(ShowFormat::HEX==val){
        return "hex";
    }else if(ShowFormat::STRING==val){
        return "string";
    }
    return "";
}
inline QString valueBitTypeToString(ValueBitType val){
    if(ValueBitType::INT8_T==val){
        return "int8";
    }else if(ValueBitType::INT16_T==val){
        return "int16";
    }else if(ValueBitType::INT32_T==val){
        return "int32";
    }else if(ValueBitType::FLOAT32_T==val){
        return "float32";
    }else if(ValueBitType::DOUBLE64_T==val){
        return "double64";
    }
    return "";
}

inline QString protocolTypeToString(ProtocolType val) {
    if(ProtocolType::UDP_CLIENT==val){
        return "UDP_CLIENT";
    }else if(ProtocolType::TCP_CLIENT==val){
        return "TCP_CLIENT";
    }else if(ProtocolType::UDP_SERVRE==val){
        return "UDP_SERVRE";
    }else if(ProtocolType::TCP_SERVRE==val){
        return "TCP_SERVRE";
    }

    return "";
};
#endif // STRUCTS_H
