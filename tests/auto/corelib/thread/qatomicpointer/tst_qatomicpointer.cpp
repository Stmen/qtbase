/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtTest/QtTest>

#include <qatomic.h>
#include <limits.h>

//TESTED_CLASS=
//TESTED_FILES=

class tst_QAtomicPointer : public QObject
{
    Q_OBJECT
private slots:
    void warningFree();

    void constructor();
    void copy_constructor();
    void equality_operator();
    void inequality_operator();
    void assignment_operator();
    void star_operator();
    void dereference_operator();

    void isTestAndSetNative();
    void isTestAndSetWaitFree();
    void testAndSet();

    void isFetchAndStoreNative();
    void isFetchAndStoreWaitFree();
    void fetchAndStore();

    void isFetchAndAddNative();
    void isFetchAndAddWaitFree();
    void fetchAndAdd_data();
    void fetchAndAdd();

private:
    static void warningFreeHelper();
};

struct WFHC
{
    void bar() {}
};

void tst_QAtomicPointer::warningFreeHelper()
{
    qFatal("This code is bogus, and shouldn't be run. We're looking for compiler warnings only.");

    QBasicAtomicPointer<WFHC> p = Q_BASIC_ATOMIC_INITIALIZER(0);

    p.load()->bar();

    WFHC *expectedValue = 0;
    WFHC *newValue = 0;
    qptrdiff valueToAdd = 0;

    p.testAndSetRelaxed(expectedValue, newValue);
    p.testAndSetAcquire(expectedValue, newValue);
    p.testAndSetRelease(expectedValue, newValue);
    p.testAndSetOrdered(expectedValue, newValue);

    p.fetchAndStoreRelaxed(newValue);
    p.fetchAndStoreAcquire(newValue);
    p.fetchAndStoreRelease(newValue);
    p.fetchAndStoreOrdered(newValue);

    p.fetchAndAddRelaxed(valueToAdd);
    p.fetchAndAddAcquire(valueToAdd);
    p.fetchAndAddRelease(valueToAdd);
    p.fetchAndAddOrdered(valueToAdd);
}

void tst_QAtomicPointer::warningFree()
{
    // This is a compile time check for warnings.
    // No need for actual work here.

    void (*foo)() = &warningFreeHelper;
    (void)foo;
}

void tst_QAtomicPointer::constructor()
{
    void *one = this;
    QAtomicPointer<void> atomic1 = one;
    QVERIFY(atomic1.load() == one);

    void *two = &one;
    QAtomicPointer<void> atomic2 = two;
    QVERIFY(atomic2.load() == two);

    void *three = &two;
    QAtomicPointer<void> atomic3 = three;
    QVERIFY(atomic3.load() == three);
}

void tst_QAtomicPointer::copy_constructor()
{
    void *one = this;
    QAtomicPointer<void> atomic1 = one;
    QAtomicPointer<void> atomic1_copy = atomic1;
    QVERIFY(atomic1_copy.load() == one);
    QCOMPARE(atomic1_copy.load(), atomic1.load());

    void *two = &one;
    QAtomicPointer<void> atomic2 = two;
    QAtomicPointer<void> atomic2_copy = atomic2;
    QVERIFY(atomic2_copy.load() == two);
    QCOMPARE(atomic2_copy.load(), atomic2.load());

    void *three = &two;
    QAtomicPointer<void> atomic3 = three;
    QAtomicPointer<void> atomic3_copy = atomic3;
    QVERIFY(atomic3_copy.load() == three);
    QCOMPARE(atomic3_copy.load(), atomic3.load());
}

void tst_QAtomicPointer::equality_operator()
{
    void *one = this;
    void *two = &one;
    void *three = &two;

    QAtomicPointer<void> atomic1 = one;
    QAtomicPointer<void> atomic2 = two;
    QAtomicPointer<void> atomic3 = three;

    QVERIFY(atomic1 == one);
    QVERIFY(!(atomic1 == two));
    QVERIFY(!(atomic1 == three));

    QVERIFY(!(atomic2 == one));
    QVERIFY(atomic2 == two);
    QVERIFY(!(atomic2 == three));

    QVERIFY(!(atomic3 == one));
    QVERIFY(!(atomic3 == two));
    QVERIFY(atomic3 == three);
}

