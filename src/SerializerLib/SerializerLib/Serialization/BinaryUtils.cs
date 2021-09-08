using System;
using System.Collections.Generic;

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

                var currentDeserializedType = GlobalMembers.SerializerTypes.Get()[typeCode].Deserialize(bytes + memPos);

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
            var byteSequenceLength = bytes.Count;

            for (int memPos = 0; memPos < byteSequenceLength;)
            {
                var commandValue = bytes[memPos];
                var typeCode = (MAP.NetworkType)commandValue;
                var currentDeserializedBytes = 0;

                var currentDeserializedType = GlobalMembers.SerializerTypes.Get()[typeCode].Deserialize(bytes[memPos]);
                foreach (var dtype in currentDeserializedType)
                {
                    currentDeserializedBytes += dtype.GetSize();
                }
                objectStructure.AddRange(currentDeserializedType);
                memPos += currentDeserializedBytes;
            }
            return new List<INetworkType>(objectStructure);
        }

        internal static T Get<T>(SortedDictionary<string, MAP.INetworkType> sequence, string name)
        {
            return (T)(sequence[name]);
        }

        internal static T Get<T>(NetworkObject sequence, string name)
        {
            //C++ TO C# CONVERTER TODO TASK: Lambda expressions cannot be assigned to 'var':
           var findedElement =  sequence.Find((val)=>{
                return val.GetName().CompareTo(name) == 0;
            });

            return (T)(findedElement);
        }
    }
}