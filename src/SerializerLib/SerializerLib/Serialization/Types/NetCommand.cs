using System.Collections.Generic;
using System;

namespace SerializerLib
{
    namespace MAP
    {
        public class NetCommand : INetworkType
        {
            public NetCommand()
            {
                this.m_command_id = 0;
                m_client_id = 255;
            }

            public NetCommand(byte commandId, byte clientId)
            {
                this.m_command_id = commandId;
                m_client_id = clientId;
            }

            public new void Dispose()
            {
                base.Dispose();
            }

            public override List<byte> RawSerialization()
            {
                return new List<byte>()
                {
                    m_command_id,
                    m_client_id
                };
            }

            public override List<byte> Serialize()
            {
                List<byte> memoryVector = new List<byte>(0);
                memoryVector.Add((byte)GetNetworkType());
                memoryVector.InsertRange(memoryVector.Count, RawSerialization());
                return memoryVector;
            }

            public override List<INetworkType> RawDeserialization(byte[] argsMemory)
            {
                throw new NotImplementedException();
            }

            public override List<INetworkType> Deserialize(byte[] argsMemory)
            {
                var commandId = argsMemory[MememoryOffset.OFFSET_1];
                var clientId = argsMemory[MememoryOffset.OFFSET_2];
                List<MAP.INetworkType> objectStructure = new List<MAP.INetworkType>();
                objectStructure.Add(new NetCommand(commandId, clientId));
                return objectStructure;
            }

            public override string GetName()
            {
                return "COMMAND";
            }

            public override NetworkType GetNetworkType()
            {
                return NetworkType.COMMAND;
            }

            public override int GetRawSize()
            {
                return sizeof(byte) + sizeof(byte);
            }

            public override int GetSize()
            {
                return GetRawSize() + 1; //+1 for the type byte
            }

            public byte Id()
            {
                return m_command_id;
            }

            public byte ClientId()
            {
                return m_client_id;
            }

            private byte m_command_id;
            private byte m_client_id;
        }
    }
}