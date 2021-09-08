using System.Collections.Generic;

//C++ TO C# CONVERTER NOTE: The following #define macro was replaced in-line:
//ORIGINAL LINE: #define DLLEXPORT __declspec(dllexport)

namespace MAP
{
	public class NetInt : INetworkType
	{
		public NetInt()
		{
			this.m_value = 0;
			this.m_instance_name = new MAP.MemoryTag("INT-SYSTEM");
		}

		public NetInt(int value, string name)
		{
			this.m_value = value;
			this.m_instance_name = new MAP.MemoryTag(name);
		}

		public new void Dispose()
		{
			base.Dispose();
		}

		public override List<byte> Serialize()
		{
			List<byte> memoryVector = new List<byte>();
			memoryVector.Add((byte)GetNetworkType());
			var memoryTagVector = m_instance_name.Serialize();
			memoryVector.AddRange(memoryTagVector);
			var intSize = sizeof(int);
//C++ TO C# CONVERTER TODO TASK: There is no equivalent to 'reinterpret_cast' in C#:
			var intBytePtr = reinterpret_cast<byte>(m_value);
			for (byte findex = 0; findex < intSize; findex++)
			{
				memoryVector.Add(intBytePtr[findex]);
			}
			return new List<byte>(memoryVector);
		}

		public override List<INetworkType> Deserialize(byte[] argsMemory)
		{
			List<INetworkType> objectStructure = new List<INetworkType>();
			var memoryTag = m_instance_name.Deserialize(argsMemory)[0];
			var intStartPos = memoryTag.GetSize() + MememoryOffset.OFFSET_1;

//C++ TO C# CONVERTER TODO TASK: There is no equivalent to 'reinterpret_cast' in C#:
			m_value = *reinterpret_cast<const int>(argsMemory + intStartPos);

			objectStructure.Add(new MAP.NetInt(m_value, memoryTag.GetName()));
			return new List<INetworkType>(objectStructure);
		}

		public override NetworkType GetNetworkType()
		{
			return NetworkType.INT;
		}

		public override string GetName()
		{
			return m_instance_name.GetName();
		}

		public override int GetSize()
		{
			return m_instance_name.GetSize() + sizeof(int) + 1; //+1 for length byte
		}

		public int GetValue()
		{
			return m_value;
		}

		private int m_value;
		private MAP.MemoryTag m_instance_name = new MAP.MemoryTag();
	}
}

