#include "structures/queue/static_queue.h"

#include <gtest/gtest.h>

#include <iostream>

#include "data/data.h"

TEST(StaticQueueTest, QueueCapacity) {
    StaticQueue<int, 5> _testQueue;
    EXPECT_EQ(5, _testQueue.capacity());
}

TEST(StaticQueueTest, QueueSize) {
    StaticQueue<int, 5> _testQueue;
    EXPECT_EQ(0, _testQueue.size());
}

TEST(StaticQueueTest, QueueMoveQueue) {
    struct MyInt : public DataHeader {
    public:
        MyInt() = default;
        MyInt(uint32_t val) : DataHeader(), value(val) {}
        uint32_t value;
    };
    using Int = DataCore<MyInt>;
    StaticQueue<Int, 3> _intQueue;
    Int& value = _intQueue.next();
    MyInt m_int(333);
    value.setBytes((unsigned char*)&m_int, sizeof(MyInt));
    EXPECT_EQ(((struct MyInt*)value.data())->value, 333);
}

TEST(StaticQueueTest, QueuePush) {
    struct TestObj : public DataHeader {
        TestObj() = default;
        TestObj(int val) : testVar(val) {}
        uint32_t testVar;
    };
    using MyObj = DataCore<TestObj>;
    StaticQueue<MyObj, 3> _testQueue;
    for (uint8_t i = 0; i < _testQueue.capacity(); i++) {
        _testQueue.getData(i).memAllocate(sizeof(MyObj));
    }
    uint32_t value = 2;
    for (int i = 0; i < _testQueue.capacity(); i++) {
        MyObj& _myVar = _testQueue.next();
        TestObj _myObj(value *= 2);
        _myVar.setBytes((unsigned char*)&_myObj, sizeof(TestObj));
        // std::cout << ((struct TestObj*)_myVar.data())->testVar << std::endl;
        // std::cout << "Inserted: " << _testQueue.capacity() << " "
        //           << _testQueue.size() << std::endl;
        _testQueue.moveNext();
    }
    EXPECT_EQ(_testQueue.capacity(), _testQueue.size());
}