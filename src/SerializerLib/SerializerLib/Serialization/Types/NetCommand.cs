using System.Collections.Generic;

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

        public override List<byte> Serialize()
        {
            List<byte> memoryVector = new List<byte>(0);
            memoryVector.Add((byte)GetNetworkType());
            memoryVector.Add(m_command_id);
            memoryVector.Add(m_client_id);
            return memoryVector;
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

        public override int GetSize()
        {
            return sizeof(byte) + sizeof(byte) + 1; //+1 for the type byte
        }

        public byte id()
        {
            return m_command_id;
        }

        public byte clientId()
        {
            return m_client_id;
        }

        private byte m_command_id = new byte();
        private byte m_client_id = new byte();
    }
}
