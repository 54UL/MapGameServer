using System.Collections.Generic;
using System.Linq;
using System;

namespace SerializerLib
{
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
                this.m_values = new NetworkObject(sequence);
            }

            public override List<byte> RawSerialization()
            {
                var arrayValuesVector = new List<byte>(); //TEMP
                foreach (var arrayValue in m_values)
                {
                    var memoryChunk = arrayValue.Serialize();
                    arrayValuesVector.InsertRange(arrayValuesVector.Count, memoryChunk);
                }
                return arrayValuesVector;
            }

            public override List<byte> Serialize()
            {
                List<byte> memoryVector = new List<byte>();
                var memoryTagVector = m_instance_name.Serialize();
                var arrayValuesVector = RawSerialization();
                memoryVector.Add((byte)GetNetworkType()); //TYPE
                memoryVector.AddRange(memoryTagVector); //MEMORY_TAG
                memoryVector.Add((byte)(memoryVector.Count + arrayValuesVector.Count)); //LENGTH  IN BYTES
                memoryVector.AddRange(arrayValuesVector); //ARRAY_BINARY_VALUE/S
                return memoryVector;
            }
            
            public override List<INetworkType> RawDeserialization(byte[] argsMemory)
            {
                NetworkObject objectStructure = new NetworkObject(){
                    new MAP.NetArray(BinaryUtils.Decode(argsMemory.ToList()),"NULL")
                };
                return objectStructure;
            }

            public override List<INetworkType> Deserialize(byte[] argsMemory)
            {
                NetworkObject objectStructure = new List<INetworkType>();
                var memoryTag = m_instance_name.Deserialize(argsMemory)[0];
                var memoryTagOffset = memoryTag.GetSize();

                var arrayLength = argsMemory[memoryTagOffset + MememoryOffset.OFFSET_1];
                var startOffset = memoryTagOffset + MememoryOffset.OFFSET_2;
                var valuesMemoryVector = argsMemory.Skip(startOffset).Take(arrayLength - startOffset + 1).ToList();

                var decodedArrayVal = BinaryUtils.Decode(valuesMemoryVector);
                m_values = decodedArrayVal;
                objectStructure.Add(new MAP.NetArray(m_values, memoryTag.GetName()));
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


            public override int GetRawSize()
            {
                int partialSize = 0;
                foreach (var arrayVal in m_values)
                {
                    partialSize += arrayVal.GetSize();
                }
                return partialSize;
            }

            public override int GetSize()
            {
                return m_instance_name.GetSize() + GetRawSize() + 2;
            }

            public NetworkObject GetValues()
            {
                return m_values;
            }
            public MAP.INetworkType At(int pos)
            {
                return m_values[(int)pos];
            }

            private NetworkObject m_values;
            private MAP.MemoryTag m_instance_name;
        }
    }
}