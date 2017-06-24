#include "../../include/solverintent.h"

#include <QtAndroid>
#include <QAndroidActivityResultReceiver>

#define SOLVER_REQUEST 1936682102 // "solv"
#define RESULT_OK -1
#define MATCH_DEFAULT_ONLY 65536

SolverIntent::SolverIntent(QObject *parent)
    : QObject(parent)
{
}

bool SolverIntent::isIntentAvailable(){
    QAndroidJniObject intent = getSolverIntent();
    QAndroidJniObject jPackageManager = QtAndroid::androidActivity().callObjectMethod("getPackageManager", "()"
                                                                                      "Landroid/content/pm/PackageManager;");
    QAndroidJniObject jList = jPackageManager.callObjectMethod("queryIntentActivities", "(Landroid/content/Intent;I)"
                                                              "Ljava/util/List;", intent.object<jobject>(), MATCH_DEFAULT_ONLY);

    jint jSize = jList.callMethod<jint>("size", "()I");
    return (jSize > 0);
}

void SolverIntent::callSolver(const QString &level){
    QAndroidJniObject intent = getSolverIntent();
    QAndroidJniObject jIntentExtraName = QAndroidJniObject::fromString("LEVEL");
    QAndroidJniObject jIntentExtraValue = QAndroidJniObject::fromString(level);
    intent.callObjectMethod("putExtra", "(Ljava/lang/String;Ljava/lang/String;)"
                                    "Landroid/content/Intent;", jIntentExtraName.object<jstring>(), jIntentExtraValue.object<jstring>());

    QtAndroid::startActivity(intent, SOLVER_REQUEST, this);
}

QAndroidJniObject SolverIntent::getSolverIntent(){
    QAndroidJniObject jIntentAction = QAndroidJniObject::fromString("nl.joriswit.sokosolver.SOLVE");
    QAndroidJniObject intent("android/content/Intent", "(Ljava/lang/String;)V", jIntentAction.object<jstring>());
    return intent;
}

void SolverIntent::handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data){
    if(receiverRequestCode == SOLVER_REQUEST && resultCode == RESULT_OK){
        QAndroidJniObject jIntentExtraName = QAndroidJniObject::fromString("SOLUTION");
        QAndroidJniObject jSolution = data.callObjectMethod("getStringExtra", "(Ljava/lang/String;)"
                                        "Ljava/lang/String;", jIntentExtraName.object<jstring>());
        QString solution = jSolution.toString();
        emit solutionFound(solution);
    }
}
