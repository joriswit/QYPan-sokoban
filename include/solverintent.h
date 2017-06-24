#ifndef SOLVERINTENT_H
#define SOLVERINTENT_H

#include <QObject>
#include <QString>
#include <QAndroidActivityResultReceiver>

class SolverIntent : public QObject, public QAndroidActivityResultReceiver
{
    Q_OBJECT

public:

    SolverIntent(QObject *parent = 0);
    Q_INVOKABLE bool isIntentAvailable();
    Q_INVOKABLE void callSolver(const QString &level);

signals:

    void solutionFound(QString solution);

private:

    QAndroidJniObject getSolverIntent();
    void handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data);

};

#endif // SOLVERINTENT_H