void tst_QAtomicPointer::inequality_operator()
{
    void *one = this;
    void *two = &one;
    void *three = &two;

    QAtomicPointer<void> atomic1 = one;
    QAtomicPointer<void> atomic2 = two;
    QAtomicPointer<void> atomic3 = three;

    QVERIFY(!(atomic1 != one));
    QVERIFY(atomic1 != two);
    QVERIFY(atomic1 != three);

    QVERIFY(atomic2 != one);
    QVERIFY(!(atomic2 != two));
    QVERIFY(atomic2 != three);

    QVERIFY(atomic3 != one);
    QVERIFY(atomic3 != two);
    QVERIFY(!(atomic3 != three));
}

void tst_QAtomicPointer::assignment_operator()
{
    void *one = this;
    void *two = &one;
    void *three = &two;

    QAtomicPointer<void> atomic1 = one;
    QAtomicPointer<void> atomic2 = two;
    QAtomicPointer<void> atomic3 = three;

    QVERIFY(atomic1 == one);
    QVERIFY(atomic2 == two);
    QVERIFY(atomic3 == three);

    atomic1 = two;
    atomic2 = three;
    atomic3 = one;

    QVERIFY(atomic1 == two);
    QVERIFY(atomic2 == three);
    QVERIFY(atomic3 == one);
}

struct Type
{
    inline const Type *self() const
    { return this; }
};

void tst_QAtomicPointer::star_operator()
{
    Type t;
    QAtomicPointer<Type> p = &t;
    QCOMPARE((*p).self(), t.self());
}

void tst_QAtomicPointer::dereference_operator()
{
    Type t;
    QAtomicPointer<Type> p = &t;
    QCOMPARE(p->self(), t.self());
}

void tst_QAtomicPointer::isTestAndSetNative()
{
#if defined(Q_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE)
    // the runtime test should say the same thing
    QVERIFY(QAtomicPointer<void>::isTestAndSetNative());

#  if (defined(Q_ATOMIC_POINTER_TEST_AND_SET_IS_SOMETIMES_NATIVE)     \
       || defined(Q_ATOMIC_POINTER_TEST_AND_SET_IS_NOT_NATIVE))
#    error "Define only one of Q_ATOMIC_POINTER_TEST_AND_SET_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(Q_ATOMIC_POINTER_TEST_AND_SET_IS_SOMETIMES_NATIVE)
    // could be either, just want to make sure the function is implemented
    QVERIFY(QAtomicPointer<void>::isTestAndSetNative()
            || !QAtomicPointer<void>::isTestAndSetNative());

#  if (defined(Q_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE) \
       || defined(Q_ATOMIC_POINTER_TEST_AND_SET_IS_NOT_NATIVE))
#    error "Define only one of Q_ATOMIC_POINTER_TEST_AND_SET_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(Q_ATOMIC_POINTER_TEST_AND_SET_IS_NOT_NATIVE)
    // the runtime test should say the same thing
    QVERIFY(!QAtomicPointer<void>::isTestAndSetNative());

#  if (defined(Q_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE) \
       || defined(Q_ATOMIC_POINTER_TEST_AND_SET_IS_SOMETIMES_NATIVE))
#    error "Define only one of Q_ATOMIC_POINTER_TEST_AND_SET_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#else
#  error "Q_ATOMIC_POINTER_TEST_AND_SET_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE is not defined"
#endif
}

void tst_QAtomicPointer::isTestAndSetWaitFree()
{
#if defined(Q_ATOMIC_POINTER_TEST_AND_SET_IS_WAIT_FREE)
    // the runtime test should say the same thing
    QVERIFY(QAtomicPointer<void>::isTestAndSetWaitFree());

    // enforce some invariants
    QVERIFY(QAtomicPointer<void>::isTestAndSetNative());
#  if defined(Q_ATOMIC_POINTER_TEST_AND_SET_IS_NOT_NATIVE)
#    error "Reference counting cannot be wait-free and unsupported at the same time!"
#  endif
#else
    // the runtime test should say the same thing
    QVERIFY(!QAtomicPointer<void>::isTestAndSetWaitFree());
#endif
}

