#pragma once

#ifndef TDS_PROTO_H
#define TDS_PROTO_H

#include <boost/format.hpp>

#include <iostream>

namespace tds {

enum class PKT_TYPES: uint8_t {
    UNDEF       =  0,
    QUERY       =  1,
    LOGIN       =  2,
    RPC         =  3,
    RESP        =  4,
    RAW         =  5,
    ATTENTION   =  6,
    BULK_DATA   =  7,
    OPEN_CHN    =  8,
    CLOSE_CHN   =  9,
    RES_ERROR   = 10,
    LOG_CHN_ACK = 11,
    ECHO        = 12,
    LOGOUT_CHN  = 13,
    TRANS_MGR   = 14,
    QUERY5      = 15,  /* or "Normal tokenized request or response */
    LOGIN7      = 16,  /* or "Urgent tokenized request or response */
    SSPI        = 17,
    PRELOGIN    = 18,
    INVALID     = 19
};

enum class PKT_STATUS: uint8_t {
    NormalMsg = 0x00,
    EOM = 0x01,
    Ignore = 0x02,
    RESETCONNECTION = 0x08,
    RESETCONNECTIONSKIPTRAN = 0x10
};

enum class TDS_TOKEN: uint8_t {
    TDS5_PARAMFMT2      = 0x20, // TDS 5.0 only
    TDS_LANG            = 0x21, // TDS 5.0 only
    TDS5_ORDERBY2       = 0x22, // TDS 5.0 only
    TDS5_CURDECLARE2    = 0x23, // TDS 5.0 only        [###]
    TDS5_ROWFMT2        = 0x61, // TDS 5.0 only
    TDS5_MSG            = 0x65, // TDS 5.0 only        [###]
    TDS_LOGOUT          = 0x71, // TDS 5.0 only? ct_close()
    TDS_RET_STAT        = 0x79, //
    TDS_PROCID          = 0x7C, // TDS 4.2 only - TDS_PROCID
    TDS_CURCLOSE        = 0x80, // TDS 5.0 only
    TDS7_RESULT         = 0x81, // TDS 7.0 only
    TDS_CURFETCH        = 0x82, // TDS 5.0 only
    TDS_CURINFO         = 0x83, // TDS 5.0 only
    TDS_CUROPEN         = 0x84, // TDS 5.0 only
    TDS_CURDECLARE      = 0x86, // TDS 5.0 only
    TDS7_COMPUTE_RESULT = 0x88, // TDS 7.0 only
    TDS_COL_NAME        = 0xA0, // TDS 4.2 only
    TDS_COL_INFO4       = 0xA1, // TDS 4.2 only - TDS_COLFMT
    TDS5_DYNAMIC2       = 0xA3, // TDS 5.0 only
    TDS_TABNAME         = 0xA4, //
    TDS_COL_INFO        = 0xA5, //
    TDS_OPTIONCMD       = 0xA6, //
    TDS_COMPUTE_NAMES   = 0xA7, //
    TDS_COMPUTE_RESULT  = 0xA8, //
    TDS_ORDER_BY        = 0xA9, // TDS_ORDER
    TDS_ERR             = 0xAA, //
    TDS_MSG             = 0xAB, //
    TDS_PARAM           = 0xAC, // RETURNVALUE?
    TDS_LOGIN_ACK       = 0xAD, //
    TDS_CONTROL         = 0xAE, // TDS_CONTROL
    TDS_KEY             = 0xCA, //                     [###]
    TDS_ROW             = 0xD1, //
    TDS_CMP_ROW         = 0xD3, //
    TDS5_PARAMS         = 0xD7, // TDS 5.0 only
    TDS_CAP             = 0xE2, //
    TDS_ENV_CHG         = 0xE3, //
    TDS_EED             = 0xE5, //
    TDS_DBRPC           = 0xE6, //
    TDS5_DYNAMIC        = 0xE7, // TDS 5.0 only
    TDS5_PARAMFMT       = 0xEC, // TDS 5.0 only
    TDS_AUTH            = 0xED, //
    TDS_RESULT          = 0xEE, //
    TDS_DONE            = 0xFD, // TDS_DONE
    TDS_DONEPROC        = 0xFE, // TDS_DONEPROC
    TDS_DONEINPROC      = 0xFF  // TDS_DONEINPROC
};

/* Microsoft internal stored procedure id's */

enum class SP_ID: uint8_t {
    CURSOR          =  1,
    CURSOROPEN      =  2,
    CURSORPREPARE   =  3,
    CURSOREXECUTE   =  4,
    CURSORPREPEXEC  =  5,
    CURSORUNPREPARE =  6,
    CURSORFETCH     =  7,
    CURSOROPTION    =  8,
    CURSORCLOSE     =  9,
    EXECUTESQL      = 10,
    PREPARE         = 11,
    EXECUTE         = 12,
    PREPEXEC        = 13,
    PREPEXECRPC     = 14,
    UNPREPARE       = 15
};

enum class RPC_OPT: uint8_t {
    WITH_RECOMP    = 0x01,
    NO_METADATA    = 0x02,
    REUSE_METADATA = 0x04
};

enum class RPC_PARAMETER_STATUS: uint8_t {
    BY_REF  = 0x01,
    DEFAULT = 0x02
};

// Sybase Data Types

enum class SYB_TYPES: uint8_t {
    SYBCHAR       = 0x2F,
    SYBVARCHAR    = 0x27,
    SYBINTN       = 0x26,
    SYBINT1       = 0x30,
    SYBINT2       = 0x34,
    SYBINT4       = 0x38,
    SYBINT8       = 0x7F,
    SYBFLT8       = 0x3E,
    SYBDATETIME   = 0x3D,
    SYBBIT        = 0x32,
    SYBTEXT       = 0x23,
    SYBNTEXT      = 0x63,
    SYBIMAGE      = 0x22,
    SYBMONEY4     = 0x7A,
    SYBMONEY      = 0x3C,
    SYBDATETIME4  = 0x3A,
    SYBREAL       = 0x3B,
    SYBBINARY     = 0x2D,
    SYBVOID       = 0x1F,
    SYBVARBINARY  = 0x25,
    SYBNVARCHAR   = 0x67,
    SYBBITN       = 0x68,
    SYBNUMERIC    = 0x6C,
    SYBDECIMAL    = 0x6A,
    SYBFLTN       = 0x6D,
    SYBMONEYN     = 0x6E,
    SYBDATETIMN   = 0x6F,
    XSYBCHAR      = 0xA7,
    XSYBVARCHAR   = 0xAF,
    XSYBNVARCHAR  = 0xE7,
    XSYBNCHAR     = 0xEF,
    XSYBVARBINARY = 0xA5,
    XSYBBINARY    = 0xAD,
    SYBLONGBINARY = 0xE1,
    SYBSINT1      = 0x40,
    SYBUINT2      = 0x41,
    SYBUINT4      = 0x42,
    SYBUINT8      = 0x43,
    SYBUNIQUE     = 0x24,
    SYBVARIANT    = 0x62
};

// FIXEDLENTYPE

enum class TDS_TYPES: uint8_t {
    NULLT           = 0x1F,  // Null (no data associated with this type)
    INT1            = 0x30,  // TinyInt (1 byte data representation)
    BIT             = 0x32,  // Bit (1 byte data representation)
    INT2            = 0x34,  // SmallInt (2 byte data representation)
    INT4            = 0x38,  // Int (4 byte data representation)
    DATETIM4        = 0x3A,  // SmallDateTime (4 byte data representation)
    FLT4            = 0x3B,  // Real (4 byte data representation)
    MONEY           = 0x3C,  // Money (8 byte data representation)
    DATETIME        = 0x3D,  // DateTime (8 byte data representation)
    FLT8            = 0x3E,  // Float (8 byte data representation)
    MONEY4          = 0x7A,  // SmallMoney (4 byte data representation)
    INT8            = 0x7F,  // BigInt (8 byte data representation)
    // BYTELEN_TYPE
    GUID            = 0x24,  // UniqueIdentifier
    INTN            = 0x26,
    DECIMAL         = 0x37,  // Decimal (legacy support)
    NUMERIC         = 0x3F,  // Numeric (legacy support)
    BITN            = 0x68,
    DECIMALN        = 0x6A,  // Decimal
    NUMERICN        = 0x6C,  // Numeric
    FLTN            = 0x6D,
    MONEYN          = 0x6E,
    DATETIMN        = 0x6F,
    DATEN           = 0x28,  // (introduced in TDS 7.3)
    TIMEN           = 0x29,  // (introduced in TDS 7.3)
    DATETIME2N      = 0x2A,  // (introduced in TDS 7.3)
    DATETIMEOFFSETN = 0x2B,  // (introduced in TDS 7.3)
    CHAR            = 0x2F,  // Char (legacy support)
    VARCHAR         = 0x27,  // VarChar (legacy support)
    BINARY          = 0x2D,  // Binary (legacy support)
    VARBINARY       = 0x25,  // VarBinary (legacy support)
    // USHORTLEN_TYPE
    BIGVARBIN       = 0xA5,  // VarBinary
    BIGVARCHR       = 0xA7,  // VarChar
    BIGBINARY       = 0xAD,  // Binary
    BIGCHAR         = 0xAF,  // Char
    NVARCHAR        = 0xE7,  // NVarChar
    NCHAR           = 0xEF,  // NChar
    // LONGLEN_TYPE
    XML             = 0xF1,  // XML (introduced in TDS 7.2)
    UDT             = 0xF0,  // CLR-UDT (introduced in TDS 7.2)
    TEXT            = 0x23,  // Text
    IMAGE           = 0x22,  // Image
    NTEXT           = 0x63,  // NText
    SSVARIANT       = 0x62   // Sql_Variant (introduced in TDS 7.2)
};

enum class TDS_PROTOCOL: uint8_t {
    V_NOT_SPECIFIED = 0x00,
    V_4             = 0x40,
    V_5             = 0x50,
    V_7_0           = 0x70,
    V_7_1           = 0x71,
    V_7_2           = 0x72,
    V_7_3           = 0x73
};

enum class STREAM_HDR_TYPE: uint8_t {
    QUERY_NOTIF = 0x0001,
    TRANS_DESCR = 0x0002
};

enum class PKT_FLAGS {
    UNDEF = 0,
    RAW,
    SQL_BATCH,
    QUERY
};

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)

