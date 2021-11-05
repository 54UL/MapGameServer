using System;
using System.Collections.Generic;
using System.Linq;

namespace SerializerLib
{
    namespace MAP
    {
        using NetworkObject = List<INetworkType>;
        public static class BinaryUtils
        {
            public static List<byte> Encode(List<INetworkType> sequence)
            {
                List<byte> m_raw_memory_packet = new List<byte>();
                //Evalua toda la sequencia y retorna el valor de memoria puro.
                foreach (var typeInstance in sequence)
                {
                    var serializedDataVector = typeInstance.Serialize();
                    m_raw_memory_packet.AddRange(serializedDataVector);
                }
                return new List<byte>(m_raw_memory_packet);
            }

            public static SortedDictionary<string, INetworkType> DecodeAsMap(byte[] bytes, int length)
            {
                SortedDictionary<string, INetworkType> objectStructure = new SortedDictionary<string, INetworkType>();

                for (int memPos = 0; memPos < length;)
                {
                    var currentDeserializedBytes = 0;
                    var commandValue = bytes[memPos];
                    var typeCode = (MAP.NetworkType)commandValue;
                    byte[] currentBytePayLoad = bytes.Skip(memPos).ToArray();
                    var currentDeserializedType = GlobalMembers.SerializerTypes.Get()[typeCode].Deserialize(currentBytePayLoad);

                    foreach (var dtype in currentDeserializedType)
                    {
                        objectStructure.Add(dtype.GetName(), dtype);
                        currentDeserializedBytes += dtype.GetSize();
                    }
                    memPos += currentDeserializedBytes;
                }
                return new SortedDictionary<string, INetworkType>(objectStructure);
            }

            public static List<INetworkType> Decode(List<byte> bytes)
            {
                List<INetworkType> objectStructure = new List<INetworkType>();
                var byteSequenceLength = bytes.Count - 1;

                for (int memPos = 0; memPos < byteSequenceLength;)
                {
                    var commandValue = bytes[memPos];
                    var typeCode = (MAP.NetworkType)commandValue;
                    var currentDeserializedBytes = 0;
                    byte[] currentBytePayLoad = bytes.Skip(memPos).ToArray();
                    var currentDeserializedType = GlobalMembers.SerializerTypes.Get()[typeCode].Deserialize(currentBytePayLoad);
                    foreach (var dtype in currentDeserializedType)
                    {
                        currentDeserializedBytes += dtype.GetSize();
                    }
                    objectStructure.AddRange(currentDeserializedType);
                    memPos += currentDeserializedBytes;
                }
                return new List<INetworkType>(objectStructure);
            }
        }
    }
}