void tst_QAtomicPointer::testAndSet()
{
    void *one = this;
    void *two = &one;
    void *three = &two;

    {
        QAtomicPointer<void> atomic1 = one;
        QAtomicPointer<void> atomic2 = two;
        QAtomicPointer<void> atomic3 = three;

        QCOMPARE(atomic1.load(), one);
        QCOMPARE(atomic2.load(), two);
        QVERIFY(atomic3.load() == three);

        QVERIFY(atomic1.testAndSetRelaxed(one, two));
        QVERIFY(atomic2.testAndSetRelaxed(two, three));
        QVERIFY(atomic3.testAndSetRelaxed(three, one));

        QVERIFY(atomic1.load() == two);
        QVERIFY(atomic2.load() == three);
        QVERIFY(atomic3.load() == one);
    }

    {
        QAtomicPointer<void> atomic1 = one;
        QAtomicPointer<void> atomic2 = two;
        QAtomicPointer<void> atomic3 = three;

        QCOMPARE(atomic1.load(), one);
        QCOMPARE(atomic2.load(), two);
        QVERIFY(atomic3.load() == three);

        QVERIFY(atomic1.testAndSetAcquire(one, two));
        QVERIFY(atomic2.testAndSetAcquire(two, three));
        QVERIFY(atomic3.testAndSetAcquire(three, one));

        QVERIFY(atomic1.load() == two);
        QVERIFY(atomic2.load() == three);
        QVERIFY(atomic3.load() == one);
    }

    {
        QAtomicPointer<void> atomic1 = one;
        QAtomicPointer<void> atomic2 = two;
        QAtomicPointer<void> atomic3 = three;

        QCOMPARE(atomic1.load(), one);
        QCOMPARE(atomic2.load(), two);
        QVERIFY(atomic3.load() == three);

        QVERIFY(atomic1.testAndSetRelease(one, two));
        QVERIFY(atomic2.testAndSetRelease(two, three));
        QVERIFY(atomic3.testAndSetRelease(three, one));

        QVERIFY(atomic1.load() == two);
        QVERIFY(atomic2.load() == three);
        QVERIFY(atomic3.load() == one);
    }

    {
        QAtomicPointer<void> atomic1 = one;
        QAtomicPointer<void> atomic2 = two;
        QAtomicPointer<void> atomic3 = three;

        QCOMPARE(atomic1.load(), one);
        QCOMPARE(atomic2.load(), two);
        QVERIFY(atomic3.load() == three);

        QVERIFY(atomic1.testAndSetOrdered(one, two));
        QVERIFY(atomic2.testAndSetOrdered(two, three));
        QVERIFY(atomic3.testAndSetOrdered(three, one));

        QVERIFY(atomic1.load() == two);
        QVERIFY(atomic2.load() == three);
        QVERIFY(atomic3.load() == one);
    }
}

void tst_QAtomicPointer::isFetchAndStoreNative()
{
#if defined(Q_ATOMIC_POINTER_FETCH_AND_STORE_IS_ALWAYS_NATIVE)
    // the runtime test should say the same thing
    QVERIFY(QAtomicPointer<void>::isFetchAndStoreNative());

#  if (defined(Q_ATOMIC_POINTER_FETCH_AND_STORE_IS_SOMETIMES_NATIVE)     \
       || defined(Q_ATOMIC_POINTER_FETCH_AND_STORE_IS_NOT_NATIVE))
#    error "Define only one of Q_ATOMIC_POINTER_FETCH_AND_STORE_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(Q_ATOMIC_POINTER_FETCH_AND_STORE_IS_SOMETIMES_NATIVE)
    // could be either, just want to make sure the function is implemented
    QVERIFY(QAtomicPointer<void>::isFetchAndStoreNative()
            || !QAtomicPointer<void>::isFetchAndStoreNative());

#  if (defined(Q_ATOMIC_POINTER_FETCH_AND_STORE_IS_ALWAYS_NATIVE) \
       || defined(Q_ATOMIC_POINTER_FETCH_AND_STORE_IS_NOT_NATIVE))
#    error "Define only one of Q_ATOMIC_POINTER_FETCH_AND_STORE_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(Q_ATOMIC_POINTER_FETCH_AND_STORE_IS_NOT_NATIVE)
    // the runtime test should say the same thing
    QVERIFY(!QAtomicPointer<void>::isFetchAndStoreNative());

