using System.Collections.Generic;
namespace SerializerLib
{
	namespace MAP
	{
		public class TypesManager : System.IDisposable
		{
			public TypesManager()
			{
				Initialize();
			}

			public void Dispose()
			{
				Clean();
			}

			public void Initialize()
			{
				m_serializer_network_types.Add(MAP.NetworkType.TAG, new MAP.MemoryTag());
				m_serializer_network_types.Add(MAP.NetworkType.COMMAND, new MAP.NetCommand());
				m_serializer_network_types.Add(MAP.NetworkType.BYTE, new MAP.NetByte());
				m_serializer_network_types.Add(MAP.NetworkType.ARRAY, new MAP.NetArray());
				m_serializer_network_types.Add(MAP.NetworkType.STRING, new MAP.NetString());
				m_serializer_network_types.Add(MAP.NetworkType.FLOAT, new MAP.NetFloat());
				m_serializer_network_types.Add(MAP.NetworkType.INT, new MAP.NetInt());
			}

			public void Clean()
			{
				m_serializer_network_types.Clear();
			}

			public SortedDictionary<MAP.NetworkType, INetworkType> Get()
			{
				return new SortedDictionary<MAP.NetworkType, INetworkType>(m_serializer_network_types);
			}

			private SortedDictionary<MAP.NetworkType, INetworkType> m_serializer_network_types = new SortedDictionary<MAP.NetworkType, INetworkType>();
		}
	}
}