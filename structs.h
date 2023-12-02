#ifndef STRUCTS_H
#define STRUCTS_H
#include <QHostAddress>
enum class IOFormat{
    MIN,
    FROM_HEX,
    TO_HEX,
    BYTE_ARRAY,
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

enum class ReadMode{
    MIN,
    ReadAll,
    Fixed,
    Head,
    MAX
};
class ValueSetting{
public:
    unsigned int valueOffset;//值 位置的偏移量1开始计数
    ValueBitType  valueBitType;//值 的字节数量
    SignedType signedType;//值 是否是无符号
    EndianType endianType;//值 大小端存储
};
enum class AcquisitionMode{
    MIN,
    Continuous,
    Single,
    MAX
};

class AdvSetting{
public:
    ValueSetting packageSize;//包大小
    ReadMode readMode;
    int fixedSize;
};

class BasicSetting
{
public:
     virtual ~BasicSetting(){}
     ProtocolType protocolType;//协议
};

class TcpSetting:public BasicSetting
{
public:

    QHostAddress destIp;
    unsigned int destPort;
    QHostAddress srcIp;
    unsigned int srcPort;
};
class UdpSetting:public BasicSetting
{
public:
    QHostAddress srcIp;
    unsigned int srcPort;
};

class DebugSetting{
public:
    std::shared_ptr<BasicSetting> setting;
    IOFormat oFormat;//输出格式
    AcquisitionMode acquisitionMode;//采集模式
    ValueSetting value; //实时值查询
    //用于粘包的协议如tcp
    AdvSetting advSetting;
};

inline QString readModeToString(ReadMode val){
    if(ReadMode::Fixed==val){
        return "Fixed";
    }else if(ReadMode::Head==val){
        return "Head";
    }else if(ReadMode::ReadAll==val){
        return "ReadAll";
    }
    return "";
}
inline QString acquisitionModeToString(AcquisitionMode val){
    if(AcquisitionMode::Continuous==val){
        return "Continuous";
    }else if(AcquisitionMode::Single==val){
        return "Single";
    }
    return "";
}
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
inline QString IOFormatToString(IOFormat val){
    if(IOFormat::TO_HEX==val){
        return "to_hex";
    }else if(IOFormat::FROM_HEX==val){
        return "from_hex";
    }else if(IOFormat::BYTE_ARRAY==val){
        return "byte_array";
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
