#include <QWidget>

#include <QListWidget>
#include <QVBoxLayout>

#include <QApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget c;

    auto l = new QVBoxLayout;
    c.setLayout(l);

    {
        auto w = new QListWidget;
        l->addWidget(w);

        w->setDragDropMode(QAbstractItemView::InternalMove);
        w->setDragDropOverwriteMode(true);

        w->addItem("hello");
        w->addItem("world");
    }

    c.show();
    return a.exec();
}
