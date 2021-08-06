# Features

Streams data via UDP using map data structure rules
* Subscribe to an pool (map) and start comunicating
* Upserts information from a pool and notifiy the changes to the subscribed clients
* Centralized server architecture
* Lightweightnetworking solution
  
----

# Example

#### Unity client player:
Subscribe to property changes in the scrit initializatoin...


```c#
//Before you send data, connect and join to some pool
void  start (){
    NetClient.GetPropperty("PLAYER_0_STEER", ((binData) =>{
            SetVehicleSteer(binData.TryGetDouble("steer"));
        }));
}
```


```c#
//Subscribe to a property change, every time the data is recived the  callback function is called
void  start (){
    NetClient.GetPropperty("PLAYER_0_STEER", ((binData) =>{
            SetVehicleSteer(binData.TryGetDouble("steer"));
        }));
}
```
Z
Share the corresponding data each 16 ms to the server 
```c#
public void shareData()
{
    NetClient.UppsertProperty('PLAYER_0_STEER', PlayerController.steer());
}

public IEnumerator sendPackets()
{
    shareData();
    yield return new WaitForSeconds(0.016F); 
}

private void Update()
{
    StartCoroutine("sendPackets");
}
```

----

# Platforms

* Linux and windows for the dedicated server
* Platforms supported by unity for the client
  
----
# MEMORY LAYOUT FOR BINARY TYPES:

## NAME_TAG:
*  3 BYTES MIN 
 * 256 BYTES MAX
#### STRUCTURE:
 | type             | byte span |
 | ---------------- | --------- |
 | TYPE_CODE (0X09) | 1 byte    |
 | TAG_LENGHT       | 1 byte    |
 | CHAR_PAYLOAD     | N bytes   |


###### EXAMPLE BUFFER:  
`[]`

## COMMAND : 
* 2 BYTES WIDE.
#### STRUCTURE:
 | type             | byte span |
 | ---------------- | --------- |
 | TYPE_CODE (0X01) | 1 byte    |
 | ID               | 1 byte    |


## STRING:

*  3 BYTES MIN 
 * 512 BYTES MAX

#### STRUCTURE:
 | type             | byte span |
 | ---------------- | --------- |
 | TYPE_CODE (0X02) | 1 byte    |
 | TAG_NAME         | N bytes   |
 | STR_LENGHT       | 1 byte    |
 | CHAR_PAYLOAD     | N bytes   |

###### EXAMPLE BUFFER:  
`[0x02,0x04,'s','a','u','l']`

## FLOAT :
* 4 BYTES WIDE

#### STRUCTURE:

 | type            | byte span |
 | --------------- | --------- |
 | TYPE_CODE(0X03) | 1 byte    |
 | TAG_NAME        | N bytes   |

 | MANTISSA        | 23 bits   |
 | EXPONENT        | 8  bits   |
 | SIGN            | 1 bit     |

 ## ARRAY: 
 * 5 BYTES MIN MAX 512 BYTES WIDE
  #### STRUCTURE:
 | type            | byte span |
 | --------------- | --------- |
 | TYPE_CODE(0X04) | 1 byte    |
 | TAG_NAME        | N bytes   |

 | ARRAY_LENGHT    | 2 bytes   |
 | ARRAY_TYPE      | 1 byte    |
 | ARRAY_PAYLOAD   | N bytes   |

###### EXAMPLE BUFFER:  
Array of strings equivalent to

