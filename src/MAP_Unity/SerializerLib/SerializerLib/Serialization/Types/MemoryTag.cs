using System;
using System.Collections.Generic;
using System.Text;

namespace SerializerLib
{
	namespace MAP
	{
		//Internal serializer class, don't use outside
		using NetworkObject = List<INetworkType>;
		
		public class MemoryTag : INetworkType
		{
			public MemoryTag()
			{
				this.m_instance_name = new List<byte>(Encoding.UTF8.GetBytes("MEMORY-TAG-SYSTEM"));
			}

			public MemoryTag(string name)
			{
				this.m_instance_name = new List<byte>(Encoding.UTF8.GetBytes(name));
			}

			public override List<byte> RawSerialization()
            {
				List<byte> memoryVector = new List<byte>();
				//NOTE: MEMORY TAG DOES NOT HAVE TO PASS HIS TYPE ID (ASSUMPTION DUE TO COMPOSITION)
				memoryVector.Add((byte)(m_instance_name.Count)); //CHANGE THIS CAST of size_t by byte
				foreach (var stringIterator in m_instance_name)
				{
					memoryVector.Add((stringIterator));
				}
				return memoryVector;
			}
			
			public override List<byte> Serialize()
			{
				return RawSerialization();//just the same due to memory tag desing...
			}
			
			
			public override NetworkObject RawDeserialization(byte[] argsMemory)
			{
				byte tagLength = argsMemory[MememoryOffset.OFFSET_1]; //length first pos
				m_instance_name = new List<byte>();
				for (byte i = 0; i < tagLength; i++)
				{
					m_instance_name.Add(argsMemory[MememoryOffset.OFFSET_2 + i]);
				}

				List<INetworkType> objectStructure = new List<INetworkType>
				{
					new MemoryTag(Encoding.ASCII.GetString(m_instance_name.ToArray()))
				};
				return objectStructure;
			}

			public override List<INetworkType> Deserialize(byte[] argsMemory)
			{
				return RawDeserialization(argsMemory);
			}

			public override NetworkType GetNetworkType()
			{
				return NetworkType.TAG;
			}

			public override string GetName()
			{
				return Encoding.ASCII.GetString(m_instance_name.ToArray());
			}

            public override int GetRawSize()
            {
                return m_instance_name.Count + 1;//1 byte extra for the size byte
            }

            public override int GetSize()
            {
                return GetRawSize();
            }

			private List<byte> m_instance_name;
		}
	}
}