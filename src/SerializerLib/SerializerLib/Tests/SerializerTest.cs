using System;
using System.Collections.Generic;

//C++ TO C# CONVERTER NOTE: The following #define macro was replaced in-line:
//ORIGINAL LINE: #define DLLEXPORT __declspec(dllexport)
//C++ TO C# CONVERTER NOTE: The following #define macro was replaced in-line:
//ORIGINAL LINE: #define DLLEXPORT __declspec(dllexport)

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
                new MAP.NetCommand(0x01, 102),
                new MAP.NetByte(42, "answer"),
                new MAP.NetByte(128, "halfbyte"),
                new MAP.NetByte(255, "fullbyte")};

            var binaryData = BinaryUtils.Encode(sequence);
            var deserialized = BinaryUtils.DecodeAsMap(binaryData.ToArray(), binaryData.Count - 1);

            var half = ((MAP.NetByte)deserialized["halfbyte"]).GetValue();
            var full = ((MAP.NetByte)deserialized["fullbyte"]).GetValue();
            var answer = ((MAP.NetByte)deserialized["answer"]).GetValue();


            return (half == 128 && full == 255 && answer == 42);
        }

        public bool DynamicTypeArrayTest()
        {
            NetworkObject sequence = new NetworkObject(){
                new MAP.NetByte(42, "byteVal"),
                new MAP.NetFloat(0.33f, "decimal"),
                new MAP.NetString("testing string lol","someString")};

            var arrayObj = new MAP.NetArray(sequence, "arrayTest");
            var serializedArrayVector = arrayObj.Serialize();
            var objMap = BinaryUtils.DecodeAsMap(serializedArrayVector.ToArray(), serializedArrayVector.Count - 1);

            var arr = ((MAP.NetArray)objMap["arrayTest"]).GetValues();
            var testbyte = ((MAP.NetByte)arr[0]).GetValue();
            var floatVal = ((MAP.NetFloat)arr[1]).GetValue();
            var stringVal =((MAP.NetString)arr[2]).GetValue();

            return testbyte == 42;
        }

        public bool StringTypeTest()
        {
            NetworkObject sequence = new NetworkObject(){
                new MAP.NetString("Hello world", "hello"),
                new MAP.NetString("This is a large text, can contain 255 characters", "longText")
                };
            var serializedStrings = BinaryUtils.Encode(sequence);
            var deserialized = BinaryUtils.DecodeAsMap(serializedStrings.ToArray(), serializedStrings.Count - 1);

            var hello = ((MAP.NetString)deserialized["hello"]).GetValue();
            var longText = ((MAP.NetString)deserialized["longText"]).GetValue();

            return hello.CompareTo("Hello world") == 0;
        }

        public bool FloatTypeTest()
        {
            NetworkObject sequence = new NetworkObject(){
                new MAP.NetFloat(0.006f, "tiny6"),
                new MAP.NetFloat(42.0f, "abc"),
                new MAP.NetFloat(128.0f, "half"),
                new MAP.NetFloat(255.0f, "full")};

            var serializedFloats = BinaryUtils.Encode(sequence);
            var deserialized = BinaryUtils.DecodeAsMap(serializedFloats.ToArray(), serializedFloats.Count - 1);

            var tinyVal = ((MAP.NetFloat)deserialized["tiny6"]).GetValue();
            var halfVal = ((MAP.NetFloat)deserialized["half"]).GetValue();

            return tinyVal == 0.006f && halfVal == 128.0f;
        }

        public bool IntTypeTest()
        {
            NetworkObject sequence = new NetworkObject(){
                new MAP.NetInt(777, "luck"),
                new MAP.NetInt(666, "hell"),
                new MAP.NetInt(12345678, "large")};

            var serializedInts = BinaryUtils.Encode(sequence);
            var deserialized = BinaryUtils.DecodeAsMap(serializedInts.ToArray(), serializedInts.Count - 1);
            var luck = ((MAP.NetInt)deserialized["luck"]).GetValue();
            var hell = ((MAP.NetInt)deserialized["hell"]).GetValue();
            var large = ((MAP.NetInt)deserialized["large"]).GetValue();

            return luck == 777 && hell == 666 && large == 12345678;
        }

        public bool ComplexArrayObjectTest()
        {
            NetworkObject poolData = new NetworkObject(){
                new MAP.NetInt(666, "PoolId"),
                new MAP.NetString("DEFAULT POOL", "PoolName")};

            NetworkObject commandPayload = new NetworkObject(){
            new MAP.NetArray(poolData, "PoolObj1"),
             new MAP.NetArray(poolData, "PoolObj2"),
             new MAP.NetArray(poolData, "PoolObj3"),
             new MAP.NetArray(poolData, "PoolObj4")};

            var serializedObject = BinaryUtils.Encode(commandPayload);
            var deserializedObjectMap = BinaryUtils.DecodeAsMap(serializedObject.ToArray(), serializedObject.Count - 1);
            //Get pool obj3
           
            var poolDataArr = ((MAP.NetArray)deserializedObjectMap["PoolObj3"]).GetValues();
            var poolId = ((MAP.NetInt)poolDataArr[0]).GetValue();
            var poolName = ((MAP.NetInt)poolDataArr[1]).GetValue();
            return true;
        }

        public bool AssigmentTest()
        {
            // //THIS IS A ASSIGMENT TEST WITHOUT OPERATOR OVERLOAD IN THE INTERFACE(TODO!!!)
            // SortedDictionary<char, MAP.INetworkType> testMap = new SortedDictionary<char, MAP.INetworkType>() { Tuple.Create("element1", new MAP.NetInt(666, "PoolId")), Tuple.Create("element2", new MAP.NetInt(666, "PoolId")) };
            // var someExternalNetworkType = new MAP.NetInt(33333, "PoolId");
            // testMap["element1"] = someExternalNetworkType;
            // int testMapVal = std::dynamic_pointer_cast<MAP.NetInt>(testMap["element1"]).GetValue();
            // return testMapVal == 33333;
            return true;
        }

        public bool Check()
        {
            bool allRight = false;
            allRight = ByteTypeTest();
            allRight = DynamicTypeArrayTest();
            allRight = StringTypeTest();
            allRight = FloatTypeTest();
            allRight = IntTypeTest();
            allRight = ComplexArrayObjectTest();
            allRight = AssigmentTest();
            return allRight;
        }
    }

}