struct packet_hdr_t {
    PKT_TYPES  type;
    PKT_STATUS status;
    uint16_t Length; // Network byte order (Big-Endian)
    uint16_t SPID;
    uint8_t PacketID;
    uint8_t Window; // unused
};

struct stream_header_t {
    uint32_t TotalLength;
    uint32_t HeaderLength;
    uint16_t HeaderType;
};

struct trans_descr_hdr_t {
    //uint32_t OutstandingRequestCount;
    //uint64_t TransactionDescriptor;
    uint16_t OutstandingRequestCount;
    uint16_t TransactionDescriptor;
};

#pragma pack(pop)

class pdu {
public:
    using pdu_ptr_t = std::unique_ptr<pdu>;
private:
    uint8_t*  m_data;
    size_t    m_size;
protected:
    pdu_ptr_t m_inner_pdu;
public:
    pdu() = delete;
    pdu(uint8_t* _data, size_t _size)
        : m_data{_data}
        , m_size{_size}
    {
    }

    inline uint8_t* data() { return m_data; }
    inline size_t    size() { return m_size; }

    inline pdu* inner_pdu()
    {
        return m_inner_pdu.get();
    }

    virtual ~pdu() {}

    template <typename T>
    T* find_pdu(PKT_FLAGS flag = T::PKT_FLAG) {
        pdu* _pdu = this;

        while (_pdu != nullptr) {
            if (_pdu->pdu_type() == flag)
                return static_cast<T*>(_pdu);

            _pdu = _pdu->inner_pdu();
        }

        return nullptr;
    }

