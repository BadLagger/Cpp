#include "bcp2classtests.h"

void bcp2ClassTests::angleOrientationFromData()
{
    QByteArray       testData;

    double           weekTime    = 2.58;
    testData.append(reinterpret_cast<const char*>(&weekTime), sizeof(weekTime));
    short            gpsWeekNum = 5;
    testData.append(reinterpret_cast<const char*>(&gpsWeekNum), sizeof(gpsWeekNum));
    float            course = -1.05f;
    testData.append(reinterpret_cast<const char*>(&course), sizeof(course));
    float            pitch = -25.05f;
    testData.append(reinterpret_cast<const char*>(&pitch), sizeof(pitch));
    float            roll = 10.85f;
    testData.append(reinterpret_cast<const char*>(&roll), sizeof(roll));
    float            courseRms = 0.36f;
    testData.append(reinterpret_cast<const char*>(&courseRms), sizeof(courseRms));
    float            pitchRms = 4.98f;
    testData.append(reinterpret_cast<const char*>(&pitchRms), sizeof(pitchRms));
    float            rollRms = 6.98f;
    testData.append(reinterpret_cast<const char*>(&rollRms), sizeof(rollRms));
    unsigned char    status = 68;
    testData.append(reinterpret_cast<const char*>(&status), sizeof(status));

    ANGLEORIENTATION testResult = getAngleOrientationFromData(testData);

    QCOMPARE(testResult.weekTime, weekTime);
    QCOMPARE(testResult.gpsWeekNum, gpsWeekNum);
    QCOMPARE(testResult.course, course);
    QCOMPARE(testResult.pitch, pitch);
    QCOMPARE(testResult.roll, roll);
    QCOMPARE(testResult.courseRms, courseRms);
    QCOMPARE(testResult.pitchRms, pitchRms);
    QCOMPARE(testResult.rollRms, rollRms);
    QCOMPARE(testResult.status, status);
}

void bcp2ClassTests::shortFromData()
{
    short a = 1589;
    QCOMPARE(shortFromByteArray(QByteArray(reinterpret_cast<const char*>(&a), sizeof(a))), a);
}

void bcp2ClassTests::ulongFromData()
{
    unsigned long a = 36987456;
    QCOMPARE(uLongFromByteArray(QByteArray(reinterpret_cast<const char*>(&a), sizeof(a))), a);
}

void bcp2ClassTests::floatFromData()
{
    float a = 15.8976f;
    QCOMPARE(floatFromByteArray(QByteArray(reinterpret_cast<const char*>(&a), sizeof(a))), a);
}

void bcp2ClassTests::doubleFromData()
{
    double a = 15.8976;
    QCOMPARE(doubleFromByteArray(QByteArray(reinterpret_cast<const char*>(&a), sizeof(a))), a);
}

