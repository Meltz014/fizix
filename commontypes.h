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
   FRect,
   FCirc,
   FLine,
   NUM_SHAPES,
};

typedef unsigned int uint32;

#define FAT_FACTOR 10.0
