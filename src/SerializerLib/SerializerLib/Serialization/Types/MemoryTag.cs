using System.Collections.Generic;

//C++ TO C# CONVERTER NOTE: The following #define macro was replaced in-line:
//ORIGINAL LINE: #define DLLEXPORT __declspec(dllexport)

namespace MAP
{
	//Internal serializer class, don't use outside
	public class MemoryTag : INetworkType
	{
		public MemoryTag()
		{
			this.m_instance_name = "MEMORY-TAG-SYSTEM";
		}

		public MemoryTag(string name)
		{
			this.m_instance_name = name;
		}

		public new void Dispose()
		{
			base.Dispose();
		}

		public override List<byte> Serialize()
		{
			List<byte> memoryVector = new List<byte>(0);
			//NOTE: MEMORY TAG DOES NOT HAVE TO PASS HIS TYPE ID (ASSUMPTION DUE TO COMPOSITION)
			memoryVector.Add((byte)m_instance_name.Length); //CHANGE THIS CAST of size_t by byte
			foreach (var stringIterator in m_instance_name)
			{
				memoryVector.Add(((byte)stringIterator));
			}
			return new List<byte>(memoryVector);
		}

		public override List<INetworkType> Deserialize(byte[] argsMemory)
		{
			byte tagLength = argsMemory[MememoryOffset.OFFSET_1]; //lenght first pos
			m_instance_name = "";
			for (byte i = 0; i < tagLength; i++)
			{
				m_instance_name. (argsMemory[MememoryOffset.OFFSET_2 + i]);
			}
			List<INetworkType> objectStructure = new List<INetworkType>();
			objectStructure.Add(new MAP.MemoryTag(m_instance_name));
			return new List<INetworkType>(objectStructure);
		}

		public override NetworkType GetNetworkType()
		{
			return NetworkType.TAG;
		}

		public override string GetName()
		{
			return m_instance_name;
		}

		public override int GetSize()
		{
			return m_instance_name.Length + 1; //1 byte extra for the size byte
		}

		private string m_instance_name = "";
	}
}

