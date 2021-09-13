using System.Collections.Generic;

namespace SerializerLib
{
    namespace MAP
    {

        public class MememoryOffset
        {
            public const byte OFFSET_1 = 1;
            public const byte OFFSET_2 = 2;
            public const byte OFFSET_3 = 3;
            public const byte OFFSET_4 = 4;
        }

        public enum NetworkType : byte
        {
            TAG = 0x01,
            COMMAND = 0x02,
            STRING = 0x03,
            FLOAT = 0x04,
            ARRAY = 0x05,
            STARRAY = 0x06,
            BYTE = 0x07,
            INT = 0x08,
            UINT = 0x09,
            BLOB = 0x0A
        }

        public abstract class INetworkType : System.IDisposable
        {
            public INetworkType()
            {
            }
            public virtual void Dispose()
            {
            }
            public abstract List<byte> RawSerialization();
            public abstract List<byte> Serialize();
            public abstract List<INetworkType> RawDeserialization(byte[] argsMemory);
            public abstract List<INetworkType> Deserialize(byte[] argsMemory);
            public abstract string GetName();
            public abstract NetworkType GetNetworkType();
            public abstract int GetRawSize();
            public abstract int GetSize();

        }

    }
}