#include "DeviceDriver.h"
#include "FlashMemoryDevice.h"
#include "gmock/gmock.h"

using ::testing::Return;

class FlashMock : public FlashMemoryDevice {
public:
    MOCK_METHOD(unsigned char, read, (long address), (override));
    MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

TEST(DD, Read시5회호출) {
    FlashMock mock;
    EXPECT_CALL(mock, read(0xA))
        .Times(5);

    DeviceDriver dd{ &mock };
    dd.read(0xA);
}

TEST(DD, 5회Read시모두같은값이올라오지않으면ReadFailException) {
    FlashMock mock;
    EXPECT_CALL(mock, read(0xA))
        .WillOnce(Return(0xB))
        .WillOnce(Return(0xB))
        .WillOnce(Return(0xB))
        .WillOnce(Return(0xB))
        .WillOnce(Return(0xC));

    DeviceDriver dd{ &mock };
    try {
        dd.read(0xA);
        FAIL();
    }
    catch (const ReadFailException& e) {
        EXPECT_STREQ(e.what(), "Read Values are not same");
    }
}

TEST(DD, 데이터가이미있는상태에서Write하는경우WriteFailException) {
    FlashMock mock;
    DeviceDriver dd{ &mock };

    EXPECT_CALL(mock, read(0xA))
        .WillOnce(Return(0xA));

    try {
        dd.write(0xA, 0xB);        
        FAIL();
    }
    catch (const WriteFailException& e) {
        EXPECT_STREQ(e.what(), "data is already written");
    }
}

TEST(APP, 읽고출력) {
    FlashMock mock;
    DeviceDriver dd{ &mock };
    Application app{ &dd };

    const long startAddr = 0x00;
    const long endAddr = 0x30;

    std::string result = "";
    for (long addr = startAddr; addr <= endAddr; addr++) {
        unsigned char pat = rand() % 0xFF;
        EXPECT_CALL(mock, read(addr))
            .Times(5)
            .WillRepeatedly(Return(pat));
    }

    app.readAndPrint(startAddr, endAddr);
}

TEST(APP, 모두Write) {
    FlashMock mock;
    DeviceDriver dd{ &mock };
    Application app{ &dd };

    const long startAddr = 0x00;
    const long endAddr = 0x04;

    unsigned char pat = rand() % 0xFF;
    for (long addr = startAddr; addr <= endAddr; addr++) {
        EXPECT_CALL(mock, read(addr))
            .Times(1)
            .WillRepeatedly(Return(0xFF));
        EXPECT_CALL(mock, write(addr, pat))
            .Times(1);
    }

    app.writeall(pat);
}

int main() {
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}