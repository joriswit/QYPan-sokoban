#ifndef MAPMANAGER_H
#define MAPMANAGER_H

// 地图管理

//#include "navigatealgorithm.h"

#include <QObject>
#include <QString>
#include <QVector>
#include <QList>

struct MapInfo {
    MapInfo () : row(0), column(0), cells("") {}
    int row, column;
    QString cells; // 地图字符串
};

struct BoxState {
    BoxState() : box_position(0), man_position(0) {}
    int box_position;
    int man_position;
};

struct BoxStateNode {
    BoxStateNode(int box_poisition = 0, int man_poisition = 0)
        : path(""),
          previous(NULL),
          next_count(0)
    {
        state.box_position = box_poisition;
        state.man_position = man_poisition;
        for(int i = 0; i < 4; i++) next[i] = NULL;
    }

    BoxStateNode *next[4];
    BoxStateNode *prevoius;
    BoxState state;
    QString path;
    int next_count;
};

class MapManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(MapType)
    Q_PROPERTY(int max_classic_level READ maxClassicLevel WRITE setMaxClassicLevel NOTIFY maxClassicLevelChanged)
    Q_PROPERTY(int max_self_make_level READ maxSelfMakeLevel WRITE setMaxSelfMakeLevel NOTIFY maxSelfMakeLevelChanged)

public:

    enum MapType {CLASSIC = 0, SELF_MAKE};

public:

    MapManager(QObject *parent = 0);
    Q_INVOKABLE bool loadMap(MapType type);
    Q_INVOKABLE bool loadMap(MapType type, int level);
    Q_INVOKABLE QString openMap(MapType type, int level);

    Q_INVOKABLE QString touchPosition(int row, int column);
    Q_INVOKABLE void setCell(int row, int column, const QString &type);

   int maxClassicLevel() const;
   int maxSelfMakeLevel() const;
   void setMaxClassicLevel(int level);
   void setMaxSelfMakeLevel(int level);

signals:

    maxClassicLevelChanged(int level);
    maxSelfMakeLevelChanged(int level);

private slots:

private:

    bool loadMap(MapType type, const QString &path);
    QString manPath(int from, int to);
    QString touchFloor(int position);
    QString touchBox(int position);

private:

    QList<MapInfo> classic_maps_;
    QList<MapInfo> self_make_maps_;
    BoxStateNode *box_state_root_;
    QString classic_maps_directory_;
    QString self_make_maps_directory_;
    MapInfo opened_map_info_;
    MapInfo static_map_info_; // 只含墙和目标点的地图
    MapInfo dynamic_map_info_; // 只含墙、人和箱子的地图
    int max_classic_level_;
    int max_self_make_level_;
};

#endif // MAPMANAGER_H
