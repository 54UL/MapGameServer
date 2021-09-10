using System.Collections.Generic;
using System;

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
			
			byte[] intByteArray = BitConverter.GetBytes(m_value);
			foreach (var intByte in intByteArray)
			{
				memoryVector.Add(intByte);
			}

			return memoryVector;
		}

		public override List<INetworkType> Deserialize(byte[] argsMemory)
		{
			List<INetworkType> objectStructure = new List<INetworkType>();
			var memoryTag = m_instance_name.Deserialize(argsMemory)[0];
			var intStartPos = memoryTag.GetSize() + MememoryOffset.OFFSET_1;

			m_value = BitConverter.ToInt32(argsMemory, intStartPos);
			
			objectStructure.Add(new MAP.NetInt(m_value, memoryTag.GetName()));
			return objectStructure;
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
			return m_instance_name.GetSize() + sizeof(int) + 1; //+1 for type byte
		}

		public int GetValue()
		{
			return m_value;
		}

		private int m_value;
		private MAP.MemoryTag m_instance_name = new MAP.MemoryTag();
	}
}

