//
// Created by wuyuanyi on 15/07/19.
//

#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include "NumberDisplay.h"

int main( int argc, char **argv )
{
    QApplication a( argc, argv );

    QPushButton hello( "Hello world!", 0 );
    hello.resize( 100, 30 );

    hello.show();
    return a.exec();
}