#  if (defined(Q_ATOMIC_POINTER_FETCH_AND_STORE_IS_ALWAYS_NATIVE) \
       || defined(Q_ATOMIC_POINTER_FETCH_AND_STORE_IS_SOMETIMES_NATIVE))
#    error "Define only one of Q_ATOMIC_POINTER_FETCH_AND_STORE_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#else
#  error "Q_ATOMIC_POINTER_FETCH_AND_STORE_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE is not defined"
#endif
}

void tst_QAtomicPointer::isFetchAndStoreWaitFree()
{
#if defined(Q_ATOMIC_POINTER_FETCH_AND_STORE_IS_WAIT_FREE)
    // the runtime test should say the same thing
    QVERIFY(QAtomicPointer<void>::isFetchAndStoreWaitFree());

    // enforce some invariants
    QVERIFY(QAtomicPointer<void>::isFetchAndStoreNative());
#  if defined(Q_ATOMIC_POINTER_FETCH_AND_STORE_IS_NOT_NATIVE)
#    error "Reference counting cannot be wait-free and unsupported at the same time!"
#  endif
#else
    // the runtime test should say the same thing
    QVERIFY(!QAtomicPointer<void>::isFetchAndStoreWaitFree());
#endif
}

void tst_QAtomicPointer::fetchAndStore()
{
    void *one = this;
    void *two = &one;
    void *three = &two;

    {
        QAtomicPointer<void> atomic1 = one;
        QAtomicPointer<void> atomic2 = two;
        QAtomicPointer<void> atomic3 = three;

        QCOMPARE(atomic1.load(), one);
        QCOMPARE(atomic2.load(), two);
        QVERIFY(atomic3.load() == three);

        QCOMPARE(atomic1.fetchAndStoreRelaxed(two), one);
        QCOMPARE(atomic2.fetchAndStoreRelaxed(three), two);
        QCOMPARE(atomic3.fetchAndStoreRelaxed(one), three);

        QVERIFY(atomic1.load() == two);
        QVERIFY(atomic2.load() == three);
        QVERIFY(atomic3.load() == one);
    }

    {
        QAtomicPointer<void> atomic1 = one;
        QAtomicPointer<void> atomic2 = two;
        QAtomicPointer<void> atomic3 = three;

        QCOMPARE(atomic1.load(), one);
        QCOMPARE(atomic2.load(), two);
        QVERIFY(atomic3.load() == three);

        QCOMPARE(atomic1.fetchAndStoreAcquire(two), one);
        QCOMPARE(atomic2.fetchAndStoreAcquire(three), two);
        QCOMPARE(atomic3.fetchAndStoreAcquire(one), three);

        QVERIFY(atomic1.load() == two);
        QVERIFY(atomic2.load() == three);
        QVERIFY(atomic3.load() == one);
    }

    {
        QAtomicPointer<void> atomic1 = one;
        QAtomicPointer<void> atomic2 = two;
        QAtomicPointer<void> atomic3 = three;

        QCOMPARE(atomic1.load(), one);
        QCOMPARE(atomic2.load(), two);
        QVERIFY(atomic3.load() == three);

        QCOMPARE(atomic1.fetchAndStoreRelease(two), one);
        QCOMPARE(atomic2.fetchAndStoreRelease(three), two);
        QCOMPARE(atomic3.fetchAndStoreRelease(one), three);

        QVERIFY(atomic1.load() == two);
        QVERIFY(atomic2.load() == three);
        QVERIFY(atomic3.load() == one);
    }

    {
        QAtomicPointer<void> atomic1 = one;
        QAtomicPointer<void> atomic2 = two;
        QAtomicPointer<void> atomic3 = three;

        QCOMPARE(atomic1.load(), one);
        QCOMPARE(atomic2.load(), two);
        QVERIFY(atomic3.load() == three);

        QCOMPARE(atomic1.fetchAndStoreOrdered(two), one);
        QCOMPARE(atomic2.fetchAndStoreOrdered(three), two);
        QCOMPARE(atomic3.fetchAndStoreOrdered(one), three);

        QVERIFY(atomic1.load() == two);
        QVERIFY(atomic2.load() == three);
        QVERIFY(atomic3.load() == one);
    }
}

