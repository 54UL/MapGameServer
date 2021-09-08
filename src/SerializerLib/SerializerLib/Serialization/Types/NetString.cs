using System;
using System.Collections.Generic;
using System.Text;

namespace MAP
{
    public class NetString : INetworkType
    {
        public NetString()
        {
            this.m_instance_name = new MAP.MemoryTag("STRING-SYSTEM");
        }

        public NetString(string value, string name)
        {
            this.m_instance_name = new MAP.MemoryTag(name);
            this.m_string_value = value;
        }

        public override List<byte> Serialize()
        {
            List<byte> memoryVector = new List<byte>(0);
            memoryVector.Add((byte)GetNetworkType());
            var memoryTagVector = m_instance_name.Serialize();
            memoryVector.AddRange(memoryTagVector);
            //INSERT STRING VALUE
            memoryVector.Add((byte)m_string_value.Length); //Length string
            foreach (var stringIterator in Encoding.ASCII.GetBytes(m_string_value)) //String data
            {
                memoryVector.Add((byte)stringIterator);
            }
            return memoryVector;
        }

        public override List<INetworkType> Deserialize(byte[] argsMemory)
        {
            
            List<INetworkType> objectStructure = new List<INetworkType>();
            var memoryTag = m_instance_name.Deserialize(argsMemory)[0];
            var memoryTagOffset = memoryTag.GetSize();
            var stringLength = argsMemory[memoryTagOffset + MememoryOffset.OFFSET_1];
            StringBuilder stringInMemory = new StringBuilder();
            for (byte i = 0; i < stringLength; i++)
            {
                stringInMemory.Append(argsMemory[i + memoryTagOffset + MememoryOffset.OFFSET_2]);
            }

            objectStructure.Add(new MAP.NetString(m_string_value, memoryTag.GetName()));
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

        public override int GetSize()
        {
            return m_instance_name.GetSize() + m_string_value.Length + 2; //+2 is for type byte and length byte
        }
        
        public string GetValue()
        {
            return m_string_value;
        }

        private MAP.MemoryTag m_instance_name;
        private string m_string_value = "";
    }
}
