MapGameServer


### NETWORK BINARY SERIALIZATION.
# NETWORK TYPES MEMORY LAYOUT:

#### COMMAND : 2 BYTES WIDE. 
    TYPE_CODE (0X01)       : 1 byte
    ID   : 1 byte                                 

#### STRING: 3 BYTES MIN , MAX 512 BYTES WIDE
    TYPE_CODE (0X02)      : 1 byte
    STR_LENGHT                  : 1 byte
    CHAR_PAYLOAD            : N bytes
    EJ:  
    [0x02,0x04,'s','a','u','l']
    
#### FLOAT : 4 BYTES WIDE
    TYPE_CODE(0X03): 1 byte
    MANTISSA             : 23 bits
    EXPONENT           : 8  bits
    SIGN                        : 1 bit

 #### ARRAY:  5 BYTES MIN MAX 512 BYTES WIDE
    TYPE_CODE(0X04): 1 byte
    ARRAY_LENGHT: 2 bytes
    ARRAY_TYPE: 1 byte 
    ARRAY_PAYLOAD: N bytes
    EJ:
    Array of strings equivalent to : ["hello","world]
    [0x04,0x02,0x05,'h','e','l','l','o',0x05,'w','o','r','l,'d'] 
    Array of binary objecst (interpreted by clients):
    [0x04,SIZE,0x08,DATA_1_SIZE,DATA_1,DATA_2_SIZE,DATA_2...]


 #### BYTE: 2 byte wide
    TYPE_CODE(0X05) :1 byte
    VALUE: 1 byte

####  INT : 5 bytes wide
    TYPE_CODE(0X06) :1 byte
    VALUE: 4 byte

 #### UINT: 5 bytes wide
    TYPE_CODE(0X07)  :1 byte
    VALUE                          : 4 byte

#### OBJECT: MIN 2 bytes MAX N bytes wide 
    TYPE_CODE(0x08) :1 byte
    DATA:N bytes


# SEQUENCE TYPES

SUBSCRIBE:
CODE : 0X01
    DESCRIPTION:
    SUBSCRIBE {PLAYER_NAME} {PLAYER_ADDRESS}  {PORT}
    Ej:
    [0X01,0X01,STRING,STRING,INT]
RESPONSE:
{STRING:AccessToken,INT:ClientId}

UNSUSCRBIE:
CODE : 0X02
DESCRIPTION:
    UNSUSCRBIE {POOL_ID} 
    ej:
    [0X01,0X02,INT]
RESPONSE:
{INT:Status}

START_POOL
CODE : 0X03
DESCRIPTION:
    START_POOL {POOL_NAME}
    ej:
    [0X01,0X03,STRING]
RESPONSE:
{INT:Status}

END_POOL
CODE : 0X04
DESCRIPTION:
    END_POOL {INT} 
    ej:
    [0X01,0X04,POOL_ID]
RESPONSE:
{INT:Status}

UPSERT:
CODE : 0x05
DESCRIPTION:
    UPSERT {INT} {TYPE_HEADER} {DATA}
    ej:
    [0X01,0X05,ID,TYPE,DATA]
RESPONSE:

REMOVE
CODE : 0x06
DESCRIPTION:
    UPSERT {ID} 
    ej:
    [0X01,0X05,INT]
RESPONSE:
{INT:Status}

SPAWN:
CODE : 0x08
DESCRIPTION:
    SPAWN {PREFAB_ID,FLOAT_VECTOR_3,FLOAT_QUATERNIION} 
    ej:
    [0X01,0X05,INT,OBJECT,OBJECT]
RESPONSE:
{INT:Status}

GET_ACTIVE_POOLS:
CODE : 0x9
DESCRIPTION:
    GET_ACTIVE_POOLS 
    ej:
    [0X01,0X09]
RESPONSE:
{INT:Status,ARRAY:ActivePools}


#NET SERVER SPEC DRAFT 1.0 (NO SECURITY FEATURES)
### Connection
COMMAND_HEADER {header,payload}
SERVER COMMANDS:
SERVER CONTROL COMMANDS:
SUBSCRIBE {PlayerName,HostName,IpAddress,port};
UNSUBSCRIBE {POOL_ID}
START_POOL {DATA}
END_POOL {DATA}; RESPONSE TO CLIENTS: "STATUS:POOL ENEDED"```
 ### DATA STREAMING/BROADCASTING COMMANDS (both sides commands):
UPSERT {PROPERTY_ID,{value}}; RESPONSE TO CLIENT: {propertyName, newValue}
REMOVE   {PROPERTY_ID}; RESPONSE TO CLIENT : {propertyName}   
SPAWN {PREFAB_NAME,TRANSFORM{VECTOR3,QUATERNION}}; RESPONSE TO CLIENT :{propertyName}
### REQUEST COMMANDS 
GET_ACTIVE_POOLS; RESPONSE TO CLIENT : {pools[]}  
      

/*
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
**  Single Precision (float)  --  Standard IEEE 754 Floating-point Specification
*/

# define IEEE_754_FLOAT_MANTISSA_BITS (23)
# define IEEE_754_FLOAT_EXPONENT_BITS (8)
# define IEEE_754_FLOAT_SIGN_BITS     (1)

# define IEEE_754_FLOAT_BIAS          ((1L << (IEEE_754_FLOAT_EXPONENT_BITS - 1)) - 1)  /* 2^{exponent_bits - 1} - 1 ; 127 */
# define IEEE_754_FLOAT_EXPONENT_MAX  ((1L << IEEE_754_FLOAT_EXPONENT_BITS) - 1)        /* 2^{exponent_bits} - 1     ; 255 */
# define IEEE_754_FLOAT_IMPLICIT_BIT  ((1UL << IEEE_754_FLOAT_MANTISSA_BITS))           /* 2^{mantissa_bits}         ; 8388607 */

# define IEEE_754_FLOAT_SUBNORMALS(exponent, mantissa) ((exponent == 0) && (mantissa >= 1 && mantissa <= ((1UL << IEEE_754_FLOAT_MANTISSA_BITS) - 1)))
# define IEEE_754_FLOAT_ZERO(exponent, mantissa)       ((exponent == 0) && (mantissa == 0))
# define IEEE_754_FLOAT_INF(exponent, mantissa)        ((exponent == IEEE_754_FLOAT_EXPONENT_MAX) && (mantissa == 0))
# define IEEE_754_FLOAT_NAN(exponent, mantissa)        ((exponent == IEEE_754_FLOAT_EXPONENT_MAX) && (mantissa != 0))

# if (IS_BIG_ENDIAN == 1)
    typedef union {
        float value;
        struct {
            int8_t   sign     : IEEE_754_FLOAT_SIGN_BITS;
            int16_t  exponent : IEEE_754_FLOAT_EXPONENT_BITS;
            uint32_t mantissa : IEEE_754_FLOAT_MANTISSA_BITS;
        };
    } IEEE_754_float;
# else
    typedef union {
        float value;
        struct {
            uint32_t mantissa : IEEE_754_FLOAT_MANTISSA_BITS;
            int16_t  exponent : IEEE_754_FLOAT_EXPONENT_BITS;
            int8_t   sign     : IEEE_754_FLOAT_SIGN_BITS;
        };
    } IEEE_754_float;
# endif



https://docs.microsoft.com/en-us/dotnet/api/system.bitconverter?view=net-5.0