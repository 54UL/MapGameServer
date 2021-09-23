using System;
using System.Collections.Generic;

namespace SerializerLib
{
    namespace MAP
    {
        using NetworkObject = List<INetworkType>;

        public class SerializerTest
        {
            public SerializerTest()
            {
            }

            public bool ByteTypeTest()
            {
                NetworkObject sequence = new NetworkObject(){
                new NetCommand(0x01, 102),
                new NetByte(42, "answer"),
                new NetByte(128, "halfbyte"),
                new NetByte(255, "fullbyte")};

                var binaryData = BinaryUtils.Encode(sequence);
                var deserialized = BinaryUtils.DecodeAsMap(binaryData.ToArray(), binaryData.Count - 1);

                var half = ((NetByte)deserialized["halfbyte"]).GetValue();
                var full = ((NetByte)deserialized["fullbyte"]).GetValue();
                var answer = ((NetByte)deserialized["answer"]).GetValue();


                return (half == 128 && full == 255 && answer == 42);
            }

            public bool DynamicTypeArrayTest()
            {
                NetworkObject sequence = new NetworkObject(){
                new NetByte(42, "byteVal"),
                new NetString("some string lol", "testStr"),
                new NetFloat(0.33f, "floating")};

                var arrayObj = new NetArray(sequence, "arrayTest");
                var serializedArrayVector = arrayObj.Serialize();
                var objMap = BinaryUtils.DecodeAsMap(serializedArrayVector.ToArray(), serializedArrayVector.Count - 1);

                var arr = ((NetArray)objMap["arrayTest"]).GetValues();
                var testbyte = ((NetByte)arr[0]).GetValue();
                var str = ((NetString)arr[1]).GetValue();
                var flt = ((NetFloat)arr[2]).GetValue();

                return testbyte == 42;
            }

            public bool StringTypeTest()
            {
                NetworkObject sequence = new NetworkObject(){
                new NetString("Hello world", "hello"),
                new NetString("This is a large text, can contain 255 characters", "longText")
                };
                var serializedStrings = BinaryUtils.Encode(sequence);
                var deserialized = BinaryUtils.DecodeAsMap(serializedStrings.ToArray(), serializedStrings.Count - 1);

                var hello = ((NetString)deserialized["hello"]).GetValue();
                var longText = ((NetString)deserialized["longText"]).GetValue();

                return hello.CompareTo("Hello world") == 0;
            }

            public bool FloatTypeTest()
            {
                NetworkObject sequence = new NetworkObject(){
                new NetFloat(0.006f, "tiny6"),
                new NetFloat(42.0f, "abc"),
                new NetFloat(128.0f, "half"),
                new NetFloat(255.0f, "full")};

                var serializedFloats = BinaryUtils.Encode(sequence);
                var deserialized = BinaryUtils.DecodeAsMap(serializedFloats.ToArray(), serializedFloats.Count - 1);

                var tinyVal = ((NetFloat)deserialized["tiny6"]).GetValue();
                var halfVal = ((NetFloat)deserialized["half"]).GetValue();

                return tinyVal == 0.006f && halfVal == 128.0f;
            }

            public bool IntTypeTest()
            {
                NetworkObject sequence = new NetworkObject(){
                new NetInt(777, "luck"),
                new NetInt(666, "hell"),
                new NetInt(12345678, "large")};

                var serializedInts = BinaryUtils.Encode(sequence);
                var deserialized = BinaryUtils.DecodeAsMap(serializedInts.ToArray(), serializedInts.Count - 1);
                var luck = ((NetInt)deserialized["luck"]).GetValue();
                var hell = ((NetInt)deserialized["hell"]).GetValue();
                var large = ((NetInt)deserialized["large"]).GetValue();

                return luck == 777 && hell == 666 && large == 12345678;
            }

            public bool ComplexArrayObjectTest()
            {
                NetworkObject poolData = new NetworkObject(){
                new NetInt(666, "PoolId"),
                new NetString("TESTING_POOL", "PoolName")};

                NetworkObject commandPayload = new NetworkObject(){
                new NetArray(poolData, "pool"),
                new NetArray(poolData, "pool2"),
                new NetArray(poolData, "pool3")
                };

                var serializedObject = BinaryUtils.Encode(commandPayload);
                var deserializedObjectMap = BinaryUtils.DecodeAsMap(serializedObject.ToArray(), serializedObject.Count - 1);


                var poolDataArr = ((NetArray)deserializedObjectMap["pool2"]).GetValues();
                var poolId = ((NetInt)poolDataArr[0]).GetValue();
                var poolName = ((NetString)poolDataArr[1]).GetValue();

                return poolId == 666 && poolName.CompareTo("TESTING_POOL") == 0;
            }

