#include "../../include/navigatealgorithm.h"

#include <QQueue>
#include <QVector>
#include <QChar>

QString NavigateAlgorithm::reachableCells(const MapInfo &mapInfo, BoxStateNode *&root, int position){
    int go_x[4] = {0, 0, -1, 1};
    int go_y[4] = {-1, 1, 0, 0};
    int max_row = mapInfo.row;
    int max_column = mapInfo.column;
    QString result(mapInfo.cells.length(), QChar('0'));
    QVector<QVector<bool> > mark(mapInfo.cells.length(), QVector<bool>(4, false));
    BoxStateNode *start_state_node_ptr = new BoxStateNode(position, mapInfo.cells.indexOf(QChar('@')));
    root = start_state_node_ptr;
    QQueue<*BoxStateNode> queue;
    queue.enqueue(start_state_node_ptr);
    while(!queue.isEmpty()){
        BoxStateNode *current_state_node_ptr = queue.dequeue();
        int current_box_position = current_state_node_ptr->state.box_position;
        int current_box_row = current_box_position / mapInfo.column;
        int current_box_column = current_box_position % mapInfo.column;
        result[current_box_position] = QChar('1');
        for(int i = 0; i < 4; i++){
            int expect_man_row = current_box_row - go_y[i];
            int expect_man_column = current_box_column - go_x[i];
            int expect_man_position = expect_man_row * max_column + expect_man_column;
            bool reachable = true;
            QString path = manPath(mapInfo, current_state_node_ptr->state.man_position, expect_man_position, reachable);
            if(reachable == false){
                continue;
            }
            mark[current_box_position][i] = true;
            int next_box_row = current_box_row + go_y[i];
            int next_box_column = current_box_column + go_x[i];
            if(next_box_row < 0 || next_box_row >= max_row || next_box_column < 0 || next_box_column >= max_column){
                continue;
            }
            int next_box_position = next_box_row * max_column + next_box_column;
            if(mapInfo.cells[next_box_position] == QChar('#') ||
                    mapInfo.cells[next_box_position] == QChar('$')){
                continue;
            }
            if(mark[next_box_position][i] == QChar('1')){
                continue;
            }
            BoxStateNode *next_state_node_ptr = new BoxStateNode(next_box_position, current_state_node_ptr->state.box_position);
            next_state_node_ptr->path = path;
            current_state_node_ptr->next[current_state_node_ptr->next_count++] = next_state_node_ptr;
            next_state_node_ptr->prevoius = current_state_node_ptr;
            queue.enqueue(next_state_node_ptr);
        }
    }
    return result;
}

QString NavigateAlgorithm::manPath(const MapInfo &mapInfo, int start_position, int end_position, bool &reachable){
    int go_x[4] = {0, 0, -1, 1};
    int go_y[4] = {-1, 1, 0, 0};
    char direction[5] = {"udlr"};

    struct PositionState{
        PositionState() : row(0), column(0), path("") {}
        int row, column;
        QString path;
    };

    int max_row = mapInfo.row;
    int max_column = mapInfo.column;

    QQueue<PositionState> queue;
    QVector<bool> mark(mapInfo.cells.length(), false);
    mark[start_position] = true;
    PositionState start_state;
    start_state.row = start_position / max_column;
    start_state.column = start_position % max_column;
    queue.enqueue(start_state);
    while(!queue.isEmpty()){
        PositionState current_state = queue.dequeue();
        int position = current_state.row * max_column + current_state.column;
        if(position == end_position){
            reachable = true;
            return current_state.path;
        }
        for(int i = 0; i < 4; i++){
            PositionState new_state;
            new_state.row = current_state.row + go_y[i];
            new_state.column = current_state.column + go_x[i];
            //qDebug() << "new row[" << new_state.row << "]";
            //qDebug() << "new column[" << new_state.column << "]";
            int new_position = new_state.row * max_column + new_state.column;
            if(new_state.row < 0 || new_state.row >= max_row || new_state.column < 0 || new_state.column >= max_column){
                continue;
            }
            if(mapInfo.cells[new_position] == QChar('#') ||
                    mapInfo.cells[new_position] == QChar('$')){
                continue;
            }
            if(mark[new_position] == true){
                continue;
            }
            mark[new_position] = true;
            new_state.path = current_state.path + direction[i];
            queue.enqueue(new_state);
        }
    }
    reachable = false;
    return "";
}
