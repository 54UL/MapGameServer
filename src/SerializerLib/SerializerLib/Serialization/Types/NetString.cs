using System;
using System.Collections.Generic;
using System.Text;

namespace SerializerLib
{
    namespace MAP
    {
        public class NetString : INetworkType
        {
            public NetString()
            {
                this.m_instance_name = new MAP.MemoryTag("STRING-SYSTEM");
                this.m_string_value = new List<byte>(Encoding.UTF8.GetBytes("null"));
            }

            public NetString(string value, string name)
            {
                this.m_instance_name = new MAP.MemoryTag(name);
                this.m_string_value = new List<byte>(Encoding.UTF8.GetBytes(value));
            }

            public override List<byte> RawSerialization()
            {
                List<byte> memoryVector = new List<byte>();
                memoryVector.Add((byte)(m_string_value.Count)); //Length string
                memoryVector.AddRange(m_string_value);          //CharacterValues
                return memoryVector;
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
                byte stringLength = argsMemory[0];
                m_string_value = new List<byte>();
                for (byte i = 0; i < stringLength; i++)
                {
                    m_string_value.Add(argsMemory[i + MememoryOffset.OFFSET_1]);
                }

                return new List<INetworkType>(){
                    new MAP.NetString(Encoding.ASCII.GetString(m_string_value.ToArray()), "")
                };
            }

            public override List<INetworkType> Deserialize(byte[] argsMemory)
            {
                List<INetworkType> objectStructure = new List<INetworkType>();
                m_string_value = new List<byte>();
                var memoryTag = m_instance_name.Deserialize(argsMemory)[0];
                var memoryTagOffset = memoryTag.GetSize();
                var stringLength = argsMemory[memoryTagOffset + MememoryOffset.OFFSET_1];
                for (byte i = 0; i < stringLength; i++)
                {
                    m_string_value.Add(argsMemory[i + memoryTagOffset + MememoryOffset.OFFSET_2]);
                }

                objectStructure.Add(new MAP.NetString(Encoding.ASCII.GetString(m_string_value.ToArray()), memoryTag.GetName()));
                return objectStructure;
            }

            public override NetworkType GetNetworkType()
            {
                return NetworkType.STRING;
            }

            public override string GetName()
            {
                return m_instance_name.GetName();
            }

            public override int GetRawSize()
            {
               return m_string_value.Count + 1;//+1 for the length 
            }

            public override int GetSize()
            {
                return m_instance_name.GetSize() + GetRawSize() + 1; //1 is for type byte
            }

            public string GetValue()
            {
                return Encoding.ASCII.GetString(m_string_value.ToArray());
            }

            private MAP.MemoryTag m_instance_name;
            private List<byte> m_string_value;
        }
    }
}