            public bool StaticTypeArrayTest()
            {
                // FLOAT TEST TYPE,first type cos i like floats
                NetworkObject sequence = new NetworkObject(){
                    new NetFloat(0.42f, ""),
                    new NetFloat(1.42f, ""),
                    new NetFloat(2.42f, "")
                    };

                var arrayObj = new NetStaticArray(sequence, NetworkType.FLOAT, "vector3f");
                var serializedArrayVector = arrayObj.Serialize();
                var objMap = BinaryUtils.DecodeAsMap(serializedArrayVector.ToArray(), serializedArrayVector.Count - 1);

                var arr = ((NetStaticArray)objMap["vector3f"]).GetValues();
                var f1 = ((NetFloat)arr[0]).GetValue();
                var f2 = ((NetFloat)arr[1]).GetValue();
                var f3 = ((NetFloat)arr[2]).GetValue();

                //STRING TYPE ARRAY TEST
                NetworkObject sequence2 = new NetworkObject(){
                    new NetString("AAAA", ""),
                    new NetString("BBBBbBBB", ""),
                    new NetString("CCCCcccccc", "")
                    };

                var arrayObj2 = new NetStaticArray(sequence2, NetworkType.STRING, "stringVector");
                var serializedArrayVector2 = arrayObj2.Serialize();
                var objMap2 = BinaryUtils.DecodeAsMap(serializedArrayVector2.ToArray(), serializedArrayVector2.Count - 1);

                var arr2 = ((NetStaticArray)objMap2["stringVector"]).GetValues();
                var s1 = ((NetString)arr2[0]).GetValue();
                var s2 = ((NetString)arr2[1]).GetValue();
                var s3 = ((NetString)arr2[2]).GetValue();

                //INT TYPE ARRAY TEST
                NetworkObject sequence3 = new NetworkObject(){
                    new NetInt(42, ""),
                    new NetInt(41, ""),
                    new NetInt(12345678, "")
                    };

                var arrayObj3 = new NetStaticArray(sequence3, NetworkType.INT, "intVector");
                var serializedArrayVector3 = arrayObj3.Serialize();
                var objMap3 = BinaryUtils.DecodeAsMap(serializedArrayVector3.ToArray(), serializedArrayVector3.Count - 1);

                var arr3 = ((NetStaticArray)objMap3["intVector"]).GetValues();
                var i1 = ((NetInt)arr3[0]).GetValue();
                var i2 = ((NetInt)arr3[1]).GetValue();
                var i3 = ((NetInt)arr3[2]).GetValue();

                //BYTE TYPE ARRAY TEST
                NetworkObject sequence4 = new NetworkObject(){
                    new NetByte(42, ""),
                    new NetByte(41, ""),
                    new NetByte(40, "")
                    };

                var arrayObj4 = new NetStaticArray(sequence4, NetworkType.BYTE, "byteVector");
                var serializedArrayVector4 = arrayObj4.Serialize();
                var objMap4 = BinaryUtils.DecodeAsMap(serializedArrayVector4.ToArray(), serializedArrayVector4.Count - 1);

                var arr4 = ((NetStaticArray)objMap4["byteVector"]).GetValues();
                var b1 = ((NetByte)arr4[0]).GetValue();
                var b2 = ((NetByte)arr4[1]).GetValue();
                var b3 = ((NetByte)arr4[2]).GetValue();

                //Dynamic type array ( MOST COMPLEX OBJECT UNTIL NOW... :> )
                NetworkObject somePair = new NetworkObject(){
                    new NetInt(123452, "id"),
                    new NetString("testing", "content")
                };

                NetworkObject pairArray = new NetworkObject(){
                    new NetArray(somePair, ""),
                    new NetArray(somePair, ""),
                    new NetArray(somePair, "")
                };

                var arrayObj5 = new NetStaticArray(pairArray, NetworkType.ARRAY, "dynamicArrayVector");
                var serializedArrayVector5 = arrayObj5.Serialize();
                var objMap5 = BinaryUtils.DecodeAsMap(serializedArrayVector5.ToArray(), serializedArrayVector5.Count - 1);

                var arr5 = ((NetStaticArray)objMap5["dynamicArrayVector"]).GetValues();
                var a1 = ((NetArray)arr5[0]).GetValues();
                var a2 = ((NetArray)arr5[1]).GetValues();
                var a3 = ((NetArray)arr5[2]).GetValues();

                return true;
            }

            public bool Check()
            {
                bool allRight = false;
                // allRight = ByteTypeTest();
                // allRight = DynamicTypeArrayTest();
                // allRight = StringTypeTest();
                // allRight = FloatTypeTest();
                // allRight = IntTypeTest();
                // allRight = ComplexArrayObjectTest();
                allRight = StaticTypeArrayTest();
                return allRight;
            }
        }

    }
}