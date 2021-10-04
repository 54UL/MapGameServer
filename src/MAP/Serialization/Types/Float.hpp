
#ifndef FLOAT_NET_TYPE_H
#define FLOAT_NET_TYPE_H
#include "./MemoryTag.hpp"
#include <string>

#define IS_BIG_ENDIAN 1

//Single Precision (float)  --  Standard IEEE 754 Floating-point Specification
#define IEEE_754_FLOAT_MANTISSA_BITS (23)
#define IEEE_754_FLOAT_EXPONENT_BITS (8)
#define IEEE_754_FLOAT_SIGN_BITS (1)

#define IEEE_754_FLOAT_BIAS ((1L << (IEEE_754_FLOAT_EXPONENT_BITS - 1)) - 1)   /* 2^{exponent_bits - 1} - 1 ; 127 */
#define IEEE_754_FLOAT_EXPONENT_MAX ((1L << IEEE_754_FLOAT_EXPONENT_BITS) - 1) /* 2^{exponent_bits} - 1     ; 255 */
#define IEEE_754_FLOAT_IMPLICIT_BIT ((1UL << IEEE_754_FLOAT_MANTISSA_BITS))    /* 2^{mantissa_bits}         ; 8388607 */

#define IEEE_754_FLOAT_SUBNORMALS(exponent, mantissa) ((exponent == 0) && (mantissa >= 1 && mantissa <= ((1UL << IEEE_754_FLOAT_MANTISSA_BITS) - 1)))
#define IEEE_754_FLOAT_ZERO(exponent, mantissa) ((exponent == 0) && (mantissa == 0))
#define IEEE_754_FLOAT_INF(exponent, mantissa) ((exponent == IEEE_754_FLOAT_EXPONENT_MAX) && (mantissa == 0))
#define IEEE_754_FLOAT_NAN(exponent, mantissa) ((exponent == IEEE_754_FLOAT_EXPONENT_MAX) && (mantissa != 0))

#if (IS_BIG_ENDIAN == 1)
typedef union
{
    float value;
    struct
    {
        int8_t sign : IEEE_754_FLOAT_SIGN_BITS;
        int16_t exponent : IEEE_754_FLOAT_EXPONENT_BITS;
        uint32_t mantissa : IEEE_754_FLOAT_MANTISSA_BITS;
    };
} IEEE_754_float;
#else
typedef union
{
    float value;
    struct
    {
        uint32_t mantissa : IEEE_754_FLOAT_MANTISSA_BITS;
        int16_t exponent : IEEE_754_FLOAT_EXPONENT_BITS;
        int8_t sign : IEEE_754_FLOAT_SIGN_BITS;
    };
} IEEE_754_float;
#endif

namespace MAP
{
    class NetFloat : public INetworkType
    {
    public:
        NetFloat();
        NetFloat(float value, std::string name);
        ~NetFloat();

        std::vector<uint8_t> RawSerialization() override;
        std::vector<uint8_t> Serialize() override;
        std::vector<std::shared_ptr<INetworkType>> Deserialize(const uint8_t *argsMemory) override;
        std::vector<std::shared_ptr<INetworkType>> RawDeserialization(std::vector<uint8_t> argsMemory) override;

        NetworkType GetType() override;
        const char *GetName() override;
        uint32_t GetRawSize() override;
        uint32_t GetSize() override;
        float GetValue();

    private:
        IEEE_754_float m_float;
        MAP::MemoryTag m_instance_name;
    };
}

#endif