    virtual PKT_FLAGS pdu_type()
    {
        return PKT_FLAG;
    }

    static constexpr auto PKT_FLAG {PKT_FLAGS::UNDEF};
};

class query_pdu: public pdu
{
    std::wstring m_text;
public:
    query_pdu() = delete;
    query_pdu(uint8_t* _data, size_t _size)
        : pdu(_data,_size)
    {
        std::wstringstream _stream;
        _stream.write((wchar_t*)_data, size()/sizeof(wchar_t));
        m_text = _stream.str();
    }

    std::wstring& text()
    {
        return m_text;
    }

    PKT_FLAGS pdu_type() override
    {
        return PKT_FLAG;
    }

    static constexpr auto PKT_FLAG {PKT_FLAGS::QUERY};
};

class sql_batch: public pdu
{
    stream_header_t* m_stm_hdr_ptr;
public:
    sql_batch() = delete;
    sql_batch(uint8_t* _data, size_t _size)
        : pdu(_data,_size)
        , m_stm_hdr_ptr {reinterpret_cast<stream_header_t*>(_data)}
    {
        if (_size < sizeof(stream_header_t)) {
            auto msg = boost::str(boost::format("{0} failed. Data size={1} < sizeof(hdr)={2}") % __FUNCSIG__ % _size % sizeof(*m_stm_hdr_ptr));
            throw std::runtime_error(msg);
        }

        switch (m_stm_hdr_ptr->HeaderType)
        {
            case STREAM_HDR_TYPE::TRANS_DESCR: {
                    auto inner_pdu_begin = data()+m_stm_hdr_ptr->HeaderLength + sizeof(trans_descr_hdr_t);
                    auto inner_pdu_size  = size()-m_stm_hdr_ptr->HeaderLength - sizeof(trans_descr_hdr_t);
                    m_inner_pdu = std::make_unique<query_pdu>(inner_pdu_begin, inner_pdu_size);
                } break;
            default:
                //throw std::runtime_error("Unknown PKT_TYPE. It can't be handled yet.");
                break;
        }
    }

