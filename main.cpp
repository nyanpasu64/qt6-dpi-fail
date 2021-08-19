#include <QApplication>
#include <QWidget>

#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QScreen>
#include <QWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget c;

    auto l = new QVBoxLayout;
    c.setLayout(l);
    l->setSizeConstraint(QLayout::SetFixedSize);

    auto w = new QPushButton("IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII");
    l->addWidget(w);
    w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    c.show();

    QObject::connect(w, &QPushButton::pressed, w, [w]() {
        qDebug() << w->sizeHint();
    });
    auto window = w->nativeParentWidget()->windowHandle();
    if (!window) {
        throw "up";
    }
    QObject::connect(window, &QWindow::screenChanged, window, [window]() {
        qDebug() << window->screen()->logicalDotsPerInch();
    });

    return a.exec();
}
