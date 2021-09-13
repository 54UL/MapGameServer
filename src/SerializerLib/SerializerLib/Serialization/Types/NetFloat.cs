using System;
using System.Collections.Generic;
using System.Linq;

namespace SerializerLib
{
    namespace MAP
    {
        public class NetFloat : INetworkType
        {
            public const int IS_BIG_ENDIAN = 1;
            public const int IEEE_754_FLOAT_MANTISSA_BITS = 23;
            public const int IEEE_754_FLOAT_EXPONENT_BITS = 8;
            public const int IEEE_754_FLOAT_SIGN_BITS = 1;

            public NetFloat()
            {
                this.m_instance_name = new MAP.MemoryTag("SYSTEM-FLOAT");
            }

            public NetFloat(float value, string name)
            {
                this.m_instance_name = new MAP.MemoryTag(name);
                m_float = value;
            }

            public override List<byte> RawSerialization()
            {
                return BitConverter.GetBytes(m_float).ToList();
            }

            public override List<byte> Serialize()
            {
                List<byte> memoryVector = new List<byte>();
                memoryVector.Add((byte)GetNetworkType());
                var memoryTagVector = m_instance_name.Serialize();
                memoryVector.AddRange(memoryTagVector);
                memoryVector.AddRange(RawSerialization());
                return memoryVector;
            }

            public override List<INetworkType> RawDeserialization(byte[] argsMemory)
            {
                throw new NotImplementedException();
            }

            public override List<INetworkType> Deserialize(byte[] argsMemory)
            {
                List<INetworkType> objectStructure = new List<INetworkType>();
                var memoryTag = m_instance_name.Deserialize(argsMemory)[0];
                var floatValueStartPos = memoryTag.GetSize() + MememoryOffset.OFFSET_1;

                m_float = BitConverter.ToSingle(argsMemory, floatValueStartPos);

                objectStructure.Add(new MAP.NetFloat(m_float, memoryTag.GetName()));
                return objectStructure;
            }

            public override NetworkType GetNetworkType()
            {
                return NetworkType.FLOAT;
            }

            public override string GetName()
            {
                return m_instance_name.GetName();
            }

            public override int GetRawSize()
            {
                return sizeof(float);
            }

            public override int GetSize()
            {
                return m_instance_name.GetSize() + GetRawSize() + 1; //+1 for type byte
            }

            public float GetValue()
            {
                return m_float;
            }

            private float m_float;
            private MAP.MemoryTag m_instance_name;
        }
    }
}