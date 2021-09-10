using System.Collections.Generic;
using System.Linq;

namespace MAP
{
    using NetworkObject = List<INetworkType>;

    public class NetArray : INetworkType
    {
        public NetArray()
        {

            this.m_instance_name = new MemoryTag("SYSTEM-ARRAY");

        }
        public NetArray(NetworkObject sequence, string name)
        {
            this.m_instance_name = new MemoryTag(name);
            this.m_values = sequence;
        }


        public override List<byte> Serialize()
        {
            List<byte> memoryVector = new List<byte>();
            memoryVector.Add((byte)GetNetworkType()); //TYPE
            var memoryTagVector = m_instance_name.Serialize();
            memoryVector.AddRange(memoryTagVector); //MEMORY_TAG
            var arrayValuesVector = new List<byte>(); //TEMP
            foreach (var arrayValue in m_values)
            {
                var memoryChunk = arrayValue.Serialize();
                arrayValuesVector.InsertRange(arrayValuesVector.Count,memoryChunk);
            }
            memoryVector.Add((byte)(memoryVector.Count + arrayValuesVector.Count )); //LENGTH  IN BYTES
            memoryVector.AddRange(arrayValuesVector); //ARRAY_BINARY_VALUE/S
            return memoryVector;
        }

        public override List<INetworkType> Deserialize(byte[] argsMemory)
        {
            List<INetworkType> objectStructure = new List<INetworkType>();
            var memoryTag = m_instance_name.Deserialize(argsMemory)[0];
            var memoryTagOffset = memoryTag.GetSize();
            var arrayLength = argsMemory[memoryTagOffset  + MememoryOffset.OFFSET_1];
            var startOffset = memoryTagOffset  + MememoryOffset.OFFSET_2;
            var diff = arrayLength - startOffset;
            var valuesMemoryVector = argsMemory.Skip(startOffset).Take(diff).ToList();

            var decodedArrayVal = BinaryUtils.Decode(valuesMemoryVector);
            objectStructure.Add(new MAP.NetArray(decodedArrayVal, memoryTag.GetName()));
            return objectStructure;
        }

        public override NetworkType GetNetworkType()
        {
            return NetworkType.ARRAY;
        }

        public override string GetName()
        {
            return m_instance_name.GetName();
        }

        public override int GetSize()
        {
            int partialSize = 0;
            foreach (var arrayVal in m_values)
            {
                partialSize += arrayVal.GetSize();
            }
            return m_instance_name.GetSize() + partialSize + 2;
        }
        public NetworkObject GetValues()
        {
            return m_values;
        }
        public MAP.INetworkType At(int pos)
        {
            return m_values[(int)pos];
        }
        private NetworkObject m_values = new NetworkObject();
        private MAP.MemoryTag m_instance_name;
    }
}