    ~sql_batch() override {}

    PKT_FLAGS pdu_type() override
    {
        return PKT_FLAG;
    }

    static constexpr auto PKT_FLAG {PKT_FLAGS::SQL_BATCH};
};

class packet: public pdu {
    packet_hdr_t* m_hdr_ptr;
public:
    packet(uint8_t* _data, size_t _size)
        : pdu(_data, _size)
        , m_hdr_ptr{ reinterpret_cast<packet_hdr_t*>(_data) }
    {
        if (_size < sizeof(packet_hdr_t)) {
            auto msg = boost::str(boost::format("{0} failed. Data size={1} < sizeof(hdr)={2}") % __FUNCSIG__ % _size % sizeof(*m_hdr_ptr));
            throw std::runtime_error(msg);
        }

        switch (m_hdr_ptr->type)
        {
            case PKT_TYPES::QUERY:
                m_inner_pdu = std::make_unique<sql_batch>(data()+sizeof(*m_hdr_ptr), size()-sizeof(*m_hdr_ptr));
                break;
            default:
                //throw std::runtime_error("Unknown PKT_TYPE. It can't be handled yet.");
                break;
        }
    }

    ~packet() override {}

    inline PKT_TYPES type() { return m_hdr_ptr->type; }
    inline void type(PKT_TYPES _packet_type) { m_hdr_ptr->type = _packet_type; }

    inline PKT_STATUS status() { return m_hdr_ptr->status; }
    inline void status(PKT_STATUS _packet_status) { m_hdr_ptr->status = _packet_status; }

    inline uint16_t length() { return m_hdr_ptr->Length; }
    inline void length(uint16_t _length) { m_hdr_ptr->Length = _length; }

    inline uint16_t channel() { return m_hdr_ptr->SPID; }
    inline void channel(uint16_t _channel) { m_hdr_ptr->SPID = _channel; }

    inline uint8_t pkt_number() { return m_hdr_ptr->PacketID; }
    inline void pkt_number(uint8_t _pkt_number) { m_hdr_ptr->PacketID = _pkt_number; }

    inline uint8_t window() { return m_hdr_ptr->Window; }
    inline void window(uint8_t _window) { m_hdr_ptr->Window = _window; }

    PKT_FLAGS pdu_type() override
    {
        return PKT_FLAG;
    }

    static constexpr auto PKT_FLAG {PKT_FLAGS::RAW};
};

}

#endif // TDS_PROTO_H
