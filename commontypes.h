#pragma once

#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QFlags>
#include <QRectF>
#include <QTime>
#include <QWidget>
#include <QPixmap>
#include <QDebug>


#include <vector>

enum ShapeType {
   FCirc,
   FRect,
   FLine,
   NUM_SHAPES,
};

typedef unsigned int uint32;
typedef unsigned char uint8;

#define FAT_FACTOR 18.0


// debug flags
#define DRAW_AABB 1
#define DRAW_INTERSECTIONS 0
