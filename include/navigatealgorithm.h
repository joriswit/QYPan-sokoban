#ifndef NAVIGATEALGORITHM_H
#define NAVIGATEALGORITHM_H

#include "mapmanager.h"

#include <QString>
#include <QObject>

// 寻路算法实现

class NavigateAlgorithm{
public:

    static QString manPath(const MapInfo &mapInfo, int start_position, int end_position, bool &reachable);
    static QString reachableCells(const MapInfo &mapInfo, BoxStateNode *&root, int man_position, int box_position);
    static QString pushPath(BoxStateNode *root, int position);
    static bool setPath(BoxStateNode *node, int position, QString &path);
    static void freeBoxStateNodes(BoxStateNode *root);

private:

    NavigateAlgorithm();

};

#endif // NAVIGATEALGORITHM_H