``js
TEST = ["hello","world]
``

` [0x04,0X01,0X04,'T','E','S','T',0x02,0x05,'h','e','l','l','o',0x05,'w','o','r','l'd']` 

Array of binary objecst (interpreted by clients):

` [0x04,SIZE,0x08,DATA_1_SIZE,DATA_1,DATA_2_SIZE,DATA_2...]`

 ## BYTE:
 * 2 byte wide
  #### STRUCTURE:

 | type            | byte span |
 | --------------- | --------- |
 | TYPE_CODE(0X05) | 1 byte    |
 | TAG_NAME        | N bytes   |
 | VALUE           | 1 byte    |

##  INT : 
* 5 bytes wide
#### STRUCTURE:
| type            | byte span |
| --------------- | --------- |
| TYPE_CODE(0X06) | 1 byte    |
| TAG_NAME        | N bytes   |

| VALUE           | 4 byte    |

 ## UINT: 
 * 5 bytes wide
#### STRUCTURE:
| type            | byte span |
| --------------- | --------- |
| TYPE_CODE(0X07) | 1 byte    |
| TAG_NAME        | N bytes   |

| VALUE           | 4 byte    |

## OBJECT:
 * MIN 2 bytes 
 * MAX N bytes wide 
#### STRUCTURE:
| type            | byte span |
| --------------- | --------- |
| TYPE_CODE(0x08) | 1 byte    |
| TAG_NAME        | N bytes   |

| DATA            | N bytes   |
----

# Commands specification
#### SUBSCRIBE:
~~~
CODE : 0X01
    DESCRIPTION:
    SUBSCRIBE {PLAYER_NAME} {PLAYER_ADDRESS}  {PORT}
    Ej:
    [0X02,0X01,STRING,STRING,INT]
RESPONSE:
{STRING:AccessToken,INT:ClientId}
~~~

#### UNSUSCRBIE:
~~~
CODE : 0X02
DESCRIPTION:
    UNSUSCRBIE {POOL_ID} 
    ej:
    [0X02,0X02,INT]
RESPONSE:
{INT:Status}
~~~

#### START_POOL
~~~
CODE : 0X03
DESCRIPTION:
    START_POOL {POOL_NAME}
    ej:
    [0X02,0X03,STRING]
RESPONSE:
{INT:Status}
~~~

#### END_POOL
~~~
CODE : 0X04
DESCRIPTION:
    END_POOL {INT} 
    ej:
    [0X02,0X04,POOL_ID]
RESPONSE:
{INT:Status}
~~~

#### UPSERT:
~~~
CODE : 0x05
DESCRIPTION:
    UPSERT {INT} {TYPE_HEADER} {DATA}
    ej:
    [0X02,0X05,ID,TYPE,DATA]
RESPONSE:
~~~

#### REMOVE
~~~
CODE : 0x06
DESCRIPTION:
    UPSERT {ID} 
    ej:
    [0X02,0X05,INT]
RESPONSE:
{INT:Status}
~~~

#### SPAWN:
~~~
CODE : 0x08
DESCRIPTION:
    SPAWN {PREFAB_ID,FLOAT_VECTOR_3,FLOAT_QUATERNIION} 
    ej:
    [0X02,0X05,INT,OBJECT,OBJECT]
RESPONSE:
{INT:Status}
~~~

#### GET_ACTIVE_POOLS:
~~~
CODE : 0x9
DESCRIPTION:
    GET_ACTIVE_POOLS 
    ej:
    [0X02,0X09]
RESPONSE:
{INT:Status,ARRAY:ActivePools}
~~~
----
# Server commands summary
SPEC DRAFT 1.0 (NO SECURITY FEATURES)

 | CODE | NAME             | PAYLOAD                              | RESPONSE                | BROADCAST |
 | ---- | ---------------- | ------------------------------------ | ----------------------- | --------- |
 | 0x00 | SUBSCRIBE        | {PlayerName,HostName,IpAddress,port} | {playerId}              | false     |
 | 0X01 | UNSUBSCRIBE      | {PoolId}                             | NONE                    | false     |
 | 0X02 | START_POOL       | {}                                   | {PoolId}                | false     |
 | 0X03 | END_POOL         | {PoolId}                             | {Status}                | false     |
 | 0x04 | UPSERT           | {PROPERTY_ID,VALUE}                  | {propertyName,newValue} | true      |
 | 0x05 | REMOVE           | {PROPERTY_ID}                        | {propertyName}          | true      |
 | 0x06 | SPAWN            | {PREFAB_NAME,TRANSFORM}              | {propertyName}          | true      |
 | 0X07 | GET_ACTIVE_POOLS | {}                                   | {pools[]}               | false     |

---

### Single Precision (float)  --  Standard IEEE 754 Floating-point Specification
```c++
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
# endif```
https://docs.microsoft.com/en-us/dotnet/api/system.bitconverter?view=net-5.0








