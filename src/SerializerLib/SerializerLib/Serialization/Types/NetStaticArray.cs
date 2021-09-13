using System.Collections.Generic;
using System.Linq;

namespace SerializerLib
{
    namespace MAP
    {
        using NetworkObject = List<INetworkType>;

        public class NetStaticArray : INetworkType
        {
            public NetStaticArray()
            {
                this.m_instance_name = new MemoryTag("SYSTEM-STATIC-ARRAY");
            }

            public NetStaticArray(NetworkObject sequence,NetworkType contentType, string name)
            {
                this.m_instance_name = new MemoryTag(name);
                this.m_values = new NetworkObject(sequence);
                this.m_array_content_type = contentType;
            }

            public override List<byte> RawSerialization()
            {
                var arrayValuesVector = new List<byte>(); //TEMP
                foreach (var arrayValue in m_values)
                {
                    var memoryChunk = arrayValue.RawSerialization();
                    arrayValuesVector.AddRange(memoryChunk);
                }
                return arrayValuesVector;
            }

            public override List<byte> Serialize()
            {
                List<byte> memoryVector = new List<byte>();
                var memoryTagVector = m_instance_name.Serialize();
                memoryVector.Add((byte)GetNetworkType()); //TYPE
                memoryVector.AddRange(memoryTagVector); //MEMORY_TAG
                var arrayValuesVector = RawSerialization();
                memoryVector.Add((byte)(memoryVector.Count + arrayValuesVector.Count)); //LENGTH  IN BYTES
                memoryVector.AddRange(arrayValuesVector); //ARRAY_BINARY_VALUE/S with no type
                return memoryVector;
            }

            public List<INetworkType> StaticDeserialization(List<byte> memorySequence)
            {
                List<INetworkType> objectStructure = new List<INetworkType>();
                for (int memPos = 0; memPos < memorySequence.Count-1;)
                {
                    var currentDeserializedBytes = 0;
                    byte[] currentBytePayLoad = memorySequence.Skip(memPos).ToArray();
                    var currentDeserializedType = GlobalMembers.SerializerTypes.Get()[m_array_content_type].RawDeserialization(currentBytePayLoad);
                    foreach (var dtype in currentDeserializedType)
                    {
                        currentDeserializedBytes += dtype.GetRawSize();
                    }
                    objectStructure.AddRange(currentDeserializedType);
                    memPos += currentDeserializedBytes;
                }
                return objectStructure;
            }

            public override NetworkObject RawDeserialization(byte[] argsMemory)
            {
                throw new System.NotImplementedException();
            }

            public override List<INetworkType> Deserialize(byte[] argsMemory)
            {
                List<INetworkType> objectStructure = new List<INetworkType>();
                var memoryTag = m_instance_name.Deserialize(argsMemory)[0];
                var memoryTagOffset = memoryTag.GetSize();
                var arrayLength = argsMemory[memoryTagOffset + MememoryOffset.OFFSET_1];
                var startOffset = memoryTagOffset + MememoryOffset.OFFSET_2;
                var valuesMemoryVector = argsMemory.Skip(startOffset).Take(arrayLength - startOffset + 1).ToList();
                var decodedArrayVal = StaticDeserialization(valuesMemoryVector);
                m_values = decodedArrayVal;
                objectStructure.Add(new MAP.NetArray(m_values, memoryTag.GetName()));
                return objectStructure;
            }

            public override NetworkType GetNetworkType()
            {
                return NetworkType.STARRAY;
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
                    partialSize += arrayVal.GetRawSize();
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

            private NetworkType m_array_content_type;
            private NetworkObject m_values;
            private MAP.MemoryTag m_instance_name;
        }
    }
}