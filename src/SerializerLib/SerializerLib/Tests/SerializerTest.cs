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
            var deserialized = BinaryUtils.DecodeAsMap(binaryData.ToArray(), binaryData.Count);

            var half = BinaryUtils.Get<MAP.NetByte>(deserialized, "halfbyte");
            var full = BinaryUtils.Get<MAP.NetByte>(deserialized, "fullbyte");
            var answer = BinaryUtils.Get<MAP.NetByte>(deserialized, "answer");

            var test1 = half.GetValue();
            var test2 = full.GetValue();
            var test3 = answer.GetValue();

            return (test1 == 128 && test2 == 255 && test3 == 42);
        }

        public bool DynamicTypeArrayTest()
        {
            NetworkObject sequence = new NetworkObject(){
                new MAP.NetByte(42, "byteVal"),
                new MAP.NetFloat(0.33f, "decimal"),
                new MAP.NetString("testing string lol","someString")};

            var arrayObj = new MAP.NetArray(sequence, "arrayTest");
            var serializedArrayVector = arrayObj.Serialize();
            var objMap = BinaryUtils.DecodeAsMap(serializedArrayVector.ToArray(), serializedArrayVector.Count);

            var arr = BinaryUtils.Get<MAP.NetArray>(objMap, "arrayTest").GetValues();

            var testbyte = BinaryUtils.Get<MAP.NetByte>(arr, "byteVal").GetValue();
            var floatVal = BinaryUtils.Get<MAP.NetFloat>(arr, "decimal").GetValue();
            var stringVal = BinaryUtils.Get<MAP.NetString>(arr, "someString").GetValue();

            return testbyte == 42;
        }

        public bool StringTypeTest()
        {
            NetworkObject sequence = new NetworkObject(){
                new MAP.NetString("Hello world", "hello"),
                new MAP.NetString("This is a large text, can contain 255 characters", "longText")
                };
            var serializedStrings = BinaryUtils.Encode(sequence);
            var deserialized = BinaryUtils.DecodeAsMap(serializedStrings.ToArray(), serializedStrings.Count);
            var hello = BinaryUtils.Get<MAP.NetString>(deserialized, "hello");
            var longText = BinaryUtils.Get<MAP.NetString>(deserialized, "longText");
            return hello.GetValue().CompareTo("Hello world") == 0;
        }

        public bool FloatTypeTest()
        {
            NetworkObject sequence = new NetworkObject(){
                new MAP.NetFloat(0.006f, "tiny6"),
                new MAP.NetFloat(42.0f, "abc"),
                new MAP.NetFloat(128.0f, "half"),
                new MAP.NetFloat(255.0f, "full")};

            var serializedFloats = BinaryUtils.Encode(sequence);
            var deserialized = BinaryUtils.DecodeAsMap(serializedFloats.ToArray(), serializedFloats.Count);

            var tinyVal = BinaryUtils.Get<MAP.NetFloat>(deserialized, "tiny6");
            var halfVal = BinaryUtils.Get<MAP.NetFloat>(deserialized, "half");

            float val = tinyVal.GetValue();
            float val2 = halfVal.GetValue();
            return val == 0.006f;
        }

        public bool IntTypeTest()
        {
            NetworkObject sequence = new NetworkObject(){
                new MAP.NetInt(777, "luck"),
                new MAP.NetInt(666, "hell"),
                new MAP.NetInt(12345678, "large")};

            var serializedInts = BinaryUtils.Encode(sequence);
            var deserialized = BinaryUtils.DecodeAsMap(serializedInts.ToArray(), serializedInts.Count);

            var luck = BinaryUtils.Get<MAP.NetInt>(deserialized, "luck");
            var hell = BinaryUtils.Get<MAP.NetInt>(deserialized, "hell");
            var large = BinaryUtils.Get<MAP.NetInt>(deserialized, "large");

            int val = luck.GetValue();
            int val2 = hell.GetValue();
            int val3 = large.GetValue();

            return val == 777 && val2 == 666 && val3 == 12345678;
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
            var deserializedObjectMap = BinaryUtils.DecodeAsMap(serializedObject.ToArray(), serializedObject.Count);
            //Get pool obj3
            var poolDataArr = BinaryUtils.Get<MAP.NetArray>(deserializedObjectMap, "PoolObj3").GetValues();
            var poolId = BinaryUtils.Get<MAP.NetInt>(poolDataArr, "PoolId").GetValue();
            var poolName = BinaryUtils.Get<MAP.NetString>(poolDataArr, "PoolName").GetValue();
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
