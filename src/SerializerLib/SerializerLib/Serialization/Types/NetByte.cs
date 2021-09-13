using System;
using System.Collections.Generic;
namespace SerializerLib
{
    namespace MAP
    {
        public class NetByte : INetworkType
        {
            public NetByte()
            {
                this.m_value = 0;
                this.m_instance_name = new MAP.MemoryTag("SYSTEM-BYTE");

            }

            public NetByte(byte value, string name)
            {
                this.m_value = value;
                this.m_instance_name = new MAP.MemoryTag(name);
            }

            public new void Dispose()
            {
                base.Dispose();
            }


            public override List<byte> RawSerialization()
            {
                return new List<byte>()
                {
                    m_value
                };
            }

            public override List<byte> Serialize()
            {
                List<byte> memoryVector = new List<byte>();
                memoryVector.Add((byte)GetNetworkType());
                var memoryTagVector = m_instance_name.Serialize();
                memoryVector.AddRange(memoryTagVector);
                memoryVector.Add(m_value);
                return memoryVector;
            }

            public override List<INetworkType> RawDeserialization(byte[] argsMemory)
            {
                return new List<INetworkType>(){
                    new MAP.NetByte(argsMemory[0],"NULL")
                };
            }

            public override List<INetworkType> Deserialize(byte[] argsMemory)
            {
                List<INetworkType> objectStructure = new List<INetworkType>();
                var memoryTag = m_instance_name.Deserialize(argsMemory)[0];
                var index = memoryTag.GetSize() + MememoryOffset.OFFSET_1;
                objectStructure.Add(new MAP.NetByte(argsMemory[index], memoryTag.GetName()));
                return objectStructure;
            }

            public override NetworkType GetNetworkType()
            {
                return NetworkType.BYTE;
            }

            public override string GetName()
            {
                return m_instance_name.GetName();
            }

            public override int GetRawSize()
            {
                return sizeof(byte);
            }

            public override int GetSize()
            {
                return m_instance_name.GetSize() + GetRawSize() + 1; //+1 for the type
            }

            public byte GetValue()
            {
                return m_value;
            }

            private byte m_value = new byte();
            private MAP.MemoryTag m_instance_name;
        }
    }
}