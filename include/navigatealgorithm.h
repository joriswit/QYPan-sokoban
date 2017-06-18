#ifndef NAVIGATEALGORITHM_H
#define NAVIGATEALGORITHM_H

#include "mapmanager.h"

#include <QString>
#include <QObject>

// 寻路算法实现

class NavigateAlgorithm{
public:

    static QString manPath(const MapInfo &mapInfo, int start_position, int end_position, bool &reachable);
    static QString reachableCells(const MapInfo &mapInfo, BoxStateNode *&root, int position);

private:

    NavigateAlgorithm();

};

#endif // NAVIGATEALGORITHM_H
