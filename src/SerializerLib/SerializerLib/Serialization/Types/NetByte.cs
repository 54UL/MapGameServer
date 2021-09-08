using System.Collections.Generic;

namespace MAP
{
    public class NetByte : INetworkType
    {
        public NetByte()
        {
            this.m_value = 0;
        }

        public NetByte(byte value)
        {
            this.m_value = value;
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

        public override List<byte> Serialize()
        {
            List<byte> memoryVector = new List<byte>();
            memoryVector.Add((byte)GetNetworkType());
            var memoryTagVector = m_instance_name.Serialize();
            memoryVector.AddRange(memoryTagVector);
            memoryVector.Add(m_value);
            return memoryVector;
        }

        public override List<INetworkType> Deserialize(byte[] argsMemory)
        {
            List<INetworkType> objectStructure = new List<INetworkType>();
            var memoryTag = m_instance_name.Deserialize(argsMemory)[0];
            objectStructure.Add(new MAP.NetByte(argsMemory[memoryTag.GetSize() + MememoryOffset.OFFSET_1], memoryTag.GetName()));
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

        public override int GetSize()
        {
            return m_instance_name.GetSize() + sizeof(byte) +1; //+1 for length byte
        }

        public byte GetValue()
        {
            return m_value;
        }

        private byte m_value = new byte();
        private MAP.MemoryTag m_instance_name = new MAP.MemoryTag();
    }
}

