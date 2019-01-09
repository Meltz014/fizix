#pragma once

#include <QBrush>
#include <QColor>
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

#define FAT_FACTOR (18.0)
// (0.5) normally
#define GRAVITY (0.0)
#define DEFAULT_BRUSH QBrush()
#define DEFAULT_PEN QPen( Qt::blue, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin )
#define RED_PEN QPen( Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin )


// debug flags
#define DRAW_AABB 1
#define DRAW_INTERSECTIONS 0