void tst_QAtomicPointer::isFetchAndAddNative()
{
#if defined(Q_ATOMIC_POINTER_FETCH_AND_ADD_IS_ALWAYS_NATIVE)
    // the runtime test should say the same thing
    QVERIFY(QAtomicPointer<void>::isFetchAndAddNative());

#  if (defined(Q_ATOMIC_POINTER_FETCH_AND_ADD_IS_SOMETIMES_NATIVE)     \
       || defined(Q_ATOMIC_POINTER_FETCH_AND_ADD_IS_NOT_NATIVE))
#    error "Define only one of Q_ATOMIC_POINTER_FETCH_AND_ADD_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(Q_ATOMIC_POINTER_FETCH_AND_ADD_IS_SOMETIMES_NATIVE)
    // could be either, just want to make sure the function is implemented
    QVERIFY(QAtomicPointer<void>::isFetchAndAddNative()
            || !QAtomicPointer<void>::isFetchAndAddNative());

#  if (defined(Q_ATOMIC_POINTER_FETCH_AND_ADD_IS_ALWAYS_NATIVE) \
       || defined(Q_ATOMIC_POINTER_FETCH_AND_ADD_IS_NOT_NATIVE))
#    error "Define only one of Q_ATOMIC_POINTER_FETCH_AND_ADD_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(Q_ATOMIC_POINTER_FETCH_AND_ADD_IS_NOT_NATIVE)
    // the runtime test should say the same thing
    QVERIFY(!QAtomicPointer<void>::isFetchAndAddNative());

#  if (defined(Q_ATOMIC_POINTER_FETCH_AND_ADD_IS_ALWAYS_NATIVE) \
       || defined(Q_ATOMIC_POINTER_FETCH_AND_ADD_IS_SOMETIMES_NATIVE))
#    error "Define only one of Q_ATOMIC_POINTER_FETCH_AND_ADD_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#else
#  error "Q_ATOMIC_POINTER_FETCH_AND_ADD_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE is not defined"
#endif
}

void tst_QAtomicPointer::isFetchAndAddWaitFree()
{
#if defined(Q_ATOMIC_POINTER_FETCH_AND_ADD_IS_WAIT_FREE)
    // the runtime test should say the same thing
    QVERIFY(QAtomicPointer<void>::isFetchAndAddWaitFree());

    // enforce some invariants
    QVERIFY(QAtomicPointer<void>::isFetchAndAddNative());
#  if defined(Q_ATOMIC_POINTER_FETCH_AND_ADD_IS_NOT_NATIVE)
#    error "Reference counting cannot be wait-free and unsupported at the same time!"
#  endif
#else
    // the runtime test should say the same thing
    QVERIFY(!QAtomicPointer<void>::isFetchAndAddWaitFree());
#endif
}

void tst_QAtomicPointer::fetchAndAdd_data()
{
    QTest::addColumn<int>("valueToAdd");

    QTest::newRow("0") << 0;
    QTest::newRow("1") << 1;
    QTest::newRow("2") << 2;
    QTest::newRow("10") << 10;
    QTest::newRow("31") << 31;
    QTest::newRow("51") << 51;
    QTest::newRow("72") << 72;
    QTest::newRow("810") << 810;
    QTest::newRow("631") << 631;
    QTest::newRow("451") << 451;
    QTest::newRow("272") << 272;
    QTest::newRow("1810") << 1810;
    QTest::newRow("3631") << 3631;
    QTest::newRow("5451") << 5451;
    QTest::newRow("7272") << 7272;
    QTest::newRow("-1") << -1;
    QTest::newRow("-2") << -2;
    QTest::newRow("-10") << -10;
    QTest::newRow("-31") << -31;
    QTest::newRow("-51") << -51;
    QTest::newRow("-72") << -72;
    QTest::newRow("-810") << -810;
    QTest::newRow("-631") << -631;
    QTest::newRow("-451") << -451;
    QTest::newRow("-272") << -272;
    QTest::newRow("-1810") << -1810;
    QTest::newRow("-3631") << -3631;
    QTest::newRow("-5451") << -5451;
    QTest::newRow("-7272") << -7272;
}

