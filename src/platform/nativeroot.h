#ifndef NATIVEROOT_H
#define NATIVEROOT_H

#include <QWidget>

class NativeRoot : public QWidget
{
    Q_OBJECT
public:
    explicit NativeRoot(WId window)
    {
        create(window, false, false);
    }
};

#endif // NATIVEROOT_H
