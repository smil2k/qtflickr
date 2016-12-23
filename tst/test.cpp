#include <QtQuickTest/quicktest.h>
#include <QQmlApplicationEngine>
#include "oauth/oauth.h"

// Fool QtCreator to find the tests
namespace x {
    QUICK_TEST_MAIN(all)
}

int main(int argc, char *argv[])
{
    QTEST_ADD_GPU_BLACKLIST_SUPPORT
    QTEST_SET_MAIN_SOURCE_PATH
    qmlRegisterType<OAuth>("Social", 1, 0, "OAuth");

    return quick_test_main(argc, argv, "all", QUICK_TEST_SOURCE_DIR );
}