void tst_QAtomicPointer::fetchAndAdd()
{
    QFETCH(int, valueToAdd);

    char c;
    char *pc = &c;
    short s;
    short *ps = &s;
    int i;
    int *pi = &i;

    {
        QAtomicPointer<char> pointer1 = pc;
        QCOMPARE(pointer1.fetchAndAddRelaxed(valueToAdd), pc);
        QCOMPARE(pointer1.fetchAndAddRelaxed(-valueToAdd), pc + valueToAdd);
        QCOMPARE(pointer1.load(), pc);
        QAtomicPointer<short> pointer2 = ps;
        QCOMPARE(pointer2.fetchAndAddRelaxed(valueToAdd), ps);
        QCOMPARE(pointer2.fetchAndAddRelaxed(-valueToAdd), ps + valueToAdd);
        QCOMPARE(pointer2.load(), ps);
        QAtomicPointer<int> pointer3 = pi;
        QCOMPARE(pointer3.fetchAndAddRelaxed(valueToAdd), pi);
        QCOMPARE(pointer3.fetchAndAddRelaxed(-valueToAdd), pi + valueToAdd);
        QCOMPARE(pointer3.load(), pi);
    }

    {
        QAtomicPointer<char> pointer1 = pc;
        QCOMPARE(pointer1.fetchAndAddAcquire(valueToAdd), pc);
        QCOMPARE(pointer1.fetchAndAddAcquire(-valueToAdd), pc + valueToAdd);
        QCOMPARE(pointer1.load(), pc);
        QAtomicPointer<short> pointer2 = ps;
        QCOMPARE(pointer2.fetchAndAddAcquire(valueToAdd), ps);
        QCOMPARE(pointer2.fetchAndAddAcquire(-valueToAdd), ps + valueToAdd);
        QCOMPARE(pointer2.load(), ps);
        QAtomicPointer<int> pointer3 = pi;
        QCOMPARE(pointer3.fetchAndAddAcquire(valueToAdd), pi);
        QCOMPARE(pointer3.fetchAndAddAcquire(-valueToAdd), pi + valueToAdd);
        QCOMPARE(pointer3.load(), pi);
    }

    {
        QAtomicPointer<char> pointer1 = pc;
        QCOMPARE(pointer1.fetchAndAddRelease(valueToAdd), pc);
        QCOMPARE(pointer1.fetchAndAddRelease(-valueToAdd), pc + valueToAdd);
        QCOMPARE(pointer1.load(), pc);
        QAtomicPointer<short> pointer2 = ps;
        QCOMPARE(pointer2.fetchAndAddRelease(valueToAdd), ps);
        QCOMPARE(pointer2.fetchAndAddRelease(-valueToAdd), ps + valueToAdd);
        QCOMPARE(pointer2.load(), ps);
        QAtomicPointer<int> pointer3 = pi;
        QCOMPARE(pointer3.fetchAndAddRelease(valueToAdd), pi);
        QCOMPARE(pointer3.fetchAndAddRelease(-valueToAdd), pi + valueToAdd);
        QCOMPARE(pointer3.load(), pi);
    }

    {
        QAtomicPointer<char> pointer1 = pc;
        QCOMPARE(pointer1.fetchAndAddOrdered(valueToAdd), pc);
        QCOMPARE(pointer1.fetchAndAddOrdered(-valueToAdd), pc + valueToAdd);
        QCOMPARE(pointer1.load(), pc);
        QAtomicPointer<short> pointer2 = ps;
        QCOMPARE(pointer2.fetchAndAddOrdered(valueToAdd), ps);
        QCOMPARE(pointer2.fetchAndAddOrdered(-valueToAdd), ps + valueToAdd);
        QCOMPARE(pointer2.load(), ps);
        QAtomicPointer<int> pointer3 = pi;
        QCOMPARE(pointer3.fetchAndAddOrdered(valueToAdd), pi);
        QCOMPARE(pointer3.fetchAndAddOrdered(-valueToAdd), pi + valueToAdd);
        QCOMPARE(pointer3.load(), pi);
    }
}

QTEST_APPLESS_MAIN(tst_QAtomicPointer)
#include "tst_qatomicpointer.moc"
