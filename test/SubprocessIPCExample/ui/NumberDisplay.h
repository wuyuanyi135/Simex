//
// Created by wuyuanyi on 17/07/19.
//

#ifndef SIMEX_NUMBERDISPLAY_H
#define SIMEX_NUMBERDISPLAY_H

#include <QtWidgets/QPushButton>
class NumberDisplay : public QPushButton {
  Q_OBJECT
 public:
  NumberDisplay(QWidget *parent);
 public slots:
      void updateContent(double content);

};
#endif //SIMEX_NUMBERDISPLAY